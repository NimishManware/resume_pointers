import React, { useState, useEffect } from "react";
import ReactDataSheet from "react-datasheet";
import "react-datasheet/lib/react-datasheet.css";
import { saveAs } from "file-saver";
import { apiUrl } from "../config/config";

const Sheet = ({ dbName , tableName }) => {
  const [grid, setGrid] = useState([]);
  const [newRowGrid , setNewRowGrid] = useState([]);
  const [formulaGrid , setFormulaGrid] = useState([[{ value: "| Enter Formula |", expr: "", readOnly: false }]]);
  const [pkeyCols, setPkeyCols] = useState([]);
  const [notNullCols, setNoNullCols] = useState([]);
  const [cols, setCols] = useState([]);
  const [rawData, setRawData] = useState([]);
  const [message, setMessage] = useState("");
  const [messageColor, setMessageColor] = useState("");
  

  const fetchData = async () => {
    try {
      const res = await fetch(`${apiUrl}/get-table-data`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ dbName , tableName }),
      });

      const result = await res.json();

      if (res.ok) {
        setRawData(result.data);
        setPkeyCols(result.primarykeycols);
        setCols(result.columns);
        setNoNullCols(result.notNullColumns);
        setMessage(`Data for ${tableName} loaded successfully.`);
        setMessageColor("green");

        const headerRow = result.columns.map((col) => ({
          value: col,
          readOnly: true,
        }));

        const dataRows = result.data.map((row) =>
          result.columns.map((col) => ({
            value: row[col],
            pkey: row["primarykeyvals"],
          }))
        );
        const dummyRow = result.columns.map((col) => ({
          value: "|   Enter Data   |",
          pkey: [],
        }));
        setNewRowGrid([dummyRow]);
        setGrid([headerRow, ...dataRows]);
      } else {
        setGrid([]);
        setMessage(result.message);
        setMessageColor("red");
      }
    } catch (err) {
      setGrid([]);
      setMessage(err.message);
      setMessageColor("red");
    }
  };

  useEffect(() => {
    fetchData();
  }, [tableName]);

  useEffect(() => {
    evaluateFormulaCell();
  },[grid]);

  const handleNewRow = (changes) => {
    const updatedGrid = newRowGrid.map((row) => [...row]);
    for (const { cell, row, col, value } of changes) {
      updatedGrid[row][col] = { ...cell, value };
    }
    setNewRowGrid(updatedGrid);
  };

  const getCoordinates = (address) => {
    const match = address.match(/^([A-Z]+)(\d+)$/);
    const [,word,row] = match;
    let col = 0;
    for(let i=0 ; i<word.length ; i++){
       col *= 26;
       col += word.charCodeAt(i) - 64;
    }
    return {
     row: parseInt(row, 10),
     col: col - 1, 
   };
 }

  const handleFormula = (changes) => {
    const temp = formulaGrid.map((row) => [...row]);
    let ans = "| Enter Formula |";
    let currentFormula = "";
    for (const { cell, row, col, value } of changes) {
      const match = value.match(/^=([A-Z]+)\(\s*([A-Z]+\d+)\s*:\s*([A-Z]+\d+)\s*\)$/);
      if (match) {   
        const formulaName = match[1];    
        const address1 = match[2];        
        const address2 = match[3];    
        const coord1 = getCoordinates(address1);
        const coord2 = getCoordinates(address2);
        const r1 = Math.min(coord1.row, coord2.row);
        const r2 = Math.max(coord1.row, coord2.row);
        const c1 = Math.min(coord1.col, coord2.col);
        const c2 = Math.max(coord1.col, coord2.col);
        const values = [];
        for (let r = r1; r <= r2; r++) {
          for (let c = c1; c <= c2; c++) {
            const cellVal = grid[r][c].value;
            const num = parseFloat(cellVal);
            if(!isNaN(num)) {
              values.push(num);
            }
          }
        }
        if(values.length === 0){
            setMessage("No numeric values in range");
            setMessageColor("red");  
        }
        else{
            if(formulaName ===  "SUM"){
              currentFormula = match[0];
              ans = values.reduce((a, b) => a + b, 0);
            } 
            else if(formulaName === "AVG"){
              currentFormula = match[0];
              ans = values.reduce((a, b) => a + b, 0) / values.length;
            } 
            else if(formulaName === "MIN"){
              currentFormula = match[0];
              ans = Math.min(...values);
            } 
            else if(formulaName == "MAX"){
              currentFormula = match[0];
              ans = Math.max(...values);
            }
            else if(formulaName == "COUNT"){
              currentFormula = match[0];
              ans = values.length;
            }
            else if (formulaName === "MEDIAN") {
              currentFormula = match[0];
              const sorted = [...values].sort((a, b) => a - b);
              const mid = Math.floor(sorted.length / 2);
              ans = sorted.length % 2 !== 0
                ? sorted[mid]
                : (sorted[mid - 1] + sorted[mid]) / 2;
            }
            else if (formulaName === "MODE") {
              currentFormula = match[0];
              const freq = {};
              values.forEach(v => freq[v] = (freq[v] || 0) + 1);
              ans = Object.keys(freq).reduce((a, b) => freq[a] > freq[b] ? a : b);
            }
            else if (formulaName === "PRODUCT") {
              currentFormula = match[0];
              ans = values.reduce((a, b) => a * b, 1);
            }
            else if (formulaName === "STDEV") {
              currentFormula = match[0];
              const mean = values.reduce((a, b) => a + b, 0) / values.length;
              const variance = values.reduce((acc, val) => acc + (val - mean) ** 2, 0) / (values.length - 1);
              ans = Math.sqrt(variance);
            }            
            else{
              setMessage("Formula not supported");
              setMessageColor("red");
            }
        }
      } 
      else {
        setMessage("Enter correct formula");
        setMessageColor("red");
      }
    }
    setFormulaGrid([[{ value: ans , expr: currentFormula , readOnly: false }]]);
  }
 
  const evaluateFormulaCell = () => {
      if(formulaGrid[0][0].expr === ""){
        return;
      }
      let currentFormula = formulaGrid[0][0].expr;
      let ans = "";
      //warna ek valid formula hai 
      const match = currentFormula.match(/^=([A-Z]+)\(\s*([A-Z]+\d+)\s*:\s*([A-Z]+\d+)\s*\)$/);
      const formulaName = match[1];    
      const address1 = match[2];        
      const address2 = match[3];    
      const coord1 = getCoordinates(address1);
      const coord2 = getCoordinates(address2);
      const r1 = Math.min(coord1.row, coord2.row);
      const r2 = Math.max(coord1.row, coord2.row);
      const c1 = Math.min(coord1.col, coord2.col);
      const c2 = Math.max(coord1.col, coord2.col);
      const values = [];
      for (let r = r1; r <= r2; r++) {
        for (let c = c1; c <= c2; c++) {
            const cellVal = grid[r][c].value;
            const num = parseFloat(cellVal);
            if(!isNaN(num)) {
              values.push(num);
            }
        }
      }
      if(formulaName ===  "SUM"){
        ans = values.reduce((a, b) => a + b, 0);
      } 
      else if(formulaName === "AVG"){
        ans = values.reduce((a, b) => a + b, 0) / values.length;
      } 
      else if(formulaName === "MIN"){
        ans = Math.min(...values);
      } 
      else if(formulaName == "MAX"){
        ans = Math.max(...values);
      }
      else if(formulaName == "COUNT"){
        ans = values.length;
      }
      else if (formulaName === "MEDIAN") {
        const sorted = [...values].sort((a, b) => a - b);
        const mid = Math.floor(sorted.length / 2);
        ans = sorted.length % 2 !== 0
          ? sorted[mid]
          : (sorted[mid - 1] + sorted[mid]) / 2;
      }
      else if (formulaName === "MODE") {
        const freq = {};
        values.forEach(v => freq[v] = (freq[v] || 0) + 1);
        ans = Object.keys(freq).reduce((a, b) => freq[a] > freq[b] ? a : b);
      }
      else if (formulaName === "PRODUCT") {
        ans = values.reduce((a, b) => a * b, 1);
      }
      else if (formulaName === "STDEV") {
        const mean = values.reduce((a, b) => a + b, 0) / values.length;
        const variance = values.reduce((acc, val) => acc + (val - mean) ** 2, 0) / (values.length - 1);
        ans = Math.sqrt(variance);
      }       
      setFormulaGrid([[{ value: ans , expr: currentFormula , readOnly: false }]]);
  }
  

  const handleCellsChanged = async (changes) => {
    const newGrid = grid.map((row) => [...row]);
    for (const { cell, row, col, value } of changes) {
      if (row === 0) continue; 
      const payload = {
        dbName,
        col: cols[col],
        newData: value,
        primaryKeyVals: cell.pkey,
        primaryKeyCols: pkeyCols,
        tableName: tableName,
      };
      try {
        const response = await fetch(`${apiUrl}/handle-cell-change`, {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify(payload),
        });
  
        const result = await response.json();
        if(response.status == 200) {
          const updatedCell = { ...cell, value };
          newGrid[row][col] = updatedCell;
          setMessage(result.message);
          setMessageColor("green");
        } else {
          setMessage(result.message);
          setMessageColor("red");
        }
      } catch (error) {
        setMessage(error);
        setMessageColor("red");
      }
    }
    setGrid(newGrid);
  };


  const getCharCode = (colNum) => {
    let ans  = "";
    let curr = colNum + 1;
    while (curr> 0) {
      let modulo = (curr - 1) % 26;
      ans = String.fromCharCode(65 + modulo) + ans;
      curr = Math.floor((curr - modulo) / 26);
    }
    return ans;
  }

  const cellRenderer = (props) => {
    const {
      row,
      col,
      cell,
      style,
      className,
      onMouseDown,
      onMouseOver,
      onDoubleClick,
      children,
    } = props;

    const isHeader = row === 0;
    const colName = cols[col];
    const isNotNull = notNullCols.includes(colName);
    const colLetter = getCharCode(col);
    const lastRow = grid.length - 1;
    const cellAddress = isHeader
      ? `${colLetter}1:${colLetter}${lastRow}`
      : `${colLetter}${row}`;

    let backgroundColor = "#f0f0f0"; 
    if (isHeader) {
      backgroundColor = isNotNull ? "#f8d7da" : "#d4edda"; // red / green
    }

    return (
      <td
        className={className}
        style={{ ...style, backgroundColor }}
        onMouseDown={onMouseDown}
        onMouseOver={onMouseOver}
        onDoubleClick={onDoubleClick}
        title={cellAddress}
      >
        {children}
      </td>
    );
  };
  
  const addRow = async () => {
      const newRow = newRowGrid[0];
      const newRowData = {};
      let valid = true;
      for(let i=0 ; i<cols.length ; i++){
         const colName = cols[i];
         const cellVal = newRow[i].value.trim();
         newRowData[colName] = cellVal;
         if(pkeyCols.includes(colName) && (cellVal === "" || cellVal === "|   Enter Data   |")){
               valid = false;
         }
      }
      if(!valid){
        setMessage("Please fill in all primary key fields");
        setMessageColor("red");
        const dummyRow = cols.map((col) => ({
          value: "|   Enter Data   |",
          pkey: [],
        }));
        setNewRowGrid([dummyRow]);
        return;
      }
      try{
        const res = await fetch(`${apiUrl}/add-row`, {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify({
            dbName,
            tableName,
            rowData: newRowData,
          }),
        });
        const data = await res.json();
        if(res.status === 200){
           setMessage(data.message);
           setMessageColor("green");

           fetchData();

           const dummyRow = cols.map((col) => ({
            value: "|   Enter Data   |",
            pkey: [],
          }));
          setNewRowGrid([dummyRow]);
        }
        else{
          setMessage(data.message);
          setMessageColor("red");
        }
      }
      catch(error){
        setMessage(error);
        setMessageColor("red");
      }

  }

  const exportToCSV = () => {
    if (grid.length === 0) return;

    const headers = grid[0].map(cell => cell.value);
    const rows = grid.slice(1).map(row =>
      row.map(cell => {
        return cell.value ?? "";
      })
    );

    const csv = [headers, ...rows].map(row => row.join(",")).join("\n");
    const blob = new Blob([csv], { type: "text/csv;charset=utf-8" });
    saveAs(blob, `${tableName || "table"}.csv`);
  };


  return (
    <div>
      <p style={{ color: messageColor }}>{message}</p>
      <button onClick={addRow} style={{ marginBottom: "10px" }}>
        Add Row
      </button>
      <button onClick={exportToCSV} style={{ marginBottom: "10px", marginLeft: "10px" }}>
        Export CSV
      </button>
      <br />
      <p>New Row :</p>
      <ReactDataSheet
        data = {newRowGrid}
        valueRenderer={(cell) => cell.value}
        onCellsChanged={handleNewRow}
        cellRenderer={cellRenderer}
      />
      <p>Formula Cell :</p>
      <ReactDataSheet
        data = {formulaGrid}
        valueRenderer={(cell) => cell.value}
        onCellsChanged={handleFormula}
      />

      <br /> <br />
      <ReactDataSheet
        data={grid}
        valueRenderer={(cell) => cell.value}
        onCellsChanged={handleCellsChanged}
        cellRenderer={cellRenderer}
      />
    </div>
  );
};

export default Sheet;

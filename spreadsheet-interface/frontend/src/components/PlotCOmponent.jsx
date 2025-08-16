import React from 'react';
import { BarChart } from '@mui/x-charts/BarChart';
import { LineChart } from '@mui/x-charts/LineChart';
import { ScatterChart } from '@mui/x-charts/ScatterChart';
import { PieChart } from '@mui/x-charts/PieChart';


const HTMLCircle = ({ className, color }) => {
  return (
    <div className={className} style={{ borderRadius: '100%', background: color }} />
  );
};

const PlotComponent = ({dbName , tableName , data, xcol, ycol, xname, yname, binSize, plotType, name}) => {  
  const arr1 = data.xdata;
  let values = arr1.map(obj => Object.values(obj)[0]); 
  const arr2 = data.ydata;
  let yvalues = [];
  const columnNames = Object.keys(arr2[0]);
  columnNames.forEach((colName, idx) => {
    const columnData = arr2.map(row => Number(row[colName]));
    yvalues.push({
      id: idx,
      data: columnData,
      label: colName,
      labelMarkType: HTMLCircle,
    });
  });

  if (plotType === 'bar-chart') {
    return (
      <BarChart
        xAxis={[{ scaleType: 'band', data: values , label: xname }]}
        yAxis={[{ label: yname }]}
        series={yvalues}
        height={300}
      />
    );
  } else if (plotType === 'line-chart') {
    return (
      <LineChart
        xAxis={[{ scaleType: 'point', data: values , label: xname}]}
        series={yvalues}
        height={300}
        yAxis={[{ width: 50 , label: yname }]}
        margin={{ right: 24 }}
      />
    );
  } else if (plotType === 'scatter-plot') {
    return (
      <ScatterChart
        height={300}
        xAxis={[{ label: xname }]}
        yAxis={[{ label: yname }]}
        series={yvalues.map((y) => ({
          label: y.label,
          data: values.map((xVal, i) => ({
            x: xVal,
            y: y.data[i],
            id: `${y.id}-${i}`,
          })),
        }))}
      />
    );
  } 
  else if (plotType === 'pie-chart') {
    const columnNames = Object.keys(arr2[0]);
    const mainCol = columnNames[0];
    const groupCounts = {};
    for (let i = 0; i < arr2.length; i++) {
      const label = arr2[i][mainCol];
      if (groupCounts[label]) {
        groupCounts[label] += 1;
      } else {
        groupCounts[label] = 1;
      }
    }
    const pieData = Object.entries(groupCounts).map(([label, value], idx) => ({
      id: idx,
      value,
      label,
    }));
    return (
      <PieChart
        series={[{ data: pieData }]}
        width={300}
        height={300}
      />
    );
  }
  else if (plotType === 'pivot-table') {
    let aggCol = arr1.map(obj => Object.values(obj)[0]);
    const columnNames = Object.keys(arr2[0]);
    const firstPivotCol= arr2.map(row => row[columnNames[0]]);
    const secondPivotCol = arr2.map(row => row[columnNames[1]]);
    const uniqueRows = [...new Set(firstPivotCol)];
    const uniqueCols = [...new Set(secondPivotCol)];

    const pivotData = {};
    uniqueRows.forEach(row => {
      pivotData[row] = {};
      uniqueCols.forEach(col => {
        pivotData[row][col] = 0;
      });
    });

    for (let i = 0; i < aggCol.length; i++) {
      const rowKey = firstPivotCol[i];
      const colKey = secondPivotCol[i];
      pivotData[rowKey][colKey] += aggCol[i];
    }

    return (
      <table border="1" cellPadding="8" style={{ borderCollapse: 'collapse', marginTop: '20px' }}>
        <thead>
          <tr>
          <th>{columnNames[0]} \\ {columnNames[1]}</th>
            {uniqueCols.map(col => (
              <th key={col}>{col}</th>
            ))}
          </tr>
        </thead>
        <tbody>
          {uniqueRows.map(row => (
            <tr key={row}>
              <td><b>{row}</b></td>
              {uniqueCols.map(col => (
                <td key={col}>{pivotData[row][col]}</td>
              ))}
            </tr>
          ))}
        </tbody>
      </table>
    );
    
  }  
  

};

export default PlotComponent;
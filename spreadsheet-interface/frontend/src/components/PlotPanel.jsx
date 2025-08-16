import { useState } from "react";
import { apiUrl } from "../config/config";
import PlotComponent from "./PlotCOmponent";
import { Rnd } from "react-rnd";


const PlotPanel = ({ databases, tables}) => {
  const [database, setDatabase] = useState("");
  const [table, setTable] = useState("");
  const [xcol, setXcol] = useState("");
  const [ycol, setYcol] = useState([]);
  const [xname, setXname] = useState("");
  const [yname, setYname] = useState("");
  const [plotType, setPlotType] = useState("");
  const [plotName, setPlotName] = useState("");
  const [binSize , setBinSize] = useState(0);
  const [message, setMessage] = useState("");
  const [messageColor, setMessageColor] = useState(""); 
  const [plots, setPlots] = useState([]); 

  const handlePlotTypeChange = (event) => {
    setPlotType(event.target.value);
    setXcol("");
    setYcol([]);
  };

  const handlePlotGenerate = async () => {
    const name = `${database}-${table}-${xcol}-${ycol.join("-")}-${plotType}`;
    setPlotName(name);
    console.log(name);
    try {
      const res = await fetch(`${apiUrl}/get-plot-data`, {
        method: "POST",
        headers: { "Content-Type": "application/json"},
        body: JSON.stringify({
          dbName: database,
          tableName: table,
          xcol : xcol,
          ycols: ycol
        })
      });
      const data = await res.json();
      if(res.status === 200){
        setMessage("Plot data fetched");
        setMessageColor("green");
        const newPlot = {
          data : data,
          xcol : xcol,
          ycol : ycol,
          xname : xname,
          yname : yname,
          binSize : binSize,
          plotType : plotType,
          database : database,
          table : table,
          name: plotName,
        };
        setPlots((prevPlots) => [...prevPlots, newPlot]);
      }
      else{
        setMessage(data.message);
        setMessageColor("red");
      }
    } 
    catch (err) {
      setMessage(err);
      setMessageColor("red");
    }
  };

  return (
      <div className="rnd-wrapper">
        <div className="rnd-header">
          <span>Plot Panel</span>
        </div>
        <div className="rnd-body">
          {/* Database Selector */}
          <div>
            <label>Select Database:</label>
            <select
              value={database}
              onChange={(e) => {
                setDatabase(e.target.value);
                setTable("");
              }}
            >
              <option value="">-- Select Database --</option>
              {databases.map((db, index) => (
                <option key={index} value={db}>
                  {db}
                </option>
              ))}
            </select>
          </div>

          {/* Table Selector */}
          <div>
            <label>Select Table:</label>
            <select
              value={table}
              onChange={(e) => setTable(e.target.value)}
              disabled={!database}
            >
              <option value="">-- Select Table --</option>
              {(tables).map((tbl, index) => (
                <option key={index} value={tbl}>
                  {tbl}
                </option>
              ))}
            </select>
          </div>

          {/* Plot Type Selector */}
          <div>
            <label>Select Plot Type:</label>
            <select onChange={handlePlotTypeChange} value={plotType}>
              <option value="">Select Plot Type</option>
              <option value="line-chart">Line Chart</option>
              <option value="bar-chart">Bar Chart</option>
              <option value="pie-chart">Pie Chart</option>
              <option value="scatter-plot">Scatter Plot</option>
              <option value="pivot-table">Pivot Table</option>
            </select>
          </div>

          {/* Plot-Specific Inputs */}
          {["line-chart", "bar-chart", "scatter-plot"].includes(plotType) ? (
            <>
              <div>
                <label>Enter X-Column:</label>
                <input type="text" value={xcol} onChange={(e) => setXcol(e.target.value)} />
              </div>
              <div>
                <label>Enter Y-Columns (comma separated):</label>
                <input
                  type="text"
                  value={ycol.join(",")}
                  onChange={(e) => setYcol(e.target.value.split(","))}
                />
              </div>
              <div>
                <label>X-Axis Label:</label>
                <input type="text" value={xname} onChange={(e) => setXname(e.target.value)} />
              </div>
              <div>
                <label>Y-Axis Label:</label>
                <input type="text" value={yname} onChange={(e) => setYname(e.target.value)} />
              </div>
            </>
          ) : plotType === "pie-chart" ? (
            <>
              <div>
                <label>Enter Column:</label>
                <input
                  type="text"
                  value={ycol.join(",")}
                  onChange={(e) => setYcol(e.target.value.split(","))}
                />
              </div>
            </>
          ) : plotType === "pivot-table" ? (
            <>
              <div>
                <label>Aggregate Column:</label>
                <input type="text" value={xcol} onChange={(e) => setXcol(e.target.value)} />
              </div>
              <div>
                <label>Enter Pivot Columns:</label>
                <input
                  type="text"
                  value={ycol.join(",")}
                  onChange={(e) => setYcol(e.target.value.split(","))}
                />
              </div>
            </>
          ) : null}
          <br />
          <div>
            <button onClick={handlePlotGenerate}>Create Plot</button>
          </div>
        </div>
        {plots.map((plot, index) => (
        <Rnd
          key={index}
          bounds="window" 
          default={{
            x: 50 + index * 30,
            y: 200 + index * 30,
            width: 600,
            height: 400,
          }}
          enableResizing={{
            top: false, right: false, bottom: false, left: false,
            topRight: false, bottomRight: false, bottomLeft: false, topLeft: false,
          }}
        >
          <div className="rnd-wrapper">
          <button
              className="close-btn-plot"
              onClick={() => {
                setPlots((prev) => prev.filter((_, i) => i !== index));
              }}
            >
              X
            </button>
            <PlotComponent
              data={plot.data}
              xcol={plot.xcol}
              ycol={plot.ycol}
              xname={plot.xname}
              yname={plot.yname}
              binSize={plot.binSize}
              plotType={plot.plotType}
              name={plot.name}
            />
          </div>
        </Rnd>
      ))}
      </div>
  );
};

export default PlotPanel;

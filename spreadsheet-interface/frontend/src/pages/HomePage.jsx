import React, { useState, useEffect } from "react";
import FileUpload from "../components/FileUpload";
import TableDropdown from "../components/TableDropDown";
import RunQuery from "../components/RunQuery";
import DatabasesView from "../components/DatabsesView";
import { apiUrl } from "../config/config";
import Sheet from "../components/Sheet";
import AddDatabase from "../components/AddDatabase";
import PlotPanel from "../components/PlotPanel";
import PlotComponent from "../components/PlotCOmponent";
import { Rnd } from "react-rnd";
import "../styles/HomePage.css";

const HomePage = () => {
  const [tables, setTables] = useState([]);
  const [databases, setDatabases] = useState([]);
  const [openTables, setOpenTables] = useState({});
  const [selectedDatabase, setSelectedDatabase] = useState("default");

//Fetch databases from API
const fetchDatabases = async () => {
      try {
        const res = await fetch(`${apiUrl}/get-databases`, {
          method: "GET",
          credentials: "include",
        });
        const data = await res.json();
        if(res.ok){
          setDatabases(data.databases);
        }
      } catch (err) {
        console.error("Failed to fetch databases", err);
      }
};

  // Fetch tables from API
  const fetchTables = async () => {
      try {
        let dbName = selectedDatabase;
        const res = await fetch(`${apiUrl}/get-tables`, {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ dbName }),  
          credentials: "include",
        });
        const data = await res.json();
        if (res.ok) {
          setTables(data.tables);
        }
      } catch (err) {
        console.error("Error fetching tables:", err);
      }
  };

  useEffect(() => {
    fetchDatabases();
    fetchTables();
  }, []);


  const handleTableSelect = (tableName) => {
    setOpenTables((prev) => {
      if (prev[tableName]) return prev;
      return { ...prev, [tableName]: true };
    });
  };

  const handleDatabaseSelect = (databaseName) => {
     setSelectedDatabase(databaseName);
  }

  const handleClose = (tableName) => {
    setOpenTables((prev) => {
      const updated = { ...prev };
      delete updated[tableName];
      return updated;
    });
  };



  return (
    <>
    <div className="homepage">
      {/* Top Section - Upload, Table Select, Query */}
      <div className="top-container">
        <div className="file-upload">
          <h2>File Upload</h2>
          <FileUpload 
            dbName = {selectedDatabase}
          />
        </div>
        <div className="database-view">
          <h2>Select Database</h2>
          <DatabasesView
             databases = {databases}
             fetchDatabases={fetchDatabases}
             onDatabaseSelect={handleDatabaseSelect}
          />
          <h2>Add Database</h2>
          <AddDatabase fetchDatabases={fetchDatabases} />
        </div>
        <div className="show-tables">
          <h2>Select Table</h2>
          <TableDropdown
            tables={tables}
            fetchTables={fetchTables}
            onTableSelect={handleTableSelect}
          />
        </div>
        <div className="query-section">
          <h2>Run Query</h2>
          <RunQuery 
            dbName = {selectedDatabase}
            fetchTables={fetchTables} />
        </div>
        <div className="plot-section">
           <h2>Plot Wizard</h2>
           <PlotPanel
              databases={databases}
              tables={tables}
           />
        </div>
      </div>
    
      {Object.keys(openTables).map((tableName, index) => (
        <Rnd
          key={tableName}
          default={{
            x: 50 + index * 30,
            y: 200 + index * 30,
            height: 500,
          }}
          enableResizing={{
            top: false, right: false, bottom: false, left: false,
            topRight: false, bottomRight: false, bottomLeft: false, topLeft: false
          }}
          bounds="window"
          dragHandleClassName="rnd-header"
        >
          <div className="rnd-wrapper">
            <div className="rnd-header">
              <span className="table-title">{tableName}</span>
              <button className="close-btn" onClick={() => handleClose(tableName)}>×</button>
            </div>
            <div className="rnd-body scrollable-sheet">
              <Sheet 
                dbName = {selectedDatabase}
                tableName={tableName} />
            </div>
          </div>
        </Rnd>
      ))}
    </div>
    
    </>

  );
};

export default HomePage;

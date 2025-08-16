import React, { useState } from "react";
import { apiUrl } from "../config/config";

const AddDatabase = ({ fetchDatabases }) => {
  const [dbName, setDbName] = useState("");
  const [message, setMessage] = useState("");
  const [messageColor, setMessageColor] = useState("");

  const handleAddDatabase = async () => {
    if (!dbName.trim()) {
      setMessage("Database name cannot be empty.");
      setMessageColor("red");
      return;
    }
    try {
      const res = await fetch(`${apiUrl}/add-database`, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ dbName }),
        credentials: "include",
      });

      const data = await res.json();

      if (res.status === 200) {
        setMessage(data.message);
        setMessageColor("green");
        setDbName(""); 
        fetchDatabases(); 
      } else {
        setMessage(data.message);
        setMessageColor("red");
      }
    } catch (err) {
      setMessage(err);
      setMessageColor("red");
    }
  };

  return (
    <div className="add-database">
      <input
        type="text"
        value={dbName}
        onChange={(e) => setDbName(e.target.value)}
        placeholder="Enter database name"
      />
      <br/><br/>
      <button onClick={handleAddDatabase}>Add</button>
      {message && <p style={{ color: messageColor }}>{message}</p>}
    </div>
  );
};

export default AddDatabase;

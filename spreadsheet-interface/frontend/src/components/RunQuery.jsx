import React, { useState } from "react";
import { apiUrl } from "../config/config";

const RunQuery = ({dbName , fetchTables }) => {
  const [sql, setSql] = useState("");
  const [message, setMessage] = useState("");
  const [messageColor, setMessageColor] = useState("");

  const handleChange = (e) => {
    setSql(e.target.value);
    setMessage("");
  };

  const handleSubmit = async (e) => {
    e.preventDefault();

    if (!sql.trim()) {
      setMessage("Please enter an SQL query.");
      setMessageColor("red");
      return;
    }

    try {
      const res = await fetch(`${apiUrl}/run-query`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ dbName , sql }),
        credentials: "include",
      });

      const data = await res.json();

      if (res.ok) {
        setMessage(data.message);
        setMessageColor("green");
        fetchTables(); 
      } else {
        setMessage(data.message);
        setMessageColor("red");
      }
    } catch (err) {
      setMessage(err.message);
      setMessageColor("red");
    }
  };

  return (
    <div>
      {message && <p style={{ color: messageColor }}>{message}</p>}
      <form onSubmit={handleSubmit}>
        <textarea rows="3" cols="30" placeholder="Enter your SQL query here..." value={sql} onChange={handleChange} required />
        <br /><br />
        <button type="submit">Execute</button>
      </form>
    </div>
  );
};

export default RunQuery;

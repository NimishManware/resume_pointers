import React, { useState } from "react";
import { apiUrl } from "../config/config";

const FileUpload = ({dbName}) => {
  const [file, setFile] = useState(null);
  const [message, setMessage] = useState("");
  const [messageColor, setMessageColor] = useState(""); 

  const handleFileChange = (e) => {
    setFile(e.target.files[0]);
    setMessage(""); 
  };


  // Handle file upload
  const handleUpload = async (e) => {
    e.preventDefault();
    if (!file) {
      setMessage("Please select a file first");
      setMessageColor("red");
      return;
    }

    const formData = new FormData();
    formData.append("sqlFile", file);
    formData.append("dbName" , dbName);

    try {
      const res = await fetch(`${apiUrl}/load-file`, {
        method: "POST",
        body: formData,
        credentials: "include",
      });

      const data = await res.json();
      if (res.status === 200) {
        setMessage(data.message);
        setMessageColor("green");
      } else {
        setMessage(data.message);
        setMessageColor("red");
      }
    } catch (error) {
      setMessage(error);
      setMessageColor("red");
    }
  };

  return (
    <div>
      {message && <p style={{ color: messageColor }}>{message}</p>}
      <form onSubmit={handleUpload}>
        <input type="file" onChange={handleFileChange} accept=".sql" /><br/><br/>
        <button type="submit">Upload</button>
      </form>
    </div>
  );
};

export default FileUpload;

import React, { useState } from "react";
import { apiUrl } from "../config/config";

const DatabasesView = ({databases , fetchDatabases , onDatabaseSelect}) => {
  return (
    <div>
      <button onClick={fetchDatabases}>Fetch Databases</button>
      <select onChange={(e) => {onDatabaseSelect(e.target.value)}}>
        {databases.map((database) => (
          <option key={database} value={database}>
            {database}
          </option>
        ))}
      </select>
      
    </div>
  );
};

export default DatabasesView;

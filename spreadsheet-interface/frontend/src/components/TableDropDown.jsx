import React from "react";
const TableDropdown = ({ tables, fetchTables, onTableSelect }) => {
  return (
    <div>
      <button onClick={fetchTables}>Fetch Tables</button>
      <select onChange={(e) => onTableSelect(e.target.value)}>
        <option value="">Select Table</option>
        {tables.map((table) => (
          <option key={table} value={table}>
            {table}
          </option>
        ))}
      </select>
    </div>
  );
};

export default TableDropdown;

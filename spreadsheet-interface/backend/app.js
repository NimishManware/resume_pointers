const express = require("express");
const bodyParser = require("body-parser");
const cors = require("cors");
const fileUpload = require("express-fileupload");
const { PGlite } = require("@electric-sql/pglite");

const app = express();
const port = 4000;

app.use(
    cors({
        origin: "http://localhost:3000",
        credentials: true,
    })
);

app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.json());
app.use(fileUpload());


//dbMap
const dbMap = {};
dbMap["default"] = new PGlite(); 

/* WRITE BACKEND LOGIC */

/*loads sql files*/
app.post('/load-file', async (req, res) => {
  if (!req.files || !req.files.sqlFile) {
    return res.status(400).json({ message: "No file uploaded" });
  }
  const sqlFile = req.files.sqlFile;
  const sqlQuery = sqlFile.data.toString();
  const dbName = req.body.dbName;
  const db = dbMap[dbName];
  try {
    await db.exec(sqlQuery);
    res.status(200).json({ message: "SQL file executed successfully" });
  } catch (error) {
    console.error(error);
    return res.status(500).json({ message: "Error loading file" });
  }
});


/*run a query and save in temp table*/
app.post('/run-query', async (req, res) => {
  const { dbName, sql } = req.body;
  let db = dbMap[dbName];
  try {
    if (sql.trim().toUpperCase().startsWith('SELECT')) {
      await db.query(`DROP TABLE IF EXISTS temp`);
      await db.query(`CREATE TABLE temp AS ${sql}`);
      res.status(200).json({ message: 'Result saved in temp table' });
    } else {
      await db.exec(sql);
      res.status(200).json({ message: 'Query executed successfully' });
    }
  } catch (err) {
    res.status(400).json({ message: 'Error running query', error: err.message });
  }
});


/*get table names*/
app.post("/get-tables", async (req, res) => {
    const { dbName } = req.body;
    let db = dbMap[dbName];
    try {
      const result = await db.query(
        `SELECT tablename FROM pg_tables WHERE schemaname = 'public'`
      );
      res.status(200).json({ tables: result.rows.map((row) => row.tablename) });
    } catch (error) {
      res.status(500).json({ message: "Error fetching tables" });
    }
});

/*fetch table entries*/
app.post("/get-table-data", async (req, res) => {
  const { dbName , tableName } = req.body;
  let db = dbMap[dbName];
  try {
      // Fetch table data
      const result = await db.query(`SELECT * FROM ${tableName}`);

      // Fetch primary key column
      const pkeyQuery = `
          SELECT kcu.column_name
          FROM information_schema.table_constraints tc
          JOIN information_schema.key_column_usage kcu
          ON tc.constraint_name = kcu.constraint_name
          WHERE tc.table_schema = 'public'
          AND tc.table_name = $1
          AND tc.constraint_type = 'PRIMARY KEY';
      `;
      
      //not null column
      const notNullQuery = `
        SELECT column_name
        FROM information_schema.columns
        WHERE table_schema = 'public'
            AND table_name = $1
            AND is_nullable = 'NO';
      `;

      //Fetch column name
      const colQuery = `
          SELECT column_name, data_type
          FROM information_schema.columns
          WHERE table_name = $1;
      `

      const res2 = await db.query(pkeyQuery, [tableName]); 
      const res3 = await db.query(colQuery , [tableName]);
      const res4 = await db.query(notNullQuery , [tableName]);

      const primaryKeyCols = res2.rows.map(row => row.column_name);
      const columns = res3.rows.map(row => row.column_name);
      const notNullColumns = res4.rows.map(row => row.column_name);

      const modifiedRows = result.rows.map(row => ({
          ...row,
          primarykeyvals: primaryKeyCols.map(col => row[col]) 
      }));

      res.status(200).json({ 
          data: modifiedRows, 
          primarykeycols: primaryKeyCols,
          columns : columns,
          notNullColumns : notNullColumns
      });

  } catch (error) {
      console.error("Error fetching table data:", error);
      res.status(500).json({ message: "Error fetching table data" });
  }
});


/*handle change*/
app.post("/handle-cell-change", async (req, res) => {
    const {dbName , col, newData, primaryKeyVals, primaryKeyCols, tableName } = req.body;
    let db = dbMap[dbName];
    const whereClauses = primaryKeyCols
        .map((colName, idx) => `${colName} = $${idx + 1}`)
        .join(" AND ");

    const selectQuery = `SELECT * FROM ${tableName} WHERE ${whereClauses}`;
    try {
        const result = await db.query(selectQuery, primaryKeyVals);
        if (result.rows.length === 0) {
            return res.status(400).json({ message: "Invalid row - primary key not found." });
        }
        const updateQuery = `UPDATE ${tableName} SET ${col} = $${primaryKeyVals.length + 1} WHERE ${whereClauses}`;
        await db.query(updateQuery, [...primaryKeyVals, newData]);
        res.status(200).json({ message: "Cell updated successfully" });
    } catch (err) {
        res.status(500).json({ message: "Error updating cell check schema once" });
    }
});

/*add row*/
app.post("/add-row" , async (req,res) => {
    const {dbName , tableName , rowData} = req.body;
    let db = dbMap[dbName];
    if (!tableName || !rowData || typeof rowData !== "object") {
        return res.status(400).json({ message: "Invalid request body" });
    }
    const columns = Object.keys(rowData);
    const values = Object.values(rowData).map((val) => {
        if (val === "" || val.toUpperCase?.() === "NULL") return null;
        return val;
    });
    if(columns.length === 0) {
        return res.status(400).json({ message: "No columns provided" });
    }
    const place = columns.map((_, idx) => `$${idx + 1}`).join(", ");
    const insertQuery = `INSERT INTO ${tableName} (${columns.join(
      ", "
    )}) VALUES (${place})`;
  
    try {
      await db.query(insertQuery, values);
      res.status(200).json({ message: "Row added successfully" });
    } 
    catch (err) {
      res.status(500).json({ message: "Failed to add row check schema once" });
    }
})

//add database
app.post("/add-database", async (req, res) => {
    const { dbName } = req.body;
    if (!dbName || typeof dbName !== "string") {
      return res.status(400).json({ message: "Invalid database name" });
    }
    if (dbMap[dbName]) {
      return res.status(400).json({ message: "Database already exists" });
    }
    try {
      dbMap[dbName] = new PGlite(); 
      res.status(200).json({ message: `Database '${dbName}' created` });
    } 
    catch (err) {
      res.status(500).json({ message: "Failed to create database" });
    }
});

//fetch all databases names
app.get("/get-databases", (req, res) => {
  try {
      const databaseNames = Object.keys(dbMap);
      res.status(200).json({ databases: databaseNames });
  } catch (err) {
      res.status(500).json({ message: "Failed to fetch databases" });
  }
});

//plot data
app.post('/get-plot-data', async (req, res) => {
  const { dbName, tableName, xcol, ycols } = req.body;
  let db = dbMap[dbName];
  try {
    const ycolSelect = ycols.map(col => `${col}`).join(", ");
    const query1 = `SELECT ${xcol} FROM ${tableName}`;
    const query2 = `SELECT ${ycolSelect} FROM ${tableName}`;
    const result1 = await db.query(query1);
    const result2 = await db.query(query2);
    res.status(200).json({ xdata: result1.rows , ydata : result2.rows});
  } 
  catch (err){
    res.status(500).json({ message: "Failed to fetch plot data"});
  }
});

// Start server
app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});

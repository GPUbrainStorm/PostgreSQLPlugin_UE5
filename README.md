# **PostgreSQL Integration Plugin for Unreal Engine 5**

A lightweight plugin that enables **direct PostgreSQL database access** inside Unreal Engine, supporting both **C++** and **Blueprints**.
This is a **BETA** version, proceed with caution.
For support, join the Discord server:
**[https://discord.gg/BxHRHDdkNy](https://discord.gg/BxHRHDdkNy)**

---

## **Features**

* **Direct PostgreSQL Connectivity**
  Connect Unreal Engine directly to any PostgreSQL server (local or remote).

* **SSL Support**
  Optional SSL connection with selectable SSL modes and optional root certificate use.

* **OS Support**
  Currently supports **Windows only**.
  (Tested on Windows 10 with UE5.2)

* **Query Execution**
  Run SQL commands, retrieve results, and map rows/columns easily.

* **Blueprint-Friendly**
  All major operations exposed as simple Blueprint nodes.

* **CSV + Structured Results**
  Query results can be returned both as CSV rows and structured maps.

---

## **Available Functions**

### **PG_Connect(...)**

Opens a connection to a PostgreSQL database with optional SSL settings.

### **PG_Disconnect(Connection)**

Closes an active PostgreSQL connection.

### **PG_IsConnected(Connection)**

Returns whether the connection is valid and open.

### **PG_ExecuteNonQuery(Connection, Sql, OutError)**

Executes SQL statements that do **not** return rows
(e.g., INSERT, UPDATE, DELETE, CREATE, DROP).

### **PG_ExecuteQuery(Connection, Sql, PrimaryKeyColumn, OutRowsByPK, OutRowsCSV, OutError)**

Runs a SELECT query and returns data in two formats:

* A map of rows keyed by a primary column
* An array of CSV-formatted rows

---

## **Compatibility**

### **Precompiled Binaries**

* Unreal Engine **5.2**

### **Source Code**

Should compile normally for other Unreal Engine versions.

Need precompiled binaries for another version?
Open an issue or ask on Discord.

---

## **Installation**

Download the correct version from Releases:
**[https://github.com/GPUbrainStorm/PostgreSQLPlugin_UE5/releases](https://github.com/GPUbrainStorm/PostgreSQLPlugin_UE5/releases)**

### **Precompiled (UE 5.2)**

Place the plugin into your project's **Plugins/** folder and launch normally.

### **From Source Code (Any Version)**

Copy the plugin to your project's **Plugins/** directory and build normally.

---

## **Usage Example**

<img width="1119" height="407" alt="image" src="https://github.com/user-attachments/assets/189f1bc8-3896-4b0c-a122-bc75f996a0d9" />  
<img width="1078" height="393" alt="image" src="https://github.com/user-attachments/assets/5170e131-1944-446c-9b08-fbcdfd1c086c" />  
<img width="1836" height="671" alt="image" src="https://github.com/user-attachments/assets/0c0e9957-d9d8-44a6-acf4-3abc5afc1cdc" />

# KPI-data-base-labs
Laboratory works from Data Bases in KPI
![ER diagram](https://github.com/SkaLe3/KPI_DataBase_Labs/blob/28a54466825e473627a322b78ef2084d8fe1a28c/ER2.png)
## Report 1 link
https://docs.google.com/document/d/19GQfMVTitYW0qXv-Y7oT_Ir1FnSwX1jy5lLQOoWN3fE/edit
## Report 2 link
https://docs.google.com/document/d/1-5DNgfjI9YFzf9TkHMiKzHig48rSyPWDKDMv0Y8PabQ/edit?usp=sharing

# How to use:
## Build from Source
1. Downloading the repository:
Start by cloning the repository with git clone --recursive https://github.com/SkaLe3/KPI_DataBase_Labs

2. Generating solution:
After cloning run Setup.bat script file to generate a Visual Studio 2022 solution file

3. Preparing:
Compile the project and move PostgreSQL dlls (or dlls from dll folder) to the executable file by path MusCat\bin\Debug-windows-x86_64\MusCat or MusCat\bin\Release-windows-x86_64\MusCat
## Compiled binaries
1. Download archive from releases
2. Replace dlls from PostgreSQL directory if needed
    
## Dependencies:
* PostgresSQL v15: <br>
     Add to your System Environment Variables:
    * POSTGRESQL_INCLUDE with path to PostgreSQL\15\include folder
    * POSTGRESQL_LIB with path to PostgresSQL\15\lib folder
* All Dependencies of [ TinyORM ](https://github.com/silverqx/TinyORM) <br>
* All remaining dependencies come with the repository

# Possible improvements
* Transfer the query execution to using concurrency
* Create generic functions for duplicated code
* Unify variable names
* Refactor some ideas of data replication between classes
* Rewrite some SQL queries

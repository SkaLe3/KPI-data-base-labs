# KPI_DataBase_Labs
Laboratory works from Data Bases in KPI

## Report 1 link
https://docs.google.com/document/d/19GQfMVTitYW0qXv-Y7oT_Ir1FnSwX1jy5lLQOoWN3fE/edit
## Report 2 link
https://docs.google.com/document/d/1-5DNgfjI9YFzf9TkHMiKzHig48rSyPWDKDMv0Y8PabQ/edit?usp=sharing

# How to use:
## Build
1. Downloading the repository:
Start by cloning the repository with git clone --recursive https://github.com/SkaLe3/KPI_DataBase_Labs

2. Generating solution:
After cloning run Setup.bat script file to generate a Visual Studio 2022 solution file

3. Preparing:
Compile project and move files from dll folder to .exe file by path MusCat\bin\Debug-windows-x86_64\MusCat or MusCat\bin\Release-windows-x86_64\MusCat
## Use
1. Download archive from releases
2. Replace dll's from PostgreSQL directory if needed
    
## Requirements:
* Installed PostgresSQL v15
* ## Add to your System Environment Variables:
* POSTGRESQL_INCLUDE with path to PostgreSQL\15\include folder
* POSTGRESQL_LIB with path to PostgresSQL\15\lib folder

# Possible improvements
* Transfer the queries execution to using concurrency
* Create generic functions for duplicated code
* Unify variable names
* Refactor some ideas of data replication between classes

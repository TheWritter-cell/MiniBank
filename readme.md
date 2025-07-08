# DataManager Project

## Description

This project is a basic data manager written in C++ using an integrated SQLite database.  
It allows creating user and admin tables, adding users with unique IDs, and managing passwords through a Python script called from C++.

## Current Features

- Dynamic creation of SQLite tables (`ADMIN`, `USER`)
- Generation of unique hexadecimal IDs (UUID-style)
- Adding users with name, password (hashed via Python), and balance
- Calling an external Python script for password hashing and verification
- Basic error handling with assertions and exceptions

## Project Structure



## Prerequisites

- C++20 compatible compiler (e.g., g++ 13+ under MSYS2)
- SQLiteCpp library for C++
- Python 3 installed and accessible in your terminal (make sure Python is in PATH)
- Standard C++ libraries (`<array>`, `<filesystem>`, etc.)

## Compilation

```bash
g++ datamanager.cpp -o db -std=c++20 \
  -I"/c/Program Files (x86)/SQLiteCpp/include" \
  -L"/c/Program Files (x86)/SQLiteCpp/lib" \
  -lSQLiteCpp -lsqlite3

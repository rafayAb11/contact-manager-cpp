# Contact Manager (C++)

Simple console contact manager to create, update, delete, and search contacts using a plain text file for storage.

## Features
- Multi-word names supported (e.g. "Ali Raza")
- Email validation (`...@....com`)
- Phone number validation (minimum 6 digits, can include +, -, spaces)
- Data stored in `contact.txt`

## Build & Run
```bash
# Compile
g++ -std=c++17 contact-manager.cpp -o contacts

# Run
./contacts   # or contacts.exe on Windows
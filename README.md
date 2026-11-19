# Offline Library Manager (C++17)

A menu-driven, offline command-line library manager for fictional book records. It supports CSV import/export, searching, updates, borrowing/returning, statistics, and local persistence.

## Features
- Add, list, search (ID/title/author/category), update and delete books.
- Borrow and return books with validated `YYYY-MM-DD` dates.
- CSV import/export with quoted-field handling and duplicate-ID rejection.
- Automatic save after mutations and load on startup.
- Malformed CSV rows are skipped with diagnostics rather than terminating the application.
- Automated tests for empty data, duplicates, missing files, malformed rows, quoted fields, date boundaries and persistence round trips.

## Architecture
- `Book`: plain record type; it owns its strings by value.
- `Library`: business rules and in-memory collection. `std::vector<Book>` owns records; `std::unordered_map<string, size_t>` is a non-owning index into the vector and is rebuilt after erasure/replacement.
- `CSVManager`: file parsing and persistence. File streams use RAII and close automatically.
- `InputValidator`: reusable validation and normalization helpers.
- `main`: terminal prompts and presentation only.

No raw owning pointers or manual resource lifetime management are used. Standard containers and streams provide deterministic RAII ownership.

## Project layout
```text
LibraryManager/
├── CMakeLists.txt
├── README.md
├── data/books.csv
├── include/{Book.h,Library.h,CSVManager.h,InputValidator.h}
├── src/{main.cpp,Library.cpp,CSVManager.cpp,InputValidator.cpp}
├── tests/{test_main.cpp,test_library.cpp,test_csv.cpp}
└── screenshots/README.md
```

## Build
Requires a C++17 compiler and CMake 3.16+.

```bash
cmake -S . -B build
cmake --build build --config Release
```

Run on Windows (single-config MinGW):
```powershell
.\build\library_manager.exe
```
For Visual Studio multi-config generators:
```powershell
.\build\Release\library_manager.exe
```
Run on Linux/macOS:
```bash
./build/library_manager
```

## Tests
```bash
ctest --test-dir build --output-on-failure
```
Or run `library_tests` directly from the build output directory. Tests use a temporary CSV file in the working directory and remove it after completion.

## CSV format
Header:
```csv
id,title,author,category,publicationYear,borrowed,borrower,borrowDate,returnDate
```
Text fields are quoted on export; embedded quotes are doubled. `borrowed` is `0` or `1`. Dates use `YYYY-MM-DD`. A missing CSV at startup is reported and the application starts with an empty collection; the first save creates the data file.

## Validation and limitations
- IDs, titles, authors and categories must be non-empty; IDs must be unique.
- Publication year is an integer from 0 through 9999 (0 may be used when unknown).
- Gregorian calendar dates are validated, including leap years. Return date cannot precede borrow date.
- Import is additive: existing IDs and duplicate IDs in the imported file are skipped; valid rows are retained even if other rows are malformed.
- Current storage uses one active borrow per book and does not maintain a historical transaction ledger.
- CSV is line-oriented; quoted commas and escaped quotes are supported. Multiline quoted CSV fields are not supported.

## Complexity note
Let `n` be the number of stored books and `k` the number of matching results.
- Add by ID: O(1) average for duplicate check/index insertion, plus amortized vector append.
- Find by ID: O(1) average using the hash index.
- Search title/author/category: O(n) comparisons; output storage O(k).
- List: O(n).
- Category statistics: O(n) expected time with hashing.
- Delete: O(n) due to vector element shifting and index rebuild.
- CSV load/save: O(n) expected time, excluding string parsing and disk I/O costs.

## Example workflow
1. Start the program and choose `2` to list the sample books.
2. Choose `6` to borrow a book using a valid borrower name and date.
3. Choose `7` to return it with a valid return date.
4. Choose `10` to view collection statistics.
5. Choose `0` to save and exit.

## GitHub submission
Create a public repository and upload this folder. Do not commit the `build/` directory or generated executables. Add terminal screenshots to `screenshots/` before submitting the repository link.

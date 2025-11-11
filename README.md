# 🗑️ Smart Bin Tracker System

This is a comprehensive Console Application written in **C** for managing smart waste bins, focusing on tracking fill levels, location, and status using bitwise flags.

---

## ✨ Features

* **Bin Management:** Add, Update (fill level, location, flags), and Delete bins from the main array database.
* **Status Tracking:** Uses **bitwise flags** (`FLAG_ACTIVE`, `FLAG_FULL`, `FLAG_NEEDS_CLEAN`) for efficient status storage and manipulation.
* **Search & Alerts:** Search by type or location, and generate alerts for bins exceeding a specified fill level threshold.
* **Sorting:** Sort bins based on **Fill Level** (descending) and **Type** (alphabetical).
* **Zone Merging:** Demonstrates merging a secondary set of bins (`zoneBins`) into the main list, checking for type duplicates.
* **Utility Demos:** Includes demonstrations of `swap` logic without a third variable and general bitwise operations.

---

## 🛠️ Usage

### Compilation

Compile the C source file (e.g., `main.c`) using a standard GCC command:

```bash
gcc -o bin_tracker main.c

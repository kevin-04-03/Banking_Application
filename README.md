File-Based Banking System in C
A lightweight Command-Line Interface (CLI) application that simulates a relational banking system using local flat-file storage. The program acts as a minimal database engine where each bank account is managed dynamically as an individual text file, featuring data verification loops and transactional fee rules.

Features
Local Flat-File Database Setup: Automatically provisions a local database/ directory at launch to handle state persistence.

Account Provisioning & Unique Tracking: Generates a randomized, verified-unique 5-to-6-digit account number (10000–1000000) for each user.

Filesystem Constraints & Safety Checks:

Prevents user actions if structural thresholds aren't met (e.g., blocks remittance if fewer than two accounts exist; blocks deposits/withdrawals/deletions if the directory is empty).

Clears stray input buffers via custom clearBuffer() routines to eliminate data contamination during menu navigations.

Cross-Account Remittance Engine: Supports fund transfers between discrete accounts with a built-in cross-account fee assessment matrix:

Savings -> Current: Charges a 2% processing fee on the principal transfer amount.

Current -> Savings: Charges a 3% processing fee on the principal transfer amount.

Same-Type Transfers: Charges a 0% fee.

Data Storage Format
Every profile is saved under database/[AccountNumber].txt. The data schema matches the layout below:


Name: John_Doe
ID: MY987654321
Account Type: Savings
Account Number: 357421
Balance: 1250.50
Environment & Compilation Requirements
Operating System Compatibility
This code contains a combination of Windows Win32 APIs (<windows.h>, FindFirstFile, FindNextFile inside listAccounts()) and POSIX directory abstractions (<dirent.h>, opendir, readdir inside transactional sub-routines).

To compile and run this securely without encountering linker faults, use a MinGW or Cygwin compiler toolchain on a Windows machine, as they natively bridge both header specifications.

Compilation Command
Open your terminal inside the directory containing your source code and execute:


gcc banking_system.c -o banking_system
Program Usage
Upon executing the compiled binary, you will navigate through an interactive management console containing 6 modular tracks:


----------Banking System-----------
Please choose an option (1-6):
1. Create New Bank Account
2. Delete Bank Account
3. Deposit
4. Withdrawal
5. Remittance
6. Exit
Enter your choice:

Module Descriptions
Create New Bank Account: Prompts for username, ID string (leading spaces are rejected), and type. Writes a clean data block with an initial balance of 0.00.

Delete Bank Account: Reads all available accounts from disk, displays them, and unlinks/purges the file using remove().

Deposit: Converts an input money string safely into floating-point numbers via strtof() validation before augmenting the saved profile balance.

Withdrawal: Validates availability of principal amounts before subtracting funds.

Remittance: Captures target parameters for two matching source files, calculates fee liabilities, and safely updates both transaction states using standard file rewrites (freopen).

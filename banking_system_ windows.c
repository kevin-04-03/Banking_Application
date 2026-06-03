#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <dirent.h>
#include <ctype.h>


#define DATABASE_FOLDER "database/"

typedef struct {
    char name[100];
    char id[20];
    char account_type[10];
    int account_number;
    float balance;
} BankAccount;

void createBankAccount();
void listAccounts();
void deleteBankAccount();
void deposit();
void withdraw();
void remittance();
void createDatabaseFolder();
void clearBuffer();

int main() {
    int choice;

    createDatabaseFolder();

    while (1) {
        printf("----------Banking System-----------\n");
        printf("Please choose an option (1-6):\n");
        printf("1. Create New Bank Account\n");
        printf("2. Delete Bank Account\n");
        printf("3. Deposit\n");
        printf("4. Withdrawal\n");
        printf("5. Remittance\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        
        // Validate user input for menu choice
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            clearBuffer(); // Clear invalid input from buffer
            continue; // Restart the loop for a valid input
        }
        
        clearBuffer(); // Clear any additional input left in buffer

        switch (choice) {
            case 1:
                createBankAccount();
                break;
            case 2:
                deleteBankAccount();
                break;
            case 3:
                deposit();
                break;
            case 4:
                withdraw();
                break;
            case 5:
                remittance();
                break;
            case 6:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("\n");
        }
    }
    return 0;
}

// Function to clear input buffer
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Create database folder if it doesnt exists
void createDatabaseFolder() {
    #ifdef _WIN32
        _mkdir(DATABASE_FOLDER);
    #else
        mkdir(DATABASE_FOLDER, 0777);
    #endif
}

// List accounts available in database
void listAccounts(){
   WIN32_FIND_DATA fileData;
    HANDLE hFind = FindFirstFile(DATABASE_FOLDER "*", &fileData);
    int accountIndex = 1;

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("No accounts available.\n");
        return;
    }

    printf("Available Accounts:\n");
    do {
        if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char *fileName = strtok(fileData.cFileName, ".");
            printf("%d. Account Number: %s\n", accountIndex++, fileName);
        }
    } while (FindNextFile(hFind, &fileData) != 0);
    FindClose(hFind);         
}                  


// Function to create a new bank account
void createBankAccount() {
    BankAccount account;
    char filename[50];
    FILE *file;
    int valid_input = 0;
    int unique_account_number_found = 0;

    printf("---------------Create bank account--------------\n");
     // Input and validate name
    do {
        printf("Enter Name: ");
        fgets(account.name, sizeof(account.name), stdin);
        account.name[strcspn(account.name, "\n")] = 0;

         // Check for leading spaces
        if (isspace((unsigned char)account.name[0])) {
            printf("Name cannot start with a space. Please try again.\n");
        }

        if (strlen(account.name) == 0) {
            printf("Name cannot be empty. Please try again.\n");
        }
    } while (strlen(account.name) == 0 || isspace((unsigned char)account.name[0]));

    // Input and validate ID
    do {
        printf("Enter ID: ");
        fgets(account.id, sizeof(account.id), stdin);
        account.id[strcspn(account.id, "\n")] = 0;

        // Check for leading spaces
        if (isspace((unsigned char)account.id[0])) {
            printf("ID cannot start with a space. Please try again.\n");
        }

        if (strlen(account.id) == 0) {
            printf("ID cannot be empty. Please try again.\n");
        }
    } while (strlen(account.id) == 0 || isspace((unsigned char)account.id[0]));

    while (!valid_input) {
        printf("Enter Account Type:\n");
        printf("1. Savings\n");
        printf("2. Current\n");
        printf("Select Option: ");

        fgets(account.account_type, sizeof(account.account_type), stdin);
        account.account_type[strcspn(account.account_type, "\n")] = 0;

        if (strcmp(account.account_type, "1") == 0) {
            strcpy(account.account_type, "Savings");
            valid_input = 1;
        } else if (strcmp(account.account_type, "2") == 0) {
            strcpy(account.account_type, "Current");
            valid_input = 1;
        } else {
            printf("Invalid input. Please enter 1 for Savings and 2 for Current.\n");
        }
    }

    srand(time(0));
    do {
        account.account_number = (rand() % (1000000 - 10000 + 1)) + 10000;  // Generates a unique account number starting from 5 digits
        sprintf(filename, DATABASE_FOLDER "%d.txt", account.account_number);

        file = fopen(filename, "r");
        if (file == NULL){
            unique_account_number_found = 1;
        } else {
            fclose(file);
        }
    } while (!unique_account_number_found);
    
    account.balance = 0.0;

    sprintf(filename, DATABASE_FOLDER "%d.txt", account.account_number);

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error creating account file.\n");
        return;
    }

    fprintf(file, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %d\nBalance: %.2f\n",
            account.name, account.id, account.account_type, account.account_number, account.balance);

    fclose(file);

    printf("Account created successfully.\n");
    printf("Account Name: %s\n", account.name);
    printf("Account ID: %s\n", account.id);
    printf("Account Number: %d\n", account.account_number);
    printf("Account Type: %s\n", account.account_type);
    printf("Account Balance: %.2f\n", account.balance);
    printf("\n");
}

// Function to delete a bank account
void deleteBankAccount() {
    char account_number_str[20];
    int account_number;
    char filename[50];

    // Count the number of accounts in the folder
    DIR *dir = opendir(DATABASE_FOLDER);
    if (dir == NULL) {
        printf("Error: Unable to access account directory.\n");
        return;
    }

    int account_count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Count only .txt files
        if (strstr(entry->d_name, ".txt") != NULL) {
            account_count++;
        }
    }
    closedir(dir);

    if (account_count < 1) {
        printf("Error: No accounts to delete. At least one account is required.\n");
        printf("\n");
        return;
    }

    listAccounts();

    printf("Enter Full Account Number to delete: ");
    scanf("%s", account_number_str);
    // Validation for if the input is a valid numeric account number
    for (int i = 0; account_number_str[i] != '\0'; i++) {
        if (!isdigit(account_number_str[i])) {
            printf("Invalid account number. Please enter digits only.\n");
            printf("\n");
            return;
        }
    }
    account_number = atoi(account_number_str);
    clearBuffer();
    sprintf(filename, DATABASE_FOLDER "%d.txt", account_number);

    if (remove(filename) == 0) {
        printf("Account deleted successfully.\n");
        printf("\n");
    } else {
        printf("Error: Account not found.\n");
        printf("\n");
    }

    /* printf("Attempting to delete: %s\n", filename); // Debug statement
    if (remove(filename) == 0) {
        printf("Account deleted successfully.\n");
    } else {
        perror("Error deleting account");  // Prints the specific error message
    }*/

}

void deposit() {
    char account_number_str[20];
    int account_number;
    char amount_str[20];
    char *endptr;
    float amount;
    char filename[50];
    BankAccount account;
    FILE *file;

    // Count the number of accounts in the folder
    DIR *dir = opendir(DATABASE_FOLDER);
    if (dir == NULL) {
        printf("Error: Unable to access account directory.\n");
        return;
    }

    int account_count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Count only .txt files
        if (strstr(entry->d_name, ".txt") != NULL) {
            account_count++;
        }
    }
    closedir(dir);

    if (account_count < 1) {
        printf("Error: Not enough accounts to perform a deposit. At least one account is required.\n");
        printf("\n");
        return;
    }

    listAccounts();

    printf("Enter Full Account Number: ");
    scanf("%s", account_number_str);
    // Validation for if the input is a valid numeric account number
    for (int i = 0; account_number_str[i] != '\0'; i++) {
        if (!isdigit(account_number_str[i])) {
            printf("Invalid account number. Please enter digits only.\n");
            printf("\n");
            return;
        }
    }

    account_number = atoi(account_number_str);
    clearBuffer();
    sprintf(filename, DATABASE_FOLDER "%d.txt", account_number);

    file = fopen(filename, "r+");
    if (file == NULL) {
        printf("Account not found.\n");
        printf("\n");
        return;
    }

    fscanf(file, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %d\nBalance: %f\n",
           account.name, account.id, account.account_type, &account.account_number, &account.balance);

    printf("Enter amount to deposit: RM ");
    scanf("%s", amount_str);
    // Convert string amount to float amount and validate
    amount = strtof(amount_str, &endptr);
    if (endptr == amount_str || *endptr != '\0' || amount <= 0) {
        printf("Invalid amount. Please enter a positive number.\n");
        printf("\n");
        fclose(file);
        return;
    }

    // Perform the deposit
    account.balance += amount;

    // Update the account file
    rewind(file); // Reset file pointer to the beginning
    fprintf(file, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %d\nBalance: %.2f\n",
            account.name, account.id, account.account_type, account.account_number, account.balance);

    fclose(file);
    printf("Deposit successful. New Balance: RM %.2f\n", account.balance);
    printf("\n");
}

void withdraw() {
    char account_number_str[20];
    int account_number;
    char amount_str[20];
    char *endptr;
    float amount;
    char filename[50];
    BankAccount account;
    FILE *file;

     // Count the number of accounts in the folder
    DIR *dir = opendir(DATABASE_FOLDER);
    if (dir == NULL) {
        printf("Error: Unable to access account directory.\n");
        return;
    }

    int account_count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Count only .txt files
        if (strstr(entry->d_name, ".txt") != NULL) {
            account_count++;
        }
    }
    closedir(dir);

    if (account_count < 1) {
        printf("Error: Not enough accounts to perform a withdrawal. At least one account is required.\n");
        printf("\n");
        return;
    }

    listAccounts();
    printf("Enter Full Account Number: ");
    scanf("%s", account_number_str);
    // Validation for if the input is a valid numeric account number
    for (int i = 0; account_number_str[i] != '\0'; i++) {
        if (!isdigit(account_number_str[i])) {
            printf("Invalid account number. Please enter digits only.\n");
            printf("\n");
            return;
        }
    }
    account_number = atoi(account_number_str);
    clearBuffer();
    sprintf(filename, DATABASE_FOLDER "%d.txt", account_number);

    file = fopen(filename, "r+");
    if (file == NULL) {
        printf("Account not found.\n");
        printf("\n");
        return;
    }

    fscanf(file, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %d\nBalance: %f\n",
           account.name, account.id, account.account_type, &account.account_number, &account.balance);

    printf("Enter amount to withdraw: RM ");
    scanf("%s", amount_str);

    // Validation for if the input is a valid numeric account number
    amount = strtof(amount_str, &endptr);
    if (endptr == amount_str || *endptr != '\0' || amount <= 0) {
        printf("Invalid amount. Please enter a positive number.\n");
        printf("\n");
        fclose(file);
        return;
    }

    if (amount > account.balance) {
        printf("Insufficient funds.\n");
        printf("\n");
        fclose(file);
        return;
    }

    // Update balance
    account.balance -= amount;

    // Update the account file
    rewind(file); // Reset file pointer to the beginning
    fprintf(file, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %d\nBalance: %.2f\n",
            account.name, account.id, account.account_type, account.account_number, account.balance);

    fclose(file);
    printf("Withdrawal successful. New Balance: RM %.2f\n", account.balance);
    printf("\n");
}


// Function to transfer money between accounts
void remittance() {
    char from_account_str[20], to_account_str[20];
    int from_account, to_account;
    char amount_str[20];
    char *endptr;
    float amount, fee;
    char from_filename[50], to_filename[50];
    BankAccount from, to;
    FILE *file_from, *file_to;

    // Count the number of accounts in the folder
    DIR *dir = opendir(DATABASE_FOLDER);
    if (dir == NULL) {
        printf("Error: Unable to access account directory.\n");
        return;
    }

    int account_count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Count only .txt files
        if (strstr(entry->d_name, ".txt") != NULL) {
            account_count++;
        }
    }
    closedir(dir);

    if (account_count < 2) {
        printf("Error: Not enough accounts to perform a remittance. At least two accounts are required.\n");
        printf("\n");
        return;
    }

    listAccounts();
    printf("Enter Full Source Account Number: ");
    scanf("%s", from_account_str);
    // Validation for if the input is a valid numeric account number
    for (int i = 0; from_account_str[i] != '\0'; i++) {
        if (!isdigit(from_account_str[i])) {
            printf("Invalid account number. Please enter digits only.\n");
            printf("\n");
            return;
        }
    }
    from_account = atoi(from_account_str);
    clearBuffer();
    printf("Enter Full Destination Account Number: ");
    scanf("%s", to_account_str);
    // Validation for if the input is a valid numeric account number
    for (int i = 0; to_account_str[i] != '\0'; i++) {
        if (!isdigit(to_account_str[i])) {
            printf("Invalid account number. Please enter digits only.\n");
            printf("\n");
            return;
        }
    }
    to_account = atoi(to_account_str);

    if (from_account == to_account) {
        printf("Error: Source and destination accounts cannot be the same.\n");
        printf("\n");
        return;
    }
    clearBuffer();

    printf("Enter amount to transfer: ");
    scanf("%s", amount_str);

    // Convert string amount to float amount and validation
    amount = strtof(amount_str, &endptr);
    if (endptr == amount_str || *endptr != '\0' || amount <= 0) {
        printf("Invalid amount. Please enter a positive number.\n");
        printf("\n");
        return;
    }

    sprintf(from_filename, DATABASE_FOLDER "%d.txt", from_account);
    sprintf(to_filename, DATABASE_FOLDER "%d.txt", to_account);

    file_from = fopen(from_filename, "r+");
    file_to = fopen(to_filename, "r+");

    if (file_from == NULL || file_to == NULL) {
        printf("Error: One or both accounts not found.\n");
        printf("\n");
        if (file_from) fclose(file_from);
        if (file_to) fclose(file_to);
        return;
    }

    fscanf(file_from, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %d\nBalance: %f\n",
           from.name, from.id, from.account_type, &from.account_number, &from.balance);

    fscanf(file_to, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %d\nBalance: %f\n",
           to.name, to.id, to.account_type, &to.account_number, &to.balance);

    if (strcmp(from.account_type, "Savings") == 0 && strcmp(to.account_type, "Current") == 0) {
        fee = amount * 0.02;  // 2% fee
    } else if (strcmp(from.account_type, "Current") == 0 && strcmp(to.account_type, "Savings") == 0) {
        fee = amount * 0.03;  // 3% fee
    } else {
        fee = 0;
    }

    if (amount + fee > from.balance) {
        printf("Insufficient funds in source account.\n");
        printf("\n");
        fclose(file_from);
        fclose(file_to);
        return;
    } else {
        from.balance -= (amount + fee);
        to.balance += amount;

        freopen(from_filename, "w", file_from);
        fprintf(file_from, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %d\nBalance: %.2f\n",
                from.name, from.id, from.account_type, from.account_number, from.balance);
        fclose(file_from); // Ensure file is closed after update to prevent file from being unable to be found

        freopen(to_filename, "w", file_to);
        fprintf(file_to, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %d\nBalance: %.2f\n",
                to.name, to.id, to.account_type, to.account_number, to.balance);
        fclose(file_to); // Ensure file is closed after update to prevent file from being unable to be found

        printf("Remittance successful.\nNew Balance in Source: %.2f\nNew Balance in Destination: %.2f\nFees: %.2f\n", 
               from.balance, to.balance, fee);
        printf("\n");
    }
}


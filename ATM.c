#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CORRECT_PIN 1234
#define ACCOUNT_FILE "account_info.txt"
#define HISTORY_FILE "transaction_history.txt"

// Function Prototypes
int authenticate();
void checkBalance(float balance);
float depositMoney(float balance);
float withdrawMoney(float balance);
void logTransaction(const char *type, float amount, float finalBalance);

int main() {
    if (!authenticate()) {
        printf("\nToo many incorrect attempts! Access Denied.\n");
        return 0;
    }

    float balance = 5000.00; // Default Starting Balance
    int choice;

    // Load balance from file if exists (Simple persistence)
    FILE *file = fopen(ACCOUNT_FILE, "r");
    if (file != NULL) {
        fscanf(file, "%f", &balance);
        fclose(file);
    }

    while (1) {
        printf("\n==============================\n");
        printf("     ATM BANKING SYSTEM       \n");
        printf("==============================\n");
        printf("1. Check Balance\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. View Transaction History\n");
        printf("5. Exit & Save\n");
        printf("Enter your choice (1-5): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Clear buffer
            continue;
        }

        switch (choice) {
            case 1:
                checkBalance(balance);
                break;
            case 2:
                balance = depositMoney(balance);
                break;
            case 3:
                balance = withdrawMoney(balance);
                break;
            case 4:
                {
                    printf("\n--- Transaction History ---\n");
                    FILE *hFile = fopen(HISTORY_FILE, "r");
                    if (hFile == NULL) {
                        printf("No transaction history found.\n");
                    } else {
                        char ch;
                        while ((ch = fgetc(hFile)) != EOF) {
                            putchar(ch);
                        }
                        fclose(hFile);
                    }
                }
                break;
            case 5:
                // Save balance to file before exiting
                file = fopen(ACCOUNT_FILE, "w");
                if (file != NULL) {
                    fprintf(file, "%.2f", balance);
                    fclose(file);
                }
                printf("\nThank you for using our ATM. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}

// 1. PIN Authentication Function (Max 3 attempts)
int authenticate() {
    int enteredPin;
    int attempts = 3;

    while (attempts > 0) {
        printf("Enter your 4-digit PIN: ");
        if (scanf("%d", &enteredPin) != 1) {
            printf("Invalid input format.\n");
            while (getchar() != '\n');
            attempts--;
            continue;
        }

        if (enteredPin == CORRECT_PIN) {
            printf("Authentication Successful!\n");
            return 1;
        } else {
            attempts--;
            printf("Incorrect PIN! You have %d attempts left.\n", attempts);
        }
    }
    return 0;
}

// 2. Check Balance Function
void checkBalance(float balance) {
    printf("\n------------------------------\n");
    printf("Current Account Balance: $%.2f\n", balance);
    printf("------------------------------\n");
}

// 3. Deposit Money Function
float depositMoney(float balance) {
    float amount;
    printf("\nEnter amount to deposit: $");
    
    
    if (scanf("%f", &amount) != 1 || amount <= 0) {
        printf("Invalid amount! Deposit must be greater than zero.\n");
        while (getchar() != '\n'); // Clear input buffer
        return balance;
    }

    balance += amount;
    printf("Successfully deposited $%.2f\n", amount);
    checkBalance(balance);
    logTransaction("DEPOSIT", amount, balance);
    return balance;
}
// 4. Withdraw Money Function
float withdrawMoney(float balance) {
    float amount;
    printf("\nEnter amount to withdraw: $");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Invalid amount! Withdrawal must be greater than zero.\n");
        return balance;
    }

    if (amount > balance) {
        printf("Transaction Failed! Insufficient funds.\n");
        return balance;
    }

    balance -= amount;
    printf("Successfully withdrew $%.2f\n", amount);
    checkBalance(balance);
    logTransaction("WITHDRAW", amount, balance);
    return balance;
}

// 5. Transaction Logger Function (Appends to text file with timestamp)
void logTransaction(const char *type, float amount, float finalBalance) {
    FILE *file = fopen(HISTORY_FILE, "a");
    if (file == NULL) return;

    time_t now = time(NULL);
    char *timeStr = ctime(&now);
    timeStr[strcspn(timeStr, "\n")] = 0; // Remove newline from ctime

    fprintf(file, "[%s] Type: %s | Amount: $%.2f | Balance: $%.2f\n", timeStr, type, amount, finalBalance);
    fclose(file);
}
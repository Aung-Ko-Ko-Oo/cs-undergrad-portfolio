#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_PHONE 20
#define MAX_EMAIL 50
#define FILENAME "contacts.txt"

// Contact Structure
typedef struct {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char email[MAX_EMAIL];
} Contact;

// Function Prototypes
void addContact();
void displayContacts();
void searchContact();
void deleteContact();

int main() {
    int choice;

    while (1) {
        printf("\n==============================\n");
        printf(" PERSONAL CONTACT MANAGEMENT \n");
        printf("==============================\n");
        printf("1. Add New Contact\n");
        printf("2. Display All Contacts\n");
        printf("3. Search Contact by Name\n");
        printf("4. Delete Contact\n");
        printf("5. Exit\n");
        printf("Enter your choice (1-5): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Clear buffer
            continue;
        }
        getchar(); // Consume newline character left by scanf

        switch (choice) {
            case 1:
                addContact();
                break;
            case 2:
                displayContacts();
                break;
            case 3:
                searchContact();
                break;
            case 4:
                deleteContact();
                break;
            case 5:
                printf("\nExiting program. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}

// 1. Add Contact Function (Appends to file)
void addContact() {
    FILE *file = fopen(FILENAME, "ab"); // Append in binary mode
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    Contact c;

    printf("\n--- Add New Contact ---\n");
    printf("Enter Name: ");
    fgets(c.name, MAX_NAME, stdin);
    c.name[strcspn(c.name, "\n")] = 0; // Remove trailing newline

    printf("Enter Phone Number: ");
    fgets(c.phone, MAX_PHONE, stdin);
    c.phone[strcspn(c.phone, "\n")] = 0;

    printf("Enter Email: ");
    fgets(c.email, MAX_EMAIL, stdin);
    c.email[strcspn(c.email, "\n")] = 0;

    fwrite(&c, sizeof(Contact), 1, file);
    fclose(file);

    printf("Contact added successfully!\n");
}

// 2. Display All Contacts Function
void displayContacts() {
    FILE *file = fopen(FILENAME, "rb"); // Read in binary mode
    if (file == NULL) {
        printf("\nNo contacts found! (File is empty or does not exist)\n");
        return;
    }

    Contact c;
    int count = 1;

    printf("\n==================================================\n");
    printf("                  CONTACT LIST                    \n");
    printf("==================================================\n");
    printf("%-5s | %-20s | %-15s | %-20s\n", "No.", "Name", "Phone", "Email");
    printf("--------------------------------------------------\n");

    while (fread(&c, sizeof(Contact), 1, file)) {
        printf("%-5d | %-20s | %-15s | %-20s\n", count++, c.name, c.phone, c.email);
    }
    printf("==================================================\n");

    fclose(file);
}

// 3. Search Contact Function
void searchContact() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("\nNo contacts found to search.\n");
        return;
    }

    char searchName[MAX_NAME];
    printf("\nEnter name to search: ");
    fgets(searchName, MAX_NAME, stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    Contact c;
    int found = 0;

    printf("\n--- Search Results ---\n");
    while (fread(&c, sizeof(Contact), 1, file)) {
        // Case-sensitive search (Can be improved with strcasecmp)
        if (strcmp(c.name, searchName) == 0) {
            printf("Name  : %s\n", c.name);
            printf("Phone : %s\n", c.phone);
            printf("Email : %s\n", c.email);
            printf("------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("No contact found with the name '%s'.\n", searchName);
    }

    fclose(file);
}

// 4. Delete Contact Function
void deleteContact() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("\nNo contacts found to delete.\n");
        return;
    }

    char targetName[MAX_NAME];
    printf("\nEnter exact name of the contact to delete: ");
    fgets(targetName, MAX_NAME, stdin);
    targetName[strcspn(targetName, "\n")] = 0;

    // Temporary file to store remaining contacts
    FILE *tempFile = fopen("temp.txt", "wb");
    if (tempFile == NULL) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }

    Contact c;
    int deleted = 0;

    while (fread(&c, sizeof(Contact), 1, file)) {
        if (strcmp(c.name, targetName) != 0) {
            fwrite(&c, sizeof(Contact), 1, tempFile); // Keep non-matching contacts
        } else {
            deleted = 1; // Mark as found and deleted
        }
    }

    fclose(file);
    fclose(tempFile);

    // Replace original file with temporary file
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (deleted) {
        printf("Contact '%s' deleted successfully!\n", targetName);
    } else {
        printf("Contact '%s' not found.\n", targetName);
    }
}
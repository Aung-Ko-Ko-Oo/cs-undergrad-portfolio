#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50

// Student Structure Node
typedef struct Student {
    int id;
    char name[MAX_NAME];
    float gpa;
    struct Student *next;
} Student;

// Function Prototypes
void addStudent(Student **head);
void displayStudents(Student *head);
void searchStudent(Student *head);
void deleteStudent(Student **head);
void sortStudents(Student *head);
void freeList(Student *head);

int main() {
    Student *head = NULL;
    int choice;

    while (1) {
        printf("\n========================================\n");
        printf("    STUDENT GRADE MANAGEMENT SYSTEM     \n");
        printf("========================================\n");
        printf("1. Add New Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student by ID\n");
        printf("4. Delete Student by ID\n");
        printf("5. Sort Students by GPA (Highest First)\n");
        printf("6. Exit & Free Memory\n");
        printf("Enter your choice (1-6): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Clear buffer
            continue;
        }

        switch (choice) {
            case 1:
                addStudent(&head);
                break;
            case 2:
                displayStudents(head);
                break;
            case 3:
                searchStudent(head);
                break;
            case 4:
                deleteStudent(&head);
                break;
            case 5:
                sortStudents(head);
                break;
            case 6:
                freeList(head);
                printf("\nExiting program. Memory freed successfully. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}

// 1. Add Student at the End of Linked List
void addStudent(Student **head) {
    Student *newNode = (Student *)malloc(sizeof(Student));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("\nEnter Student ID: ");
    scanf("%d", &newNode->id);
    getchar(); // Consume newline

    printf("Enter Student Name: ");
    fgets(newNode->name, MAX_NAME, stdin);
    newNode->name[strcspn(newNode->name, "\n")] = 0;

    printf("Enter Student GPA (0.0 - 4.0): ");
    scanf("%f", &newNode->gpa);

    newNode->next = NULL;

    // If list is empty, make new node the head
    if (*head == NULL) {
        *head = newNode;
    } else {
        Student *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    printf("Student added successfully!\n");
}

// 2. Display All Students
void displayStudents(Student *head) {
    if (head == NULL) {
        printf("\nNo student records found!\n");
        return;
    }

    printf("\n==================================================\n");
    printf("                   STUDENT LIST                   \n");
    printf("==================================================\n");
    printf("%-10s | %-25s | %-5s\n", "ID", "Name", "GPA");
    printf("--------------------------------------------------\n");

    Student *temp = head;
    while (temp != NULL) {
        printf("%-10d | %-25s | %-5.2f\n", temp->id, temp->name, temp->gpa);
        temp = temp->next;
    }
    printf("==================================================\n");
}

// 3. Search Student by ID
void searchStudent(Student *head) {
    if (head == NULL) {
        printf("\nList is empty.\n");
        return;
    }

    int targetId;
    printf("\nEnter Student ID to search: ");
    scanf("%d", &targetId);

    Student *temp = head;
    while (temp != NULL) {
        if (temp->id == targetId) {
            printf("\n--- Student Found ---\n");
            printf("ID   : %d\n", temp->id);
            printf("Name : %s\n", temp->name);
            printf("GPA  : %.2f\n", temp->gpa);
            return;
        }
        temp = temp->next;
    }

    printf("Student with ID %d not found.\n", targetId);
}

// 4. Delete Student by ID
void deleteStudent(Student **head) {
    if (*head == NULL) {
        printf("\nList is empty. Nothing to delete.\n");
        return;
    }

    int targetId;
    printf("\nEnter Student ID to delete: ");
    scanf("%d", &targetId);

    Student *temp = *head;
    Student *prev = NULL;

    // If head node itself holds the ID to be deleted
    if (temp != NULL && temp->id == targetId) {
        *head = temp->next; // Changed head
        free(temp);         // Free memory
        printf("Student with ID %d deleted successfully.\n", targetId);
        return;
    }

    // Search for the ID to be deleted, keep track of the previous node
    while (temp != NULL && temp->id != targetId) {
        prev = temp;
        temp = temp->next;
    }

    // If ID was not present in linked list
    if (temp == NULL) {
        printf("Student with ID %d not found.\n", targetId);
        return;
    }

    // Unlink the node from linked list
    prev->next = temp->next;
    free(temp); // Free memory
    printf("Student with ID %d deleted successfully.\n", targetId);
}

// 5. Sort Students by GPA (Bubble Sort on Linked List data values)
void sortStudents(Student *head) {
    if (head == NULL || head->next == NULL) {
        printf("\nNot enough students to sort.\n");
        return;
    }

    int swapped;
    Student *ptr1;
    Student *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (ptr1->gpa < ptr1->next->gpa) { // Descending order (Highest GPA first)
                // Swap GPA
                float tempGpa = ptr1->gpa;
                ptr1->gpa = ptr1->next->gpa;
                ptr1->next->gpa = tempGpa;

                // Swap ID
                int tempId = ptr1->id;
                ptr1->id = ptr1->next->id;
                ptr1->next->id = tempId;

                // Swap Name
                char tempName[MAX_NAME];
                strcpy(tempName, ptr1->name);
                strcpy(ptr1->name, ptr1->next->name);
                strcpy(ptr1->next->name, tempName);

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    printf("\nStudents sorted successfully by GPA (Highest to Lowest)!\n");
    displayStudents(head);
}

// 6. Free Memory to prevent Memory Leaks
void freeList(Student *head) {
    Student *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
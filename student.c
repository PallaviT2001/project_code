#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "fileoperation.h"

struct Student *studentHead = NULL;

void insertStudent(int id, const char *name, int age, const char *contactNumber) {
    struct Student *newStudent = (struct Student *)malloc(sizeof(struct Student));
    if (!newStudent) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    newStudent->id = id;
    strncpy(newStudent->name, name, sizeof(newStudent->name) - 1);
    newStudent->name[sizeof(newStudent->name) - 1] = '\0';
    newStudent->age = age;
    strncpy(newStudent->contactNumber, contactNumber, sizeof(newStudent->contactNumber) - 1);
    newStudent->contactNumber[sizeof(newStudent->contactNumber) - 1] = '\0';
    newStudent->next = NULL;

    if (studentHead == NULL || strcmp(studentHead->name, newStudent->name) > 0) {
        newStudent->next = studentHead;
        studentHead = newStudent;
    } else {
        struct Student *current = studentHead;
        while (current->next != NULL && strcmp(current->next->name, newStudent->name) < 0) {
            current = current->next;
        }
        newStudent->next = current->next;
        current->next = newStudent;
    }
    writeToStudentFile("students.dat");
}

void deleteStudent(int id) {
    if (studentHead == NULL) {
        printf("Student list is empty.\n");
        return;
    }

    struct Student *temp = studentHead, *prev = NULL;
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Student with ID %d not found!\n", id);
        return;
    }
    if (prev == NULL) {
        studentHead = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
    printf("Student deleted successfully!\n");
    deleteStudentInFile("students.dat", id);
}

void updateStudent(int id) {
    struct Student *temp = studentHead;

    while (temp != NULL && temp->id != id) {
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Student with ID %d not found!\n", id);
        return;
    }

    int choice;
    printf("Select the field to update:\n");
    printf("1. Name\n");
    printf("2. Age\n");
    printf("3. Contact Number\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        printf("Enter new name: ");
        scanf(" %[^\n]", temp->name);
        updateStudentInFile("students.dat", id, temp->name, temp->age, temp->contactNumber);
        break;
    case 2:
        printf("Enter new age: ");
        scanf("%d", &temp->age);
        updateStudentInFile("students.dat", id, temp->name, temp->age, temp->contactNumber);
        break;
    case 3:
        printf("Enter new contact number: ");
        scanf(" %[^\n]", temp->contactNumber);
        updateStudentInFile("students.dat", id, temp->name, temp->age, temp->contactNumber);
        break;
    default:
        printf("Invalid choice! Please select a valid option.\n");
        return;
    }
    printf("Student updated successfully!\n");
}

void displayStudentDetails() {
    if (studentHead == NULL) {
        printf("No students found!\n");
        return;
    }

    printf("------Student Details:------\n");
    struct Student *temp = studentHead;
    while (temp != NULL) {
        printf("ID: %d, Name: %s, Age: %d, Contact Number: %s\n",
               temp->id, temp->name, temp->age, temp->contactNumber);
        temp = temp->next;
    }
}

struct Student* mergeSortedLists(struct Student* left, struct Student* right) {
    if (!left) return right;
    if (!right) return left;

    struct Student* result = NULL;

    if (left->id <= right->id) {
        result = left;
        result->next = mergeSortedLists(left->next, right);
    } else {
        result = right;
        result->next = mergeSortedLists(left, right->next);
    }
    return result;
}

struct Student* getMiddle(struct Student* head) {
    if (!head) return head;

    struct Student* slow = head;
    struct Student* fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

struct Student* mergeSort(struct Student* head) {
    if (!head || !head->next) return head;

    struct Student* middle = getMiddle(head);
    struct Student* nextOfMiddle = middle->next;

    middle->next = NULL;

    struct Student* left = mergeSort(head);
    struct Student* right = mergeSort(nextOfMiddle);

    return mergeSortedLists(left, right);
}

void sortStudentsByID() {
    if (studentHead == NULL || studentHead->next == NULL) {
        printf("Not enough students to sort.\n");
        return;
    }
    studentHead = mergeSort(studentHead);
    printf("Students sorted by ID.\n");
    displayStudentDetails();
}

void sortStudentsByName() {
    if (studentHead == NULL || studentHead->next == NULL) {
        printf("Not enough students to sort.\n");
        return;
    }
    for (struct Student *i = studentHead; i != NULL; i = i->next) {
        for (struct Student *j = i->next; j != NULL; j = j->next) {
            if (strcmp(i->name, j->name) > 0) {
                struct Student temp = *i;
                *i = *j;
                *j = temp;
                temp.next = i->next;
                i->next = j->next;
                j->next = temp.next;
            }
        }
    }
    printf("Students sorted by Name.\n");
    displayStudentDetails();
}

void searchStudentById(int id) {
    struct Student *temp = studentHead;

    while (temp != NULL) {
        if (temp->id == id) {
            printf("Student found:\n");
            printf("ID: %d, Name: %s, Age: %d, Contact Number: %s\n",
                   temp->id, temp->name, temp->age, temp->contactNumber);
            return;
        }
        temp = temp->next;
    }
    printf("Student with ID %d not found!\n", id);
}

int getTotalStudentCount() {
    int count = 0;
    struct Student *temp = studentHead;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}


//******************************************************************************//

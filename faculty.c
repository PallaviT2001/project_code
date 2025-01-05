#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "faculty.h"
#include "fileoperation.h"

struct Faculty *facultyHead = NULL;

void insertFaculty(int id, const char *name, const char *department, int age, const char *qualification) {
    struct Faculty *newFaculty = (struct Faculty *)malloc(sizeof(struct Faculty));
    if (!newFaculty) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    newFaculty->id = id;
    strncpy(newFaculty->name, name, sizeof(newFaculty->name) - 1);
    newFaculty->name[sizeof(newFaculty->name) - 1] = '\0';
    strncpy(newFaculty->department, department, sizeof(newFaculty->department) - 1);
    newFaculty->department[sizeof(newFaculty->department) - 1] = '\0';
    newFaculty->age = age;
    strncpy(newFaculty->qualification, qualification, sizeof(newFaculty->qualification) - 1);
    newFaculty->qualification[sizeof(newFaculty->qualification) - 1] = '\0';
    newFaculty->next = NULL;

    if (facultyHead == NULL || strcmp(facultyHead->name, newFaculty->name) > 0) {
        newFaculty->next = facultyHead;
        facultyHead = newFaculty;
    } else {
        struct Faculty *current = facultyHead;
        while (current->next != NULL && strcmp(current->next->name, newFaculty->name) < 0) {
            current = current->next;
        }
        newFaculty->next = current->next;
        current->next = newFaculty;
    }
    writeFacultyToFile("faculty_data.txt");
}

void deleteFaculty(int id) {
    if (facultyHead == NULL) {
        printf("Faculty list is empty.\n");
        return;
    }

    struct Faculty *temp = facultyHead, *prev = NULL;
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Faculty with ID %d not found!\n", id);
        return;
    }
    if (prev == NULL) {
        facultyHead = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
    printf("Faculty deleted successfully!\n");
    deleteFacultyInFile("faculty_data.txt", id);
}

void updateFaculty(int id) {
    struct Faculty *temp = facultyHead;

    while (temp != NULL && temp->id != id) {
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Faculty with ID %d not found!\n", id);
        return;
    }

    int choice;
    printf("What would you like to update?\n");
    printf("1. Name\n");
    printf("2. Department\n");
    printf("3. Age\n");
    printf("4. Qualification\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        printf("Enter new name: ");
        scanf(" %[^\n]", temp->name);
        break;
    case 2:
        printf("Enter new department: ");
        scanf(" %[^\n]", temp->department);
        break;
    case 3:
        printf("Enter new age: ");
        scanf("%d", &temp->age);
        break;
    case 4:
        printf("Enter new qualification: ");
        scanf(" %[^\n]", temp->qualification);
        break;
    default:
        printf("Invalid choice!\n");
        return;
    }
    printf("Faculty updated successfully!\n");
    updateFacultyInFile("faculty_data.txt",id,temp->name,temp->department,temp->age,temp->qualification);
}

void displayFacultyDetails() {
    if (facultyHead == NULL) {
        printf("No faculties available.\n");
        return;
    }
    printf("\nFaculty Details:\n");
    struct Faculty *temp = facultyHead;
    while (temp != NULL) {
        printf("ID: %d, Name: %s, Department: %s, Age: %d, Qualification: %s\n",
               temp->id, temp->name, temp->department, temp->age, temp->qualification);
        temp = temp->next;
    }
}

struct Faculty* mergeFacultySortedLists(struct Faculty* left, struct Faculty* right) {
    if (!left) return right;
    if (!right) return left;

    struct Faculty* result = NULL;

    if (left->id <= right->id) {
        result = left;
        result->next = mergeFacultySortedLists(left->next, right);
    } else {
        result = right;
        result->next = mergeFacultySortedLists(left, right->next);
    }
    return result;
}

struct Faculty* getFacultyMiddle(struct Faculty* head) {
    if (!head) return head;

    struct Faculty* slow = head;
    struct Faculty* fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

struct Faculty* mergeSortFaculty(struct Faculty* head) {
    if (!head || !head->next) return head;

    struct Faculty* middle = getFacultyMiddle(head);
    struct Faculty* nextOfMiddle = middle->next;

    middle->next = NULL;

    struct Faculty* left = mergeSortFaculty(head);
    struct Faculty* right = mergeSortFaculty(nextOfMiddle);

    return mergeFacultySortedLists(left, right);
}

void sortFacultiesByID() {
    if (facultyHead == NULL || facultyHead->next == NULL) {
        printf("Not enough faculties to sort.\n");
        return;
    }
    facultyHead = mergeSortFaculty(facultyHead);
    printf("Faculties sorted by ID.\n");
    displayFacultyDetails();
}

void sortFacultiesByName() {
    if (facultyHead == NULL || facultyHead->next == NULL) {
        printf("Not enough faculties to sort.\n");
        return;
    }

    for (struct Faculty *i = facultyHead; i != NULL; i = i->next) {
        for (struct Faculty *j = i->next; j != NULL; j = j->next) {
            if (strcmp(i->name, j->name) > 0) {
                struct Faculty temp = *i;
                *i = *j;
                *j = temp;
                temp.next = i->next;
                i->next = j->next;
                j->next = temp.next;
            }
        }
    }
    printf("Faculties sorted by Name.\n");
    displayFacultyDetails();
}

void searchFacultyById(int id) {
    struct Faculty *temp = facultyHead;

    while (temp != NULL) {
        if (temp->id == id) {
            printf("\nFaculty found:\n");
            printf("ID: %d, Name: %s, Department: %s, Age: %d, Qualification: %s\n",
                   temp->id, temp->name, temp->department, temp->age, temp->qualification);
            return;
        }
        temp = temp->next;
    }

    printf("Faculty with ID %d not found!\n", id);
}

int getTotalFacultyCount() {
    struct Faculty *temp = facultyHead;
    int count = 0;

    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    return count;
}

/*void addFacultyToList(struct Faculty *newFaculty) {
    if (facultyHead == NULL) {
        facultyHead = newFaculty;
    } else {
        struct Faculty *temp = facultyHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newFaculty;
    }
}*/

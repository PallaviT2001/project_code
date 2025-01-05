#include <stdio.h>
#include <stdlib.h>
#include "fileoperation.h"
#include "fees.h"
#include "student.h"

struct Fees *feesHead = NULL;

void insertFees(int studentID, int receipt_number, float paid_amount) {
    struct Student *student = NULL;

    struct Student *temp = studentHead;
    while (temp != NULL) {
        if (temp->id == studentID) {
            student = temp;
            break;
        }
        temp = temp->next;
    }

    if (student == NULL) {
        printf("Student with ID %d not found!\n", studentID);
        return;
    }

    struct Fees *newFees = (struct Fees *)malloc(sizeof(struct Fees));
    if (!newFees) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    newFees->studentDetails = student;
    newFees->receipt_number = receipt_number;
    newFees->paid_amount = paid_amount;
    newFees->next = NULL;

    addFeesToList(newFees);
    writeFeesToFile("fees.dat");
    printf("Fee record for student ID %d added successfully and written to file!\n", studentID);
}

void displayFeesDetails() {
    if (feesHead == NULL) {
        printf("No fee records available.\n");
        return;
    }

    printf("\nFees Details:\n");

    struct Fees *temp = feesHead;
    while (temp != NULL) {
        printf("Receipt Number: %d, Paid Amount: %.2f\n", temp->receipt_number, temp->paid_amount);

        if (temp->studentDetails != NULL) {
            struct Student *student = temp->studentDetails;
            printf("Student ID: %d, Name: %s, Age: %d, Contact Number: %s\n",
                   student->id,
                   student->name,
                   student->age,
                   student->contactNumber);
        } else {
            printf("No student details available.\n");
        }

        temp = temp->next;
    }
}

void addFeesToList(struct Fees *newFees) {
    if (feesHead == NULL) {
        feesHead = newFees;
    } else {
        struct Fees *temp = feesHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newFees;
    }

    writeFeesToFile("fees.dat");
}


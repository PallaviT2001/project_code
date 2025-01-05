#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "section.h"
#include "student.h"
#include "fileoperation.h"

struct Section *sectionHead = NULL;

void addSectionToList(struct Section *newSection) {
    if (sectionHead == NULL) {
        sectionHead = newSection;
    } else {
        struct Section *temp = sectionHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newSection;
    }

    writeSectionToFile("sections.dat");
}

void insertSection(int studentID, int section_id, const char *section_name) {
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

    struct Section *newSection = (struct Section *)malloc(sizeof(struct Section));
    if (!newSection) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    newSection->studentDetails = student;
    newSection->section_id = section_id;
    strncpy(newSection->section_name, section_name, sizeof(newSection->section_name) - 1);
    newSection->section_name[sizeof(newSection->section_name) - 1] = '\0';
    newSection->next = NULL;

    addSectionToList(newSection);
    writeSectionToFile("sections.dat");
    printf("Section record for student ID %d added successfully and written to file!\n", studentID);
}

void displaySectionDetails() {
    if (sectionHead == NULL) {
        printf("No section records available.\n");
        return;
    }

    printf("\nSection Details:\n");

    struct Section *temp = sectionHead;
    while (temp != NULL) {
        printf("Section ID: %d, Section Name: %s\n", temp->section_id, temp->section_name);

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

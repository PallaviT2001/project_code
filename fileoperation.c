#include "fileoperation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "faculty.h"
#include "section.h"
#include "fees.h"

struct Student *findStudentById(int student_id) {
    struct Student *temp = studentHead;
    while (temp != NULL) {
        if (temp->id == student_id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

FILE *openFile(const char *filename) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        file = fopen(filename, "w+");
        if (file == NULL) {
            printf("Error: Could not open or create the file: %s\n", filename);
            return NULL;
        }
    }
    return file;
}

//**************************************************************************************//

void loadStudentFromFile(const char *filename) {
    FILE *file = openFile(filename);
    if (file == NULL) {
        return ;
    }

    int id, age;
    char name[50], contactNumber[20];
    while (fscanf(file, "%d %49s %d %19s", &id, name, &age, contactNumber) != EOF) {
        insertStudent(id, name, age, contactNumber);
    }
    fclose(file);
    printf("Student Data loaded successfully from file.\n");
}

void writeToStudentFile(const char *filename) {
    FILE *file = openFile(filename);
    if (file == NULL) {
        return;
    }

    struct Student *temp = studentHead;
    while (temp != NULL) {
        fprintf(file, "%d %s %d %s\n", temp->id, temp->name, temp->age, temp->contactNumber);
        temp = temp->next;
    }

    fclose(file);
    printf("Data written to file successfully.\n");
}

void deleteStudentInFile(const char *filename, int student_id) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open the file.\n");
        return;
    }

    FILE *tempFile = fopen("tempfile.txt", "w");
    if (tempFile == NULL) {
        printf("Error: Could not create a temporary file.\n");
        fclose(file);
        return;
    }

    int id, age;
    char name[50], contactNumber[20];
    long position = findStudentPosition(file, student_id);
    if (position == -1) {
        printf("Student ID %d not found in the file.\n", student_id);
        fclose(file);
        fclose(tempFile);
        return;
    }

    rewind(file);
    while (fscanf(file, "%d  %s  %d  %s\n", &id, name, &age, contactNumber) != EOF) {
        if (id != student_id) {
            fprintf(tempFile, "%d  %s  %d  %s\n", id, name, age, contactNumber);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("tempfile.txt", filename);

    printf("Student ID %d deleted successfully from the file.\n", student_id);
}

long findStudentPosition(FILE *file, int student_id) {
    rewind(file);
    int id, age;
    char name[50], contactNumber[20];
    long position = ftell(file); // Get the starting position.

    while (fscanf(file, "%d  %s  %d  %s\n", &id, name, &age, contactNumber) != EOF) {
        if (id == student_id) {
            return position;
        }
        position = ftell(file);
    }

    return -1;
}

void updateStudentInFile(const char *filename, int student_id, const char *newName, int newAge, const char *newContact) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open the file for reading.\n");
        return;
    }

    FILE *tempFile = fopen("tempfile.txt", "w");
    if (tempFile == NULL) {
        printf("Error: Could not create a temporary file.\n");
        fclose(file);
        return;
    }

    int id, age;
    char name[50], contactNumber[20];
    long position = findStudentPosition(file, student_id);
    if (position == -1) {
        printf("Student ID %d not found in the file.\n", student_id);
        fclose(file);
        fclose(tempFile);
        return;
    }

    rewind(file);
    while (fscanf(file, "%d  %s  %d  %s\n", &id, name, &age, contactNumber) != EOF) {
        if (id == student_id) {
            fprintf(tempFile, "%d  %s  %d  %s\n", student_id, newName, newAge, newContact);
        } else {
            fprintf(tempFile, "%d  %s  %d  %s\n", id, name, age, contactNumber);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("tempfile.txt", filename);

    printf("Student ID %d updated successfully in the file.\n", student_id);
}

//*****************************************************************************//

void loadFacultyFromFile(const char *filename) {
    FILE *file = openFile(filename);
    if (file == NULL) {
        return;
    }

    int id, age;
    char name[50], department[50], qualification[50];
    while (fscanf(file, "%d %49s %49s %d %49s", &id, name, department, &age, qualification) != EOF) {
        insertFaculty(id, name, department, age, qualification);
    }

    fclose(file);
    printf("Faculty data loaded successfully from file.\n");
}

void writeFacultyToFile(const char *filename) {
    FILE *file = openFile(filename);
    if (file == NULL) {
        return;
    }

    struct Faculty *temp = facultyHead;
    while (temp != NULL) {
        fprintf(file, "%d %s %s %d %s\n", temp->id, temp->name, temp->department, temp->age, temp->qualification);
        temp = temp->next;
    }

    fclose(file);
    printf("Faculty data written to file successfully.\n");
}

void deleteFacultyInFile(const char *filename, int faculty_id) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open the file.\n");
        return;
    }

    FILE *tempFile = fopen("tempfile.txt", "w");
    if (tempFile == NULL) {
        printf("Error: Could not create a temporary file.\n");
        fclose(file);
        return;
    }

    int id, age;
    char name[50], department[50], qualification[50];
    long position = findFacultyPosition(file, faculty_id);
    if (position == -1) {
        printf("Faculty ID %d not found in the file.\n", faculty_id);
        fclose(file);
        fclose(tempFile);
        return;
    }

    rewind(file);
    while (fscanf(file, "%d  %s  %s  %d  %s\n", &id, name, department, &age, qualification) != EOF) {
        if (id != faculty_id) {
            fprintf(tempFile, "%d  %s  %s  %d  %s\n", id, name, department, age, qualification);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("tempfile.txt", filename);

    printf("Faculty ID %d deleted successfully from the file.\n", faculty_id);
}

long findFacultyPosition(FILE *file, int faculty_id) {
    rewind(file);
    int id, age;
    char name[50], department[50], qualification[50];
    long position = ftell(file);

    while (fscanf(file, "%d  %s  %s  %d  %s\n", &id, name, department, &age, qualification) != EOF) {
        if (id == faculty_id) {
            return position;
        }
        position = ftell(file);
    }

    return -1;
}

void updateFacultyInFile(const char *filename, int faculty_id, const char *newName, const char *newDepartment, int newAge, const char *newQualification) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open the file for reading.\n");
        return;
    }

    FILE *tempFile = fopen("tempfile.txt", "w");
    if (tempFile == NULL) {
        printf("Error: Could not create a temporary file.\n");
        fclose(file);
        return;
    }

    int id, age;
    char name[50], department[50], qualification[50];
    long position = findFacultyPosition(file, faculty_id);
    if (position == -1) {
        printf("Faculty ID %d not found in the file.\n", faculty_id);
        fclose(file);
        fclose(tempFile);
        return;
    }

    rewind(file);
    while (fscanf(file, "%d  %s  %s  %d  %s\n", &id, name, department, &age, qualification) != EOF) {
        if (id == faculty_id) {
            fprintf(tempFile, "%d  %s  %s  %d  %s\n", faculty_id, newName, newDepartment, newAge, newQualification);
        } else {
            fprintf(tempFile, "%d  %s  %s  %d  %s\n", id, name, department, age, qualification);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("tempfile.txt", filename);

    printf("Faculty ID %d updated successfully in the file.\n", faculty_id);
}

//*****************************************************************************************//

void loadSectionFromFile(const char *filename) {
    FILE *file = openFile(filename);
    if (file == NULL) {
        return;
    }

    int section_id, student_id;
    char section_name[50];
    while (fscanf(file, "%d %49s %d", &section_id, section_name, &student_id) != EOF) {
        struct Student *student = findStudentById(student_id);
        if (student != NULL) {
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
        } else {
            printf("Warning: Student ID %d not found while loading section data.\n", student_id);
        }
    }

    fclose(file);
    printf("Section data loaded successfully from file.\n");
}

void writeSectionToFile(const char *filename) {
    FILE *file = openFile(filename);
    if (file == NULL) {
        return;
    }

    struct Section *temp = sectionHead;
    while (temp != NULL) {
        if (temp->studentDetails != NULL) {
            fprintf(file, "%d %s %d\n", temp->section_id, temp->section_name, temp->studentDetails->id);
        }
        temp = temp->next;
    }

    fclose(file);
    printf("Section data written to file successfully.\n");
}

//*********************************************************************//

void loadFeesFromFile(const char *filename) {
    FILE *file = openFile(filename);
    if (file == NULL) {
        return;
    }

    int receipt_number, student_id;
    float paid_amount;
    while (fscanf(file, "%d %f %d", &receipt_number, &paid_amount, &student_id) != EOF) {
        struct Student *student = findStudentById(student_id);
        if (student != NULL) {
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
        } else {
            printf("Warning: Student ID %d not found while loading fee data.\n", student_id);
        }
    }

    fclose(file);
    printf("Fees data loaded successfully from file.\n");
}

void writeFeesToFile(const char *filename) {
    FILE *file = openFile(filename);
    if (file == NULL) {
        return;
    }

    struct Fees *temp = feesHead;
    while (temp != NULL) {
        if (temp->studentDetails != NULL) {
            fprintf(file, "%d %.2f %d\n", temp->receipt_number, temp->paid_amount, temp->studentDetails->id);
        }
        temp = temp->next;
    }

    fclose(file);
    printf("Fee data written to file successfully.\n");
}

//**************************************************************************//



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "faculty.h"
#include "fees.h"
#include "section.h"

#define MAX_NAME_LENGTH 50
#define RECORD_SIZE 10 + MAX_NAME_LENGTH + 10 + 20 + 5

FILE *studentFile = NULL;
FILE *facultyFile= NULL;

void openStudentFileForReadingWriting()
{
    studentFile = fopen("StudentRecords.txt", "r+");
    if (!studentFile)
    {
        studentFile = fopen("StudentRecords.txt", "w+");
        if (!studentFile)
        {
            return ;
        }
    }
    return ;
}
void closeStudentFile() {
    if (studentFile != NULL) {
        fflush(studentFile);
        fclose(studentFile);
    }
}
void serializeStudentRecord(const struct Student *student, char *buffer) {
    snprintf(buffer, RECORD_SIZE + 1,
             "%-10d%-50s%-10d%-20s%-5c\n",
             student->id,
             student->name,
             student->age,
             student->contactNumber,
             student->status);
}

void deserializeStudentRecord(const char *buffer, struct Student *student) {
    sscanf(buffer,
           "%10d%50s%10d%20s%5c",
           &student->id,
           student->name,
           &student->age,
           student->contactNumber,
           &student->status);

    // printf("DS : %d %s %d %s %c \n", student->id, student->name, student->age, student->contactNumber, student->status);
}

void saveStudentRecords(struct StudentList *list) {
    fseek(studentFile, 0, SEEK_SET);

    struct Student *temp = list->head;
    char buffer[RECORD_SIZE + 1];
    while (temp != NULL) {
        serializeStudentRecord(temp, buffer);
        fprintf(studentFile, "%s", buffer);
        temp = temp->next;
    }
    fflush(studentFile);
    printf("Student records saved successfully.\n");
}

void addStudentToFile(struct StudentList *list, struct Student newStudent) {
    fseek(studentFile, 0, SEEK_END);

    char buffer[RECORD_SIZE + 1];
    serializeStudentRecord(&newStudent, buffer);
    fprintf(studentFile, "%s\n", buffer);

    fflush(studentFile);
    printf("Student added successfully.\n");
}

void updateStudentFieldInFile(struct StudentList *list, int studentID, int field, void *newValue)
{
    fseek(studentFile, 0, SEEK_SET);
    char buffer[RECORD_SIZE + 1];
    struct Student temp;
    long currentPos = 0;

    while (fgets(buffer, RECORD_SIZE + 1, studentFile) != NULL) {
        deserializeStudentRecord(buffer, &temp);

        if (temp.id == studentID)
        {
            int fieldOffset = 0;
            switch (field)
            {
            case 1:
                fieldOffset = 10;
                break;
            case 2:
                fieldOffset = 10 + MAX_NAME_LENGTH;
                break;
            case 3:
                fieldOffset = 10 + MAX_NAME_LENGTH + 10;
                break;
            case 4:
                fieldOffset = 10 + MAX_NAME_LENGTH + 10 + 20;
                break;
            default:
                printf("Invalid field.\n");
                return;
            }

            fseek(studentFile, currentPos, SEEK_SET);
            fseek(studentFile, fieldOffset, SEEK_CUR);


            switch (field)
            {
            case 1:
                fprintf(studentFile, "%-*s", MAX_NAME_LENGTH, (char *)newValue);
                break;
            case 2:
                fprintf(studentFile, "%-10d", *(int *)newValue);
                break;
            case 3:
                fprintf(studentFile,  "%-*s", 20, (char *)newValue);
                break;
            case 4:
                fprintf(studentFile, "%-1c", *(char *)newValue);
                break;
            }

            fflush(studentFile);
            return;
        }

        currentPos = ftell(studentFile);
    }

    printf("Student with ID %d not found.\n", studentID);
}

void deleteStudentInFile(struct StudentList *list, int studentID) {
    fseek(studentFile, 0, SEEK_SET);
    char buffer[RECORD_SIZE + 1];
    struct Student temp;

    while (fgets(buffer, RECORD_SIZE + 1, studentFile) != NULL) {
        deserializeStudentRecord(buffer, &temp);

        if (temp.id == studentID)
        {
            temp.status = 'D';
            long studentoffset = ftell(studentFile)-RECORD_SIZE -80;
            fseek(studentFile, studentoffset, SEEK_SET);

            fprintf(studentFile, "%c", temp.status);
            fflush(studentFile);
            printf("Student marked as deleted.\n");
            return;
        }
    }

    printf("Student with ID %d not found.\n", studentID);
}


void loadStudentRecords(struct StudentList *list)
{
    fseek(studentFile, 0, SEEK_SET);
    list->head = NULL;
    char buffer[RECORD_SIZE + 1];

    while (fgets(buffer, RECORD_SIZE + 1, studentFile) != NULL)
    {
        if(strlen(buffer) < RECORD_SIZE)
        {
            continue;
        }
        struct Student *newStudent = (struct Student *)malloc(sizeof(struct Student));
        if(newStudent == NULL)
        {
            printf("Memory allocation Failed\n");
            return;
        }

        deserializeStudentRecord(buffer, newStudent);

        newStudent->next = NULL;

        if (list->head == NULL)
        {
            list->head = newStudent;

        }
        else
        {
            struct Student *current = list->head;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = newStudent;
        }
        list->studentCount++;
    }
    if(list->studentCount > 0)
    {
        printf("Student records loaded successfully.\n");
    }
    else
    {
        printf("No Student records found\n");
    }

}

//******************************************************************************************************//

#define MAX_FACULTY_NAME_LENGTH 50
#define MAX_FACULTY_DEPARTMENT_LENGTH 50
#define MAX_FACULTY_QUALIFICATION_LENGTH 50
#define FACULTY_RECORD_SIZE 10 + MAX_FACULTY_NAME_LENGTH + MAX_FACULTY_DEPARTMENT_LENGTH + 10 + MAX_FACULTY_QUALIFICATION_LENGTH + 5

void openFacultyFileForReadingWriting() {
    facultyFile = fopen("FacultyRecords.txt", "r+");
    if (!facultyFile) {
        facultyFile = fopen("FacultyRecords.txt", "w+");
        if (!facultyFile) {
            return;
        }
    }
    return;
}

void closeFacultyFile() {
    if (facultyFile != NULL) {
        fflush(facultyFile);
        fclose(facultyFile);
    }
}

void serializeFacultyRecord(const struct Faculty *faculty, char *buffer) {
    snprintf(buffer, FACULTY_RECORD_SIZE + 1,
             "%-10d%-50s%-50s%-10d%-50s%-5c\n",
             faculty->id,
             faculty->name,
             faculty->department,
             faculty->age,
             faculty->qualification,
             faculty->status);
}

void deserializeFacultyRecord(const char *buffer, struct Faculty *faculty) {
    sscanf(buffer,"%10d%50s%50s%10d%50s%5c",
           &faculty->id,
           faculty->name,
           faculty->department,
           &faculty->age,
           faculty->qualification,
           &faculty->status);
}

void saveFacultyRecords(struct FacultyList *list) {
    fseek(facultyFile, 0, SEEK_SET);

    struct Faculty *temp = list->head;
    char buffer[FACULTY_RECORD_SIZE + 1];
    while (temp != NULL) {
        serializeFacultyRecord(temp, buffer);
        fprintf(facultyFile, "%s", buffer);
        temp = temp->next;
    }
    fflush(facultyFile);
    printf("Faculty records saved successfully.\n");
}

void addFacultyToFile(struct FacultyList * list, struct Faculty newFaculty){
    fseek(facultyFile, 0, SEEK_END);

    char buffer[FACULTY_RECORD_SIZE + 1];
    serializeFacultyRecord(&newFaculty, buffer);
    fprintf(facultyFile, "%s\n", buffer);

    fflush(facultyFile);
    printf("Faculty added successfully.\n");
}

void updateFacultyFieldInFile(struct FacultyList *list, int facultyID, int field, void *newValue) {
    fseek(facultyFile, 0, SEEK_SET);
    char buffer[FACULTY_RECORD_SIZE + 1];
    struct Faculty temp;
    long currentPos = 0;

    while (fgets(buffer, FACULTY_RECORD_SIZE + 1, facultyFile) != NULL) {
        deserializeFacultyRecord(buffer, &temp);

        if (temp.id == facultyID) {
            int fieldOffset = 0;
            switch (field) {
            case 1:
                fieldOffset = 10;
                break;
            case 2:
                fieldOffset = 10 + MAX_FACULTY_NAME_LENGTH;
                break;
            case 3:
                fieldOffset = 10 + MAX_FACULTY_NAME_LENGTH + MAX_FACULTY_DEPARTMENT_LENGTH;
                break;
            case 4:
                fieldOffset = 10 + MAX_FACULTY_NAME_LENGTH + MAX_FACULTY_DEPARTMENT_LENGTH + 10;
                break;
            case 5:
                fieldOffset = 10 + MAX_FACULTY_NAME_LENGTH + MAX_FACULTY_DEPARTMENT_LENGTH + 10 + MAX_FACULTY_QUALIFICATION_LENGTH;
                break;
            default:
                printf("Invalid field.\n");
                return;
            }

            fseek(facultyFile, currentPos, SEEK_SET);
            fseek(facultyFile, fieldOffset, SEEK_CUR);

            switch (field) {
            case 1:
                fprintf(facultyFile, "%-50s", (char *)newValue);
                break;
            case 2:
                fprintf(facultyFile, "%-50s", (char *)newValue);
                break;
            case 3:
                fprintf(facultyFile, "%-10d", *(int *)newValue);
                break;
            case 4:
                fprintf(facultyFile, "%-50s", (char *)newValue);
                break;
            case 5:
                fprintf(facultyFile, "%-1c", *(char *)newValue);
                break;
            }

            fflush(facultyFile);
            printf("Faculty record updated successfully.\n");
            return;
        }

        currentPos = ftell(facultyFile);
    }

    printf("Faculty with ID %d not found.\n", facultyID);
}

void deleteFacultyInFile(struct FacultyList *list, int facultyID) {
    fseek(facultyFile, 0, SEEK_SET);
    char buffer[FACULTY_RECORD_SIZE + 1];
    struct Faculty temp;

    while (fgets(buffer, FACULTY_RECORD_SIZE + 1, facultyFile) != NULL) {
        deserializeFacultyRecord(buffer, &temp);

        if (temp.id == facultyID) {
            temp.status = 'D';
            long facultyOffset = ftell(facultyFile) - strlen(buffer);
            fseek(facultyFile, facultyOffset+170, SEEK_SET);

            fprintf(facultyFile, "%-1c", temp.status);
            fflush(facultyFile);
            printf("Faculty marked as deleted.\n");
            return;
        }
    }

    printf("Faculty with ID %d not found.\n", facultyID);
}

void loadFacultyRecords(struct FacultyList *list) {
    rewind(facultyFile);
    fseek(facultyFile, 0, SEEK_SET);
    list->head = NULL;
    char buffer[FACULTY_RECORD_SIZE + 1];

    while (fgets(buffer, FACULTY_RECORD_SIZE + 1, facultyFile) != NULL)
    {
        if(strlen(buffer) < FACULTY_RECORD_SIZE)
        {
            continue;
        }
        struct Faculty *newFaculty = (struct Faculty *)malloc(sizeof(struct Faculty));
        if (!newFaculty) {
            printf("Memory allocation failed.\n");
            return;
        }

        deserializeFacultyRecord(buffer, newFaculty);

        if (newFaculty->status == 'D') {
            free(newFaculty);
            continue;
        }

        newFaculty->next = NULL;

        if (list->head == NULL) {
            list->head = newFaculty;
        } else {
            struct Faculty *current = list->head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newFaculty;
        }
        list->facultyCount++;

    }
    if(list->facultyCount>0)
    {
        printf("faculty record loaded successfullty\n");
    }
    else
    {
        printf("No faculty records found\n");
    }
}

//************************************************************************************************************//

#define SECTION_FILE "sections.txt"
FILE *sectionFile = NULL;

void openSectionFileForReadingWriting() {
    sectionFile = fopen(SECTION_FILE, "r+");
    if (!sectionFile) {
        sectionFile = fopen(SECTION_FILE, "w+");
        if (!sectionFile) {
            return;
        }
    }
    return;
}

void closeSectionFile() {
    if (sectionFile != NULL) {
        fflush(sectionFile);
        fclose(sectionFile);
    }
}

struct Student *findStudentByIdfunctiontosection(struct StudentList *students, int studentId) {
    if (students == NULL || students->head == NULL) {
        return NULL;
    }

    struct Student *temp = students->head;
    while (temp != NULL) {
        if (temp->id == studentId) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void writeSectionToFile(struct Section **newSection) {
    FILE *file = fopen(SECTION_FILE, "a");
    if (!file) {
        perror("Error opening file for writing section details");
        return;
    }

    if ((*newSection)->studentDetails == NULL) {
        printf("Error: Student details are missing for this section record.\n");
        fclose(file);
        return;
    }

    fprintf(file, "%d,%s,%d,%s\n",
            (*newSection)->section_id,
            (*newSection)->section_name,
            (*newSection)->studentDetails->id,
            (*newSection)->studentDetails->name);

    fclose(file);
    printf("Section record written to file successfully.\n");
}

/*void loadSections(struct Section **sectionHead, struct StudentList *students) {
    FILE *file = fopen(SECTION_FILE, "r");
    if (!file) {
        perror("Error opening file for loading sections");
        return;
    }

    int sectionId = 0;
    char sectionName[100];
    int studentId = 0;
    struct Section *newSection = NULL;

    while (fscanf(file, "%d,%99[^,],%d\n", &sectionId, sectionName, &studentId) == 3) {
        newSection = (struct Section *)malloc(sizeof(struct Section));
        if (!newSection) {
            perror("Error allocating memory for section");
            fclose(file);
            exit(EXIT_FAILURE);
        }
        newSection->section_id = sectionId;
        strncpy(newSection->section_name, sectionName, sizeof(newSection->section_name) - 1);
        newSection->section_name[sizeof(newSection->section_name) - 1] = '\0';
        newSection->studentDetails =findStudentByIdfunctiontosection(students, studentId);

        newSection->next = *sectionHead;
        *sectionHead = newSection;
    }

    fclose(file);

    if (*sectionHead == NULL) {
        printf("No section records found.\n");
    } else {
        printf("Section records loaded successfully.\n");
    }
}*/
//***************************************************************************************************************//

#define FEES_FILE "fees.txt"
FILE *feesFile = NULL;

void openFeesFileForReadingWriting() {
    feesFile = fopen(FEES_FILE, "r+");
    if (!feesFile) {
        feesFile = fopen(FEES_FILE, "w+");
        if (!feesFile) {
            perror("Error opening fees file");
            return;
        }
    }
}

void closeFeesFile() {
    if (feesFile != NULL) {
        fflush(feesFile);
        fclose(feesFile);
    }
}

struct Student *findStudentByIdfunction(struct StudentList *students, int studentId) {
    if (students == NULL || students->head == NULL) {
        return NULL;
    }

    struct Student *current = students->head;
    while (current != NULL) {
        if (current->id == studentId) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

void writeFeesToFile(struct Fees* newFees)
{
    FILE *file = fopen(FEES_FILE, "a");
    if (!file) {
        perror("Error opening file for writing fees");
        return;
    }

    if (newFees->studentDetails == NULL) {
        printf("Error: Student details are missing for this fees record.\n");
        fclose(file);
        return;
    }
    //printf("In file------%d %f",newFees->receiptNumber,newFees->paidAmount);
    fprintf(file, "%d,%.2f,%d,%s\n",
            newFees->receiptNumber,
            newFees->paidAmount,
            newFees->studentDetails->id,
            newFees->studentDetails->name);

    fclose(file);
    printf("Fees record written to file successfully.\n");
}

//************************************************************************************************//

void loadSections(struct Section **sectionHead, struct StudentList *students)
{
    FILE *file = fopen(SECTION_FILE, "r");
    if (!file)
    {
        perror("Error opening file for loading sections");
        return;
    }

    int sectionId = 0;
    char sectionName[100];
    int studentId = 0;
    struct Section *newSection = NULL;

    while (fscanf(file, "%d,%99[^,],%d\n", &sectionId, sectionName, &studentId) == 3) {
        newSection = (struct Section *)malloc(sizeof(struct Section));
        if (!newSection) {
            perror("Error allocating memory for section");
            fclose(file);
            exit(EXIT_FAILURE);
        }
        newSection->section_id = sectionId;
        strncpy(newSection->section_name, sectionName, sizeof(newSection->section_name) - 1);
        newSection->section_name[sizeof(newSection->section_name) - 1] = '\0';
        newSection->studentDetails =findStudentByIdfunctiontosection(students, studentId);

        newSection->next = *sectionHead;
        *sectionHead = newSection;
    }

    fclose(file);

    if (*sectionHead == NULL) {
        printf("No section records found.\n");
    } else {
        printf("Section records loaded successfully.\n");
    }
}

// Function to load fees from a file and associate them with students
void loadFees(struct Fees **feesHeadAddress, struct StudentList *students) {
    FILE *file = fopen("fees.txt", "r");  // Open the file containing fee details
    if (!file) {
        perror("Error opening file");
        return;
    }

    int receiptNumber;
    float paidAmount;
    int studentID;
    char studentName[50];  // Assuming the name is up to 49 characters

    struct Fees *newFeesNode = NULL;
    struct Fees *lastFeesNode = NULL;

    // Read each line from the file and parse it
    while (fscanf(file, "%d,%f,%d,%49s", &receiptNumber, &paidAmount, &studentID, studentName) != EOF) {
        // Find the corresponding student in the StudentList by ID
        // printf("Student with receiptNumber %d %f %d %s.\n", receiptNumber, paidAmount, studentID, studentName);
        struct Student *studentDetails = NULL;
        struct Student *tempStudent = students->head;

        while (tempStudent != NULL) {
            if (tempStudent->id == studentID) {
                studentDetails = tempStudent;
                break;
            }
            tempStudent = tempStudent->next;
        }

        if (studentDetails == NULL) {
            printf("Student with ID %d not found.\n", studentID);
            continue; // Skip this fee entry if student isn't found
        }

        // Create a new Fees node
        newFeesNode = (struct Fees*)malloc(sizeof(struct Fees));
        if (!newFeesNode) {
            perror("Memory allocation error");
            fclose(file);
            return;
        }

        newFeesNode->receiptNumber = receiptNumber;
        newFeesNode->paidAmount = paidAmount;
        newFeesNode->studentDetails = studentDetails;
        newFeesNode->next = NULL;

        // printf("newFeesNode receiptNumber %d %f %d %s.\n", newFeesNode->receiptNumber, newFeesNode->paidAmount, newFeesNode->studentDetails->id, newFeesNode->studentDetails->name);
        // Add the new node to the linked list of fees
        if (*feesHeadAddress == NULL) {
            *feesHeadAddress = newFeesNode;  // The first node in the list
        } else {
            lastFeesNode->next = newFeesNode;  // Append to the list
        }

        lastFeesNode = newFeesNode;  // Update the last node pointer
    }
    // printf("In, Loadfess::  %p %p.\n", &feesHead, feesHead);
    // displayFees();

    fclose(file);  // Close the file after reading
}


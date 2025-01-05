#ifndef FILEOPERATION_H
#define FILEOPERATION_H
#include "student.h"
#include "faculty.h"
#include "section.h"
#include "fees.h"
#include <stdio.h>

FILE *openFile(const char *filename);
struct Student *findStudentById(int student_id);

void loadStudentFromFile(const char *filename);
void writeToStudentFile(const char *filename);
void updateStudentInFile(const char *filename, int student_id, const char *newName, int newAge, const char *newContact);
long findStudentPosition(FILE *file, int student_id);
void deleteStudentInFile(const char *filename, int student_id);

void loadFacultyFromFile(const char *filename);
void writeFacultyToFile(const char *filename);
long findFacultyPosition(FILE *file, int faculty_id);
void deleteFacultyInFile(const char *filename, int faculty_id);
void updateFacultyInFile(const char *filename, int faculty_id, const char *newName, const char *newDepartment, int newAge, const char *newQualification);

void loadSectionFromFile(const char *filename);
void writeSectionToFile(const char *filename);

void loadFeesFromFile(const char *filename);
void writeFeesToFile(const char *filename);



#endif

#ifndef SECTION_H
#define SECTION_H
#include "student.h"

struct Section {
    int section_id;
    char section_name[50];
    struct Student *studentDetails;
    struct Section *next;
};

extern struct Section *sectionHead;

void insertSection(int studentID, int section_id, const char *section_name);
void displaySectionDetails();
void addSectionToList(struct Section *newSection);

#endif

#ifndef FEES_H
#define FEES_H
#include "student.h"

struct Fees {
    int receipt_number;
    float paid_amount;
    struct Student *studentDetails;
    struct Fees *next;
};

extern struct Fees *feesHead;

void insertFees(int studentID, int receipt_number, float paid_amount);
void displayFeesDetails();
void addFeesToList(struct Fees *newFees);

#endif

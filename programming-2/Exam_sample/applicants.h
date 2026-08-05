#ifndef APPLICANTS_H
#define APPLICANTS_H

/* One applicant = one line of the data file. */
typedef struct {
    char *firstName;       /* dynamically allocated, owned by this struct */
    char *lastName;        /* dynamically allocated, owned by this struct */
    int   math;
    int   physics;
    int   electronics;
    int   agreedToPay;     /* 1 = agreed to pay if not selected, 0 = did not */
} Applicant;

/* Average of the three subject scores. */
double AdmissionPoints(const Applicant *a);

/* Reads the whole file into a dynamically grown array.
   Returns 1 on success, 0 on failure. Caller must FreeApplicants(). */
int ReadApplicants(const char *fileName, Applicant **outArr, int *outCount);

/* Frees every owned string and the array itself. */
void FreeApplicants(Applicant *arr, int count);

#endif
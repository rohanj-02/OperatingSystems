#include <unistd.h>
#include <stdio.h>

#ifndef LOCK_FUNCTIONS_H

#define LOCK_FUNCTIONS_H

int get_advisory_locks(FILE *fptr);
int release_advisory_locks(FILE *fptr);
int warn_user(FILE *fptr);

#endif
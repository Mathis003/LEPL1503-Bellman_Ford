#ifndef LEPL1503__VERIFY_OUTPUT__
#define LEPL1503__VERIFY_OUTPUT__

#include "../../../include/files.h"


bool check_same_size(FILE *f1, FILE *f2, bool verbose);

bool check_same_contains(FILE *f1, FILE *f2);

bool compare_two_outputFile(char *filename1, char *filename2, bool verbose);


#endif // LEPL1503__VERIFY_OUTPUT__

#ifndef HELPER_H
#define HELPER_H

#include "employee.h"

void clear_buffer(void);                            // clears input buffer, for safe next input.
int read_float(float *value);                       // a function specifically designed to check that if a float is given as input only or not.
int only_letters_and_spaces(const char *text);      // a function specifically designed to check that if only letters are given as input.
void printLine(int length);                         // prints a row of dashes.
void write_header(FILE *fp);                        // writes the column names as the first line of the CSV.
void pause_for_enter(void);                         // waits until the user presses Enter, so the output can be read before the menu returns.
int id_exists(const char *id);                      // checks if an employee ID already exists in memory.
int savedata(void);                                // rewrites the whole file from memory (used after update / delete).
void load_data(void);                               // loads programs data from file into the memory (an array), and also determines no. of employees in the file
void calculate_net_salary(Employee *e);             // calculates net salary of employee.
FILE *open_csv_for_append(void);                    // opens the CSV for appending (adds header to a new file, fixes a missing newline).

#endif

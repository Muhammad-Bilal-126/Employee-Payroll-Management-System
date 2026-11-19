#include<string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "employee.h"


/* Name of the data file. It is opened from the folder the program is run in. */
#define CSV_FILE "Employee Payroll Records.csv"
/* First line of the CSV file (column names). The order of the numbers matches the record order below. */
#define CSV_HEADER "NAME,ID,DESIGNATION,DEPARTMENT,EMAIL,DATE OF JOINING,BANK NAME,BANK ACCOUNT,BASIC SALARY,ALLOWANCE,OVERTIME PAY,BONUSES,TAXES,NET SALARY"
/* Pattern used by sscanf to split ONE csv line into the 14 fields of an Employee.
   %99[^,] = read text up to the next comma (so spaces inside a name are allowed, max 99 characters)
   %f      = read a decimal number
   The space before every % (and after each comma) skips the blanks that follow the commas. */
#define CSV_FORMAT " %99[^,], %19[^,], %99[^,], %99[^,], %99[^,], %19[^,], %49[^,], %49[^,], %f, %f, %f, %f, %f, %f"


/* =====================================================================
                            FILE HELPERS
===================================================================== */

void write_header(FILE *fp) {
    fprintf(fp, "%s\n", CSV_HEADER);
}

/* Opens the CSV in append mode ("a") so old records stay untouched. Before that it peeks at the file
   (binary mode "rb", so the file size and last character are exact):
   - file missing or empty -> the header line is written first
   - last record has no Enter at the end -> add one, otherwise the new record would be glued to it */
FILE *open_csv_for_append(void) {
    FILE *fp;
    long size = -1;
    int last = '\n';

    /* Look at the file first: does it exist, is it empty, does it end with a newline? */
    fp = fopen(CSV_FILE, "rb");

    if (fp != NULL) {
        if (fseek(fp, 0, SEEK_END) == 0) {
            size = ftell(fp);

            if (size > 0) {
                fseek(fp, -1L, SEEK_END);
                last = fgetc(fp);
            }
        }
        fclose(fp);
    }

    fp = fopen(CSV_FILE, "a");

    if (fp == NULL) {
        return NULL;
    }

    if (size <= 0) {
        write_header(fp);                 /* new or empty file: start with the column names */
    } else if (last != '\n') {
        fputc('\n', fp);                  /* last record had no newline: do not glue records together */
    }

    return fp;
}


void printLine(int length) {
    int i;

    for (i = 0; i < length; i++) {
        putchar('-');
    }

    putchar('\n');
}

/* =====================================================================
                            INPUT HELPERS
===================================================================== */

/* Throws away everything left in the input buffer up to and including Enter. Used after every scanf. */
void clear_buffer(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF);
}

/* Reads one decimal number safely. Returns 0 if the user typed ONLY a valid number, -1 otherwise
   (letters, "12abc", empty). The result is stored through the pointer 'value'. */
int read_float(float *value) {
    int c;

    if (scanf(" %f", value) != 1) {
        clear_buffer();
        return -1;
    }

    while ((c = getchar()) == ' ' || c == '\t');      /* skip spaces typed after the number */

    if (c != '\n' && c != EOF) {                        /* something other than Enter follows, e.g. "12abc": reject it */
        clear_buffer();
        return -1;
    }

    return 0;
}

/* Returns 0 if text contains only letters and spaces, -1 if any other character is found. */
int only_letters_and_spaces(const char *text) {
    int i;

    for (i = 0; text[i] != '\0'; i++) {
        if (isalpha((unsigned char)text[i]) == 0 && text[i] != ' ') {
            return -1;
        }
    }

    return 0;
}

/* Holds the screen until Enter is pressed. clear_buffer() reads characters until the Enter key,
   and the input buffer is already empty at this point, so it really waits for a new Enter. */
void pause_for_enter(void) {
    printf("\nPress Enter to return to the menu...");
    clear_buffer();
}

// checking if id has already exists

int id_exists(const char *id) {
    int i;
    for (i = 0; i < employee_count; i++) {
        if (strcmpi(emp[i].id, id) == 0) {
            return 1; // Duplicate found
        }
    }
    return 0; // Unique
}

/* The ONE formula for net salary, used by input() and every update. e is a pointer, so the change is made on the real employee. */
void calculate_net_salary(Employee *e) {
    e->net_salary = e->basic_salary + e->allowance + e->overtime_pay + e->bonuses - e->taxes;
}


/* =====================================================================
                            SAVE DATA
===================================================================== */

/* Rewrites the whole CSV from emp[]. Mode "w" erases the old file content first, so the header
   and every record are written again. Called after every update and delete. */
int savedata(void) {
    FILE *fp;
    int i;

    fp = fopen(CSV_FILE, "w");

    if (fp == NULL) {
        printf("\nError! Could not save the changes to the file.\n");
        return -1;
    }

    write_header(fp);

    for (i = 0; i < employee_count; i++) {
        fprintf(fp, "%s, %s, %s, %s, %s, %s, %s, %s, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n",
            emp[i].name,
            emp[i].id,
            emp[i].designation,
            emp[i].department,
            emp[i].email,
            emp[i].date_joining,
            emp[i].bank_name,
            emp[i].bank_account,
            emp[i].basic_salary,
            emp[i].allowance,
            emp[i].overtime_pay,
            emp[i].bonuses,
            emp[i].taxes,
            emp[i].net_salary);
    }
 
    fclose(fp);
    return 1;
}





/* =====================================================================
                            LOAD DATA
===================================================================== */

/* Reads the CSV file into emp[] and sets employee_count. Every line is split with sscanf and CSV_FORMAT:
   sscanf returns how many fields it filled, 14 means a complete, valid record. Anything else is
   skipped (the header line and blank lines silently, damaged lines with a warning). */
void load_data(void) {
    FILE *fp;
    char line[1024];
    int matched, skipped = 0, i, blank;

    employee_count = 0;

    fp = fopen(CSV_FILE, "r");

    if (fp == NULL) {
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL && employee_count < Max_Employee) {
        /* &emp[..].basic_salary etc.: numbers need the address (&), text arrays do not */
        matched = sscanf(line, CSV_FORMAT,
            emp[employee_count].name,
            emp[employee_count].id,
            emp[employee_count].designation,
            emp[employee_count].department,
            emp[employee_count].email,
            emp[employee_count].date_joining,
            emp[employee_count].bank_name,
            emp[employee_count].bank_account,
            &emp[employee_count].basic_salary,
            &emp[employee_count].allowance,
            &emp[employee_count].overtime_pay,
            &emp[employee_count].bonuses,
            &emp[employee_count].taxes,
            &emp[employee_count].net_salary);

        if (matched == 14) {
            employee_count++;
        } else {
            /* Not a record. Blank lines and the header row are normal; anything else is a damaged line. */
            blank = 1;

            for (i = 0; line[i] != '\0'; i++) {
                if (isspace((unsigned char)line[i]) == 0) {
                    blank = 0;
                    break;}
            }
            if (blank == 0 && strstr(line, "NAME,ID") == NULL) {
                skipped++;}
        }
    }
    fclose(fp);

    if (skipped > 0) {
        printf("\nWarning: %d line(s) in \"%s\" could not be read and were skipped.\n", skipped, CSV_FILE);
        printf("They have the wrong number or order of fields. If you update or delete an employee,\n");
        printf("those lines will be removed from the file, so fix or back up the file first.\n\n");
    }
}


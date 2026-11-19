
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#define Max_Employee 100
#define TAX_RATE 0.10    // Tax is always 10% of the basic salary. 
#define TABLE_WIDTH 190  /* Width of the one-row table: the 14 column widths in displayAllRecords() added up (164)
                            plus 13 gaps of 2 spaces (26). If you change a column width, change this number too. */

typedef struct {
    char name[100];
    char id[20];
    char designation[100];
    char department[100];
    char email[100];
    char date_joining[20];
    char bank_name[50];
    char bank_account[50];

    float basic_salary;
    float allowance;
    float overtime_pay;
    float bonuses;
    float taxes;
    float net_salary;

} Employee;

/* Global variable declarations */
extern Employee emp[Max_Employee];
extern int employee_count;

#endif

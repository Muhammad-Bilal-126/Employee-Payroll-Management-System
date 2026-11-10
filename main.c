#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

/*
Functions list:
1) Input employee details -> Shehryar
2) Add employee record -> Shehryar
3) Search employee by ID -> Bilal
4) Display all employees -> Qaim
5) Delete employee by ID -> Amin
6) Generate payslip -> Qaim
7) Update employee by ID -> Amin
8) Exit -> Qaim

EDITS MADE:
- Experience removed from CSV_FORMAT, struct, input(), add_employee(), display(),
  savedata(), updateEmployee(), load_data(), displayAllRecords(), generatePayslip()
- Taxes fixed at 10% using TAX_RATE
- validate_experience() and validate_taxes() removed
- add_employee() fixed
- employee_count bug fixed: load_data() is now the only source of employee_count
*/

#define Max_Employee 100

#define CSV_FORMAT " %99[^,], %19[^,], %99[^,], %99[^,], %99[^,], %19[^,], %49[^,], %49[^,], %f, %f, %f, %f, %f, %f"
#define TABLE_WIDTH 109
#define TAX_RATE 0.10f

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

/* Global variables */
Employee emp[Max_Employee];
int employee_count = 0;

/* Function prototypes */
int search(char arr[]);                             // searches for an employee in file and displays its entire data. 
void load_data(void);                               // loads programs data from file into the memory (an array), and also determines no. of employees in the 
void display(int i);                                // displays an employess data.
void updateEmployee(void);                          // update an employees certain record field like email, department, etc.
void deleteEmployee(void);                          // delete an employee record from file.
void savedata(void);                                //
void input(void);                                   // take valid input and return to add_employee to save in the file
int add_employee(void);                             // writes input into the file and calls function load_data(), so memory is also updated accordingly.
void displayAllRecords(void);                       // displays all records saved in the file.
void generatePayslip(void);                         // generates pay slip for an employee, based on his id.

void clear_buffer(void);                            // clears input buffer, for safe next input.
int read_float(float *value);                       // a function specifically designed to check that if a float is given as input only or not.
int only_letters_and_spaces(const char *text);      // a function specifically designed to check that if only letters are given as input.

// INPUT VALIDATORS (returns -1 for an invalid and 0 for a valid input.)  NOTE: it recieves input as an argument from input() function.
int validate_name(const char *name);                 
int validate_id(const char *id);
int validate_designation(const char *designation);
int validate_department(const char *department);
int validate_email(const char *email);
int validate_bank_name(const char *bank_name);
int validate_bank_account(const char *account);
int validate_basic_salary(float amount);
int validate_allowance(float amount);
int validate_overtime(float amount);
int validate_bonuses(float amount);

int main() {
    int option;

    /* Load actual employee records from CSV.
       load_data() is the only function that sets employee_count. */
    load_data();

    do {
        printf("%110s\n", "=== Employee Payroll Management System ===");
        printf("\n%70s\n", "1-Add a new employee");
        printf("\n%80s\n", "2-Display all employee records");
        printf("\n%79s\n", "3-Update an employee's record");
        printf("\n%77s\n", "4-Delete an employee record");
        printf("\n%81s\n", "5-Search for an employee record");
        printf("\n%84s\n", "6-Generate payslip for an employee");
        printf("\n%83s\n", "7-Exit from the management system");
        printf("\n%81s", "Which Option Do You Want to Choose: ");

        if (scanf("%d", &option) != 1) {
            clear_buffer();
            option = 0;
        } else {
            clear_buffer();
        }

        printf("Total no. of employees are: %d\n", employee_count);

        switch (option) {
            case 1:
                add_employee();
                break;

            case 2:
                displayAllRecords();
                break;

            case 3:
                updateEmployee();
                break;

            case 4:
                deleteEmployee();
                break;

            case 5: {
                char employid[20];

                printf("Enter the employee ID to search: ");
                scanf(" %19s", employid);
                clear_buffer();

                search(employid);
                break;
            }

            case 6:
                generatePayslip();
                break;

            case 7:
                printf("\nExiting From Management System Program\n");
                break;

            default:
                printf("\nInvalid Option Chosen!\n");
        }

    } while (option != 7);

    return 0;
}

/* =======================================================================
                                INPUTS
===================================================================== */

void input(void) {
    Employee *e = &emp[employee_count];               
    int status;

    printf("Enter The Details as prompted\n");

    /* Name */
    do {
        printf("\nName as per CNIC: ");
        scanf(" %99[^\n]", e->name);
        clear_buffer();

        status = validate_name(e->name);

        if (status == -1) {
            printf("Invalid name! Use letters only.\n");
        }
    } while (status == -1);

    /* ID */
    do {
        printf("\nID: ");
        scanf(" %19[^\n]", e->id);
        clear_buffer();

        status = validate_id(e->id);

        if (status == -1) {
            printf("Invalid ID! An ID can have letters, digits and hyphen ('-') only.\n");
        }
    } while (status == -1);

    /* Designation */
    do {
        printf("\nEmployee Designation: ");
        scanf(" %99[^\n]", e->designation);
        clear_buffer();

        status = validate_designation(e->designation);

        if (status == -1) {
            printf("Invalid designation entered! Use letters only.\n");
        }
    } while (status == -1);

    /* Department */
    do {
        printf("\nDepartment: ");
        scanf(" %99[^\n]", e->department);
        clear_buffer();

        status = validate_department(e->department);

        if (status == -1) {
            printf("Invalid department! Use letters only.\n");
        }
    } while (status == -1);

    /* Email */
    do {
        printf("\nEmail: ");
        scanf(" %99[^\n]", e->email);
        clear_buffer();

        status = validate_email(e->email);

        if (status == -1) {
            printf("Invalid email entered! Wrong format.\n");
        }
    } while (status == -1);

    /* Joining Date */
    printf("\nJoining Date strictly in format (dd/mm/yy): ");
    scanf(" %19[^\n]", e->date_joining);
    clear_buffer();

    /* Bank Name */
    do {
        printf("\nBank Name: ");
        scanf(" %49[^\n]", e->bank_name);
        clear_buffer();

        status = validate_bank_name(e->bank_name);

        if (status == -1) {
            printf("Invalid bank name! Bank name consists of letters only.\n");
        }
    } while (status == -1);

    /* Bank Account Number */
    do {
        printf("\nBank Account Number: ");
        scanf(" %49[^\n]", e->bank_account);
        clear_buffer();

        status = validate_bank_account(e->bank_account);

        if (status == -1) {
            printf("Invalid account number! Use 8 to 34 letters/digits, no spaces or special characters.\n");
        }
    } while (status == -1);

    /* Basic Salary */
    do {
        printf("\nBasic Salary (in PKR): ");
        status = read_float(&e->basic_salary);

        if (status == 0) {
            status = validate_basic_salary(e->basic_salary);
        }

        if (status == -1) {
            printf("Invalid salary entered! Enter a valid amount of salary.\n");
        }
    } while (status == -1);

    /* Allowance */
    do {
        printf("\nAllowance (in PKR): ");
        status = read_float(&e->allowance);

        if (status == 0) {
            status = validate_allowance(e->allowance);
        }

        if (status == -1) {
            printf("Invalid allowance! Enter a valid amount of allowance.\n");
        }
    } while (status == -1);

    /* Overtime Pay */
    do {
        printf("\nOvertime Pay (in PKR): ");
        status = read_float(&e->overtime_pay);

        if (status == 0) {
            status = validate_overtime(e->overtime_pay);
        }

        if (status == -1) {
            printf("Invalid overtime pay! Enter a valid amount of overtime pay.\n");
        }
    } while (status == -1);

    /* Bonus */
    do {
        printf("\nBonus (in PKR): ");
        status = read_float(&e->bonuses);

        if (status == 0) {
            status = validate_bonuses(e->bonuses);
        }

        if (status == -1) {
            printf("Invalid bonus! Enter a valid amount of bonus.\n");
        }
    } while (status == -1);

    /* Tax is fixed at 10% of basic salary */
    e->taxes = e->basic_salary * TAX_RATE;

    /* Calculate net salary */
    e->net_salary = e->basic_salary + e->allowance + e->overtime_pay + e->bonuses - e->taxes;
}

/* =====================================================================
                            ADD EMPLOYEE
===================================================================== */

int add_employee(void) {
    FILE *fp;

    if (employee_count >= Max_Employee) {
        printf("\nMaximum employee limit has been reached, no more employee entry possible.\n");
        return -1;
    }

    input();

    fp = fopen("Employee Payroll Records.csv", "a");

    if (fp == NULL) {
        printf("\nError! Unable to open employee records file.\n");
        return -1;
    }

    fprintf(fp, "%s, %s, %s, %s, %s, %s, %s, %s, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n",
        emp[employee_count].name,
        emp[employee_count].id,
        emp[employee_count].designation,
        emp[employee_count].department,
        emp[employee_count].email,
        emp[employee_count].date_joining,
        emp[employee_count].bank_name,
        emp[employee_count].bank_account,
        emp[employee_count].basic_salary,
        emp[employee_count].allowance,
        emp[employee_count].overtime_pay,
        emp[employee_count].bonuses,
        emp[employee_count].taxes,
        emp[employee_count].net_salary);

    fclose(fp);

    /* Increase count only after the employee has been successfully appended */
    employee_count++;

    printf("\nEmployee appended successfully in the file!\n");
    return 0;
}

/* =====================================================================
                            INPUT HELPERS
===================================================================== */

void clear_buffer(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF);
}

int read_float(float *value) {
    int c;

    if (scanf(" %f", value) != 1) {
        clear_buffer();
        return -1;
    }

    while ((c = getchar()) == ' ' || c == '\t');

    if (c != '\n' && c != EOF) {
        clear_buffer();
        return -1;
    }

    return 0;
}

int only_letters_and_spaces(const char *text) {
    int i;

    for (i = 0; text[i] != '\0'; i++) {
        if (isalpha((unsigned char)text[i]) == 0 && text[i] != ' ') {
            return -1;
        }
    }

    return 0;
}

/* =====================================================================
                            VALIDATORS
===================================================================== */

int validate_name(const char *name) {
    return only_letters_and_spaces(name);
}

int validate_id(const char *id) {
    int i;

    if (id[0] == '\0') {
        return -1;
    }

    for (i = 0; id[i] != '\0'; i++) {
        if (isalnum((unsigned char)id[i]) == 0 && id[i] != '-') {
            return -1;
        }
    }

    return 0;
}

int validate_designation(const char *designation) {
    return only_letters_and_spaces(designation);
}

int validate_department(const char *department) {
    return only_letters_and_spaces(department);
}

int validate_email(const char *email) {
    int i;
    const char *at;
    const char *dot;

    if (strlen(email) < 5) {
        return -1;
    }

    for (i = 0; i < (int)strlen(email); i++) {
        if (isspace((unsigned char)email[i])) {
            return -1;
        }
    }

    at = strchr(email, '@');

    if (at == NULL) {
        return -1;
    }

    if (strchr(at + 1, '@') != NULL) {
        return -1;
    }

    if (at == email) {
        return -1;
    }

    dot = strchr(at + 1, '.');

    if (dot == NULL) {
        return -1;
    }

    if (dot == at + 1) {
        return -1;
    }

    if (email[strlen(email) - 1] == '.') {
        return -1;
    }

    return 0;
}

int validate_bank_name(const char *bank_name) {
    return only_letters_and_spaces(bank_name);
}

int validate_bank_account(const char *account) {
    int i;

    if (strlen(account) < 8 || strlen(account) > 34) {
        return -1;
    }

    for (i = 0; i < (int)strlen(account); i++) {
        if (!isalnum((unsigned char)account[i])) {
            return -1;
        }
    }

    return 0;
}

int validate_basic_salary(float amount) {
    if (amount <= 0) {
        return -1;
    }

    return 0;
}

int validate_allowance(float amount) {
    if (amount < 0) {
        return -1;
    }

    return 0;
}

int validate_overtime(float amount) {
    if (amount < 0) {
        return -1;
    }

    return 0;
}

int validate_bonuses(float amount) {
    if (amount < 0) {
        return -1;
    }

    return 0;
}

/* =====================================================================
                            SEARCH / DISPLAY
===================================================================== */

int search(char employid[]) {
    int i;

    for (i = 0; i < employee_count; i++) {
        if (strcmp(emp[i].id, employid) == 0) {
            display(i);
            return i;
        }
    }

    printf("Wrong ID. Data Not Found\n");
    return -1;
}

void display(int i) {
    printf("Name: %s\n", emp[i].name);
    printf("ID: %s\n", emp[i].id);
    printf("Designation: %s\n", emp[i].designation);
    printf("Department: %s\n", emp[i].department);
    printf("Email: %s\n", emp[i].email);
    printf("Joining Date: %s\n", emp[i].date_joining);
    printf("Bank Name: %s\n", emp[i].bank_name);
    printf("Bank Account: %s\n", emp[i].bank_account);
    printf("Basic Salary: %.2f\n", emp[i].basic_salary);
    printf("Allowance: %.2f\n", emp[i].allowance);
    printf("Overtime Pay: %.2f\n", emp[i].overtime_pay);
    printf("Bonus: %.2f\n", emp[i].bonuses);
    printf("Taxes: %.2f\n", emp[i].taxes);
    printf("Final Salary: %.2f\n", emp[i].net_salary);
}

/* =====================================================================
                            SAVE DATA
===================================================================== */

void savedata(void) {
    FILE *fp;
    int i;
    fp = fopen("Employee Payroll Records.csv", "w");

    if (fp != NULL) {
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
    }
}

/* =====================================================================
                            DELETE EMPLOYEE
===================================================================== */

void deleteEmployee(void) {
    char delete_id[20];
    int index;
    char confirm;

    printf("Enter employee ID to delete: ");
    scanf("%19s", delete_id);
    clear_buffer();

    index = search(delete_id);

    if (index == -1) {
        printf("Employee not found. Deletion cancelled.\n");
        return;
    }

    printf("\nAre you sure you want to permanently delete this employee? (y/n): ");
    scanf(" %c", &confirm);
    clear_buffer();

    if (confirm != 'y' && confirm != 'Y') {
        printf("Deletion cancelled.\n");
        return;
    }

    for (int i = index; i < employee_count - 1; i++) {
        emp[i] = emp[i + 1];
    }

    employee_count--;

    savedata();

    printf("Employee deleted successfully.\n");
}

/* =====================================================================
                            UPDATE EMPLOYEE
===================================================================== */

void updateEmployee(void) {
    char search_id[20];
    int index;
    int choice;

    printf("Enter employee ID to update: ");
    scanf("%19s", search_id);
    clear_buffer();

    index = search(search_id);

    if (index == -1) {
        printf("Employee not found. Updation cancelled.\n");
        return;
    }

    do {
        printf("\n=== Update Fields for %s ===\n", emp[index].name);
        printf("1. Update Designation\n");
        printf("2. Update Department\n");
        printf("3. Update Email\n");
        printf("4. Update Basic Salary\n");
        printf("Choose a field to update (1-4), press 0 to exit: ");

        if (scanf("%d", &choice) != 1) {
            clear_buffer();
            choice = -1;
        } else {
            clear_buffer();
        }

        switch (choice) {
            case 1:
                printf("Enter new Designation: ");
                scanf(" %99[^\n]", emp[index].designation);
                clear_buffer();

                savedata();
                printf("Designation updated and saved successfully!\n");
                break;

            case 2:
                printf("Enter new Department: ");
                scanf(" %99[^\n]", emp[index].department);
                clear_buffer();

                savedata();
                printf("Department updated and saved successfully!\n");
                break;

            case 3:
                printf("Enter new Email: ");
                scanf(" %99[^\n]", emp[index].email);
                clear_buffer();

                savedata();
                printf("Email updated and saved successfully!\n");
                break;

            case 4:
                printf("Enter new Basic Salary: ");

                if (read_float(&emp[index].basic_salary) == 0 &&
                    validate_basic_salary(emp[index].basic_salary) == 0) {

                    emp[index].taxes = emp[index].basic_salary * TAX_RATE;

                    emp[index].net_salary =
                        emp[index].basic_salary +
                        emp[index].allowance +
                        emp[index].overtime_pay +
                        emp[index].bonuses -
                        emp[index].taxes;

                    savedata();
                    printf("Salary updated and saved successfully!\n");
                } else {
                    printf("Invalid salary entered! No changes were made.\n");
                }
                break;

            case 0:
                printf("Exiting Update Menu\n");
                break;

            default:
                printf("Invalid choice! No changes were made.\n");
        }

    } while (choice != 0);
}

/* =====================================================================
                            LOAD DATA
===================================================================== */

void load_data(void) {
    FILE *fp;
    char line[1024];
    int matched;

    fp = fopen("Employee Payroll Records.csv", "r");

    if (fp == NULL) {
        employee_count = 0;
        return;
    }

    employee_count = 0;

    while (fgets(line, sizeof(line), fp) != NULL && employee_count < Max_Employee) {
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
        }
    }

    fclose(fp);
}

/* =====================================================================
                            DISPLAY ALL RECORDS
===================================================================== */

void skipHeader(FILE *fp) {
    char line[800];

    if (fgets(line, sizeof(line), fp) == NULL || strstr(line, "NAME,ID") == NULL) {
        rewind(fp);
    }
}

void printLine(int length) {
    int i;

    for (i = 0; i < length; i++) {
        putchar('-');
    }

    putchar('\n');
}

void displayAllRecords(void) {
    FILE *fp;
    Employee e;
    int count = 0;

    fp = fopen("Employee Payroll Records.csv", "r");

    if (fp == NULL) {
        printf("\nNo records file found.\n");
        printf("\nPress Enter to return to the menu...");
        clear_buffer();
        return;
    }

    skipHeader(fp);

    printf("\n");
    printLine(TABLE_WIDTH);

    printf("%-20s %-10s %-16s %-14s %-26s %-15s\n",
        "NAME", "ID", "ROLE", "DEPARTMENT", "EMAIL", "DATE OF JOINING");

    printf("%-20s %-16s %12s %10s %12s %10s %10s %12s\n",
        "BANK NAME", "BANK ACCOUNT", "BASIC SALARY", "ALLOWANCE",
        "OVERTIME PAY", "TAXES", "BONUSES", "NET SALARY");

    printLine(TABLE_WIDTH);

    while (fscanf(fp, CSV_FORMAT,
        e.name, e.id, e.designation, e.department, e.email,
        e.date_joining, e.bank_name, e.bank_account,
        &e.basic_salary, &e.allowance, &e.overtime_pay,
        &e.bonuses, &e.taxes, &e.net_salary) == 14) {

        count++;

        printf("%-20.20s %-10.10s %-16.16s %-14.14s %-26.26s %-15.15s\n",
            e.name, e.id, e.designation, e.department, e.email, e.date_joining);

        printf("%-20.20s %-16.16s %12.2f %10.2f %12.2f %10.2f %10.2f %12.2f\n",
            e.bank_name, e.bank_account, e.basic_salary, e.allowance,
            e.overtime_pay, e.taxes, e.bonuses, e.net_salary);

        printLine(TABLE_WIDTH);
    }

    fclose(fp);

    if (count == 0) {
        printf("No employee records found.\n");
    } else {
        printf("Total employees: %d\n", count);
    }

    printf("\nPress Enter to return to the menu...");
    clear_buffer();
}

/* =====================================================================
                            GENERATE PAYSLIP
===================================================================== */

void generatePayslip(void) {
    FILE *fp;
    char id[20];
    char period[30];
    Employee e;
    int found = 0;
    int len;
    int i;
    float gross;
    float net;
    time_t now = time(NULL);

    printf("Enter employee ID for the payslip: ");
    scanf(" %19[^\n]", id);
    clear_buffer();

    fp = fopen("Employee Payroll Records.csv", "r");

    if (fp == NULL) {
        printf("\nNo records file found.\n");
        return;
    }

    skipHeader(fp);

    while (fscanf(fp, CSV_FORMAT,
        e.name, e.id, e.designation, e.department, e.email,
        e.date_joining, e.bank_name, e.bank_account,
        &e.basic_salary, &e.allowance, &e.overtime_pay,
        &e.bonuses, &e.taxes, &e.net_salary) == 14) {

        if (strcmp(e.id, id) == 0) {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("\nEmployee not found.\n");
        return;
    }

    strftime(period, sizeof(period), "%B %Y", localtime(&now));

    gross = e.basic_salary + e.allowance + e.overtime_pay + e.bonuses;
    net = gross - e.taxes;
    len = (int)strlen(e.bank_account);

    printf("\n");

    for (i = 0; i < 48; i++) {
        putchar('-');
    }

    printf("\n%27s\n%*s\n", "PAYSLIP", 24 + (int)strlen(period) / 2, period);

    for (i = 0; i < 48; i++) {
        putchar('-');
    }

    printf("\n%-16s: %s\n", "Employee ID", e.id);
    printf("%-16s: %s\n", "Name", e.name);
    printf("%-16s: %s\n", "Designation", e.designation);
    printf("%-16s: %s\n", "Department", e.department);
    printf("%-16s: %s\n", "Joining Date", e.date_joining);
    printf("%-16s: %s\n", "Bank", e.bank_name);

    if (len > 4) {
        printf("%-16s: ****%s\n", "Account", e.bank_account + len - 4);
    } else {
        printf("%-16s: %s\n", "Account", e.bank_account);
    }

    for (i = 0; i < 48; i++) {
        putchar('-');
    }

    printf("\nEARNINGS\n");
    printf("  %-28s %14.2f\n", "Basic Salary", e.basic_salary);
    printf("  %-28s %14.2f\n", "Allowance", e.allowance);
    printf("  %-28s %14.2f\n", "Overtime Pay", e.overtime_pay);
    printf("  %-28s %14.2f\n", "Bonus", e.bonuses);
    printf("  %-28s %14.2f\n", "Gross Salary", gross);

    for (i = 0; i < 48; i++) {
        putchar('-');
    }

    printf("\nDEDUCTIONS\n");
    printf("  %-28s %14.2f\n", "Taxes", e.taxes);

    for (i = 0; i < 48; i++) {
        putchar('-');
    }

    printf("\n%-30s %14.2f\n", "NET SALARY", net);

    for (i = 0; i < 48; i++) {
        putchar('-');
    }

    printf("\n");
}
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

/*
Important Functions list:
1) Input employee details -> Shehryar ul Haq
2) Add employee record -> Shehryar ul Haq
3) Search employee by ID -> Muhammad Bilal Ansari
4) Display all employees -> Qaim Abbas
5) Delete employee by ID -> Amin Aqil
6) Generate payslip -> Qaim Abbas
7) Update employee by ID -> Amin Aqil
8) Exit -> Qaim Abbas
*/

/*
HOW THE PROGRAM WORKS (overview)
- All employees live in the global array emp[]; employee_count says how many slots are in use.
- At start-up (after login) load_data() reads the CSV file into emp[].
- Add    : the new record is appended to the end of the CSV file and to emp[].
- Update / Delete : emp[] is changed first, then savedata() rewrites the WHOLE CSV from emp[].
- Display / Search / Payslip only read emp[], so they always agree with the file.
*/

#define Max_Employee 100

/* Login details (hard-coded for this project; real software would never store a password in the source code). */
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "1234"
#define MAX_LOGIN_ATTEMPTS 3

/* Name of the data file. It is opened from the folder the program is run in. */
#define CSV_FILE "Employee Payroll Records.csv"
/* First line of the CSV file (column names). The order of the numbers matches the record order below. */
#define CSV_HEADER "NAME,ID,DESIGNATION,DEPARTMENT,EMAIL,DATE OF JOINING,BANK NAME,BANK ACCOUNT,BASIC SALARY,ALLOWANCE,OVERTIME PAY,BONUSES,TAXES,NET SALARY"
/* Pattern used by sscanf to split ONE csv line into the 14 fields of an Employee.
   %99[^,] = read text up to the next comma (so spaces inside a name are allowed, max 99 characters)
   %f      = read a decimal number
   The space before every % (and after each comma) skips the blanks that follow the commas. */
#define CSV_FORMAT " %99[^,], %19[^,], %99[^,], %99[^,], %99[^,], %19[^,], %49[^,], %49[^,], %f, %f, %f, %f, %f, %f"
/* Width of the one-row table: the 14 column widths in displayAllRecords() added up (164)
   plus 13 gaps of 2 spaces (26). If you change a column width, change this number too. */
#define TABLE_WIDTH 190
/* Tax is always 10% of the basic salary. */
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
int search(char arr[]);                             // searches for an employee in memory, prints a message if not found, returns its index (or -1).
int id_exists(const char *id);                      // checks if an employee ID already exists in memory.
void load_data(void);                               // loads programs data from file into the memory (an array), and also determines no. of employees in the file
void display(char arr[]);                           // displays an employess data.
void updateEmployee(void);                          // update an employees certain record field like email, department, etc.
void deleteEmployee(void);                          // delete an employee record from file.
void savedata(void);                                // rewrites the whole file from memory (used after update / delete).
int input(void);                                   // take valid input and return to add_employee to save in the file
int add_employee(void);                             // appends the new employee to the file; memory is updated accordingly.
void displayAllRecords(void);                       // displays all records, one employee per row.
void generatePayslip(void);                         // generates pay slip for an employee, based on his id.
int login(void);                                    // asks for username and password (3 attempts). returns 1 if correct, 0 if not.
void pause_for_enter(void);                         // waits until the user presses Enter, so the output can be read before the menu returns.

void clear_buffer(void);                            // clears input buffer, for safe next input.
int read_float(float *value);                       // a function specifically designed to check that if a float is given as input only or not.
int only_letters_and_spaces(const char *text);      // a function specifically designed to check that if only letters are given as input.
void calculate_net_salary(Employee *e);             // net = basic + allowance + overtime + bonus - taxes (one formula, used everywhere).
void printLine(int length);                         // prints a row of dashes.
void write_header(FILE *fp);                        // writes the column names as the first line of the CSV.
FILE *open_csv_for_append(void);                    // opens the CSV for appending (adds header to a new file, fixes a missing newline).

// INPUT VALIDATORS (returns -1 for an invalid and 0 for a valid input.)  NOTE: it recieves input as an argument from input() function.
int validate_name(const char *name);
int validate_id(const char *id);
int validate_designation(const char *designation);
int validate_department(const char *department);
int validate_email(const char *email);
int validate_date(const char *date);
int validate_bank_name(const char *bank_name);
int validate_bank_account(const char *account);
int validate_basic_salary(float amount);
int validate_allowance(float amount);
int validate_overtime(float amount);
int validate_bonuses(float amount);

int main(void) {
    int option;

    /* Step 1: the user must log in before the menu is shown.
       After 3 wrong attempts the program ends and returns 1 (non-zero = error). */
    if (login() == 0) {
        printf("\nToo many failed login attempts. Program closed.\n");
        return 1;
    }

    /* Step 2: load actual employee records from CSV.
       load_data() is the only function that sets employee_count. */
    load_data();

    do {
        printf("\n");                                               /* blank line before the menu */
        printf("%s\n", "=== Employee Payroll Management System ===");
        printf("==========================================\n");
        printf("\n%s\n", "1-Add a new employee");
        printf("\n%s\n", "2-Display all employee records");
        printf("\n%s\n", "3-Update an employee's record");
        printf("\n%s\n", "4-Delete an employee record");
        printf("\n%s\n", "5-Search for an employee record");
        printf("\n%s\n", "6-Generate payslip for an employee");
        printf("\n%s\n", "7-Exit from the management system");
        printf("\n%s", "Which Option Do You Want to Choose: ");

        /* scanf returns how many values it read. If it is not 1, the user typed something that is not a number.
           clear_buffer() removes the rest of the line (including the Enter key), otherwise it would
           be picked up by the next scanf and skip an input. option = 0 makes the switch print "Invalid". */
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
                scanf(" %19s", employid);                           /* %19s: at most 19 characters, so the array cannot overflow */
                clear_buffer();

                display(employid);
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

        /* After every action (except Exit) wait for Enter, so the result stays on screen
           until the user has read it. Then the loop shows the menu again. */
        if (option != 7) {
            pause_for_enter();
        }

    } while (option != 7);

    return 0;
}

/* =======================================================================
                                LOGIN
===================================================================== */

/* Returns 1 if the correct username AND password are entered within MAX_LOGIN_ATTEMPTS tries, otherwise 0.
   Note: the password is visible while typing, hiding it needs non-standard functions (like getch). */
int login(void) {
    char username[30];
    char password[30];
    int attempt;

    printf("\n=== Employee Payroll Management System ===\n");
    printf("                  LOGIN\n");

    for (attempt = 1; attempt <= MAX_LOGIN_ATTEMPTS; attempt++) {
        username[0] = '\0';                                         /* empty strings, so a failed scanf cannot leave garbage behind */
        password[0] = '\0';

        printf("\nUsername: ");
        scanf(" %29[^\n]", username);                               /* reads a whole line (max 29 characters) */
        clear_buffer();

        printf("Password: ");
        scanf(" %29[^\n]", password);
        clear_buffer();

        /* strcmp returns 0 when both texts are exactly the same (it is case-sensitive). */
        if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
            printf("\nLogin successful. Welcome, %s!\n", ADMIN_USERNAME);
            return 1;
        }

        printf("\nIncorrect username or password.");

        if (attempt < MAX_LOGIN_ATTEMPTS) {
            printf(" Attempts left: %d\n", MAX_LOGIN_ATTEMPTS - attempt);
        } else {
            printf("\n");
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

/* =======================================================================
                                INPUTS
===================================================================== */

/* Asks for every field of a new employee and checks each answer.
   Pattern used for every text field: ask -> read -> validate -> repeat while the answer is invalid.
   scanf(" %99[^\n]"): the leading space skips old Enter keys, %99[^\n] reads a whole line
   (spaces allowed) but never more than 99 characters, so the array cannot overflow. */
int input(void) {
    Employee *e = &emp[employee_count];     /* e points to the first free slot of emp[]: the new employee is built directly there */
    int status;                             /* validators return 0 = valid, -1 = invalid */

    printf("Enter The Details as prompted\n");

    /* Name */
    do {
        printf("\nName as per CNIC (or type 'exit' to cancel): ");
        scanf(" %99[^\n]", e->name);
        clear_buffer();
        // Checks if the user wants to exit the input process because he may have press one accedentally the add employee option and wants to return to the main menu.
        if (strcmpi(e->name, "exit") == 0) {
        printf("\nOperation cancelled. Returning to main menu...\n");
        return -1;
       }  

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
        } else if (id_exists(e->id) == 1) {      /* an ID must be unique, otherwise search/update/delete would hit the wrong person */
            printf("An employee with this ID already exists! IDs must be unique.\n");
            status = -1;
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
    do {
        printf("\nJoining Date strictly in format (dd/mm/yy): ");
        scanf(" %19[^\n]", e->date_joining);
        clear_buffer();

        status = validate_date(e->date_joining);

        if (status == -1) {
            printf("Invalid date! Use the format dd/mm/yy with a real day and month (e.g. 25/03/24).\n");
        }
    } while (status == -1);

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

    /* Basic Salary (numbers: read_float() rejects letters, then the validator checks the value itself) */
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
    calculate_net_salary(e);
    return 0;
}

/* =====================================================================
                            ADD EMPLOYEE
===================================================================== */

/* Adds one employee: check space -> input() -> append to the CSV file -> count it.
   employee_count is increased only AFTER the file write succeeded, so memory and file never disagree.
   Returns 0 on success, -1 on failure. */
int add_employee(void) {
    FILE *fp;
    int input_result;
    if (employee_count >= Max_Employee) {
        printf("\nMaximum employee limit has been reached, no more employee entry possible.\n");
        return -1;
    }

    input_result = input();

    if (input_result == -1) {
        return -1;
    }

    fp = open_csv_for_append();

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

/* The ONE formula for net salary, used by input() and every update. e is a pointer, so the change is made on the real employee. */
void calculate_net_salary(Employee *e) {
    e->net_salary = e->basic_salary + e->allowance + e->overtime_pay + e->bonuses - e->taxes;
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

/* =====================================================================
                            VALIDATORS
===================================================================== */

int validate_name(const char *name) {
    return only_letters_and_spaces(name);
}

int validate_id(const char *id) {
    int i;

    

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

/* Basic email check: at least 5 characters, no spaces or commas, exactly one '@' that is not first,
   and a '.' after the '@' that is neither right after it nor the last character. */
int validate_email(const char *email) {
    int i;
    const char *at;
    const char *dot;

    if (strlen(email) < 5) {
        return -1;
    }

    /* no spaces, and no commas (a comma would split the CSV line into the wrong fields) */
    for (i = 0; i < (int)strlen(email); i++) {
        if (isspace((unsigned char)email[i]) || email[i] == ',') {
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

/* Joining date must be exactly dd/mm/yy with a real day and month. */
/* Joining date must be exactly dd/mm/yy: 8 characters, '/' at positions 2 and 5, digits elsewhere,
   month 1-12 and a day that exists in that month (February has 29 days in leap years). */
int validate_date(const char *date) {
    static const int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int i, day, month, year, max_day;
   
    if (strlen(date) != 8) {
        return -1;
    }

    for (i = 0; i < 8; i++) {
        if (i == 2 || i == 5) {
            if (date[i] != '/') {
                return -1;
            }
        } else if (isdigit((unsigned char)date[i]) == 0) {
            return -1;
        }
    }

    day   = (date[0] - '0') * 10 + (date[1] - '0');
    month = (date[3] - '0') * 10 + (date[4] - '0');
    year  = (date[6] - '0') * 10 + (date[7] - '0');       /* yy means 20yy */

    if (month < 1 || month > 12) {
        return -1;
    }

    max_day = days[month - 1];

    if (month == 2 && year % 4 == 0) {
        max_day = 29;                                       /* leap year */
    }

    if (day < 1 || day > max_day) {
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

/* isfinite() rejects "nan" and "inf", which scanf("%f") would otherwise accept. */
int validate_basic_salary(float amount) {
    if (!isfinite(amount) || amount <= 0) {
        return -1;
    }

    return 0;
}

int validate_allowance(float amount) {
    if (!isfinite(amount) || amount < 0) {
        return -1;
    }

    return 0;
}

int validate_overtime(float amount) {
    if (!isfinite(amount) || amount < 0) {
        return -1;
    }

    return 0;
}

int validate_bonuses(float amount) {
    if (!isfinite(amount) || amount < 0) {
        return -1;
    }

    return 0;
}

/* =====================================================================
                            SEARCH / DISPLAY
===================================================================== */

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

int search(char employid[]) {
    int i;

    for (i = 0; i < employee_count; i++) {
        if (strcmpi(emp[i].id, employid) == 0) {
            return i;
        }
    }
    printf("Wrong ID. Data Not Found\n");
    return -1;
}
/* =====================================================================

                DISPLAY FOR SEARCHED EMPLOYEE RECORD

 =====================================================================*/
void display(char employid[]) {
    int i;
    i = search(employid);
    if (i != -1) {
    printf("\nName: %s\n", emp[i].name);
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
 }

/* =====================================================================
                            SAVE DATA
===================================================================== */

/* Rewrites the whole CSV from emp[]. Mode "w" erases the old file content first, so the header
   and every record are written again. Called after every update and delete. */
void savedata(void) {
    FILE *fp;
    int i;

    fp = fopen(CSV_FILE, "w");

    if (fp == NULL) {
        printf("\nError! Could not save the changes to the file.\n");
        return;
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
}

/* =====================================================================
                            DELETE EMPLOYEE
===================================================================== */

/* Deletes one employee after asking for confirmation, then saves the file. */
void deleteEmployee(void) {
    char delete_id[20];
    int index;
    int i;
    char confirm;

    printf("Enter employee ID to delete: ");
    scanf("%19s", delete_id);
    clear_buffer();

    index = search(delete_id);

    if (index == -1) {
        printf("Deletion cancelled.\n");
        return;
    }
    printf("\n=== Record Deletion of %s ===\n", emp[index].name);
    printf("\nAre you sure you want to permanently delete this employee? (y/n): ");
    scanf(" %c", &confirm);
    clear_buffer();

    if (confirm != 'y' && confirm != 'Y') {
        printf("Deletion cancelled.\n");
        return;
    }

    /* Close the gap: copy every employee after 'index' one place to the left (a whole struct is copied at once),
       then reduce the count so the last, now duplicate, slot is ignored. */
    for (i = index; i < employee_count - 1; i++) {
        emp[i] = emp[i + 1];
    }

    employee_count--;

    savedata();

    printf("Employee deleted successfully.\n");
}

/* =====================================================================
                            UPDATE EMPLOYEE
===================================================================== */
/* Every new value is read into a temporary variable and checked FIRST.
   emp[index] is only changed after the value is valid, so a wrong entry
   can never end up in memory (or in the file). */

void updateEmployee(void) {
    char search_id[20];
    char temp_text[100];
    char temp_bank[50];
    char temp_acc[50];
    float amount;
    int index;
    int choice;

    printf("Enter employee ID to update: ");
    scanf("%19s", search_id);
    clear_buffer();

    index = search(search_id);

    if (index == -1) {
        printf("Updation cancelled.\n");
        return;
    }

    do {
        printf("\n=== Update Fields for %s ===\n", emp[index].name);
        printf("1. Update Designation\n");
        printf("2. Update Department\n");
        printf("3. Update Email\n");
        printf("4. Update Bank Name (and Account Number)\n");
        printf("5. Update Bank Account Number\n");
        printf("6. Update Basic Salary\n");
        printf("7. Update Allowance\n");
        printf("8. Update Bonus\n");
        printf("9. Update Overtime Pay\n");
        printf("Choose a field to update (1-9), press 0 to exit: ");

        if (scanf("%d", &choice) != 1) {
            clear_buffer();
            choice = -1;
        } else {
            clear_buffer();
        }

        switch (choice) {
            case 1:
                printf("\nEnter Employee Designation: ");
                scanf(" %99[^\n]", temp_text);
                clear_buffer();

                if (validate_designation(temp_text) == -1) {
                    printf("Invalid designation entered! No changes were made (Use letters only).\n");
                    break;
                }
                strcpy(emp[index].designation, temp_text);
                savedata();
                printf("Designation updated and saved successfully!\n");
                break;

            case 2:
                printf("\nEnter Employee Department: ");
                scanf(" %99[^\n]", temp_text);
                clear_buffer();

                if (validate_department(temp_text) == -1) {
                    printf("Invalid department entered! No changes were made (Use letters only).\n");
                    break;
                }
                strcpy(emp[index].department, temp_text);
                savedata();
                printf("Department updated and saved successfully!\n");
                break;

            case 3:
                printf("\nEnter Employee Email: ");
                scanf(" %99[^\n]", temp_text);
                clear_buffer();

                if (validate_email(temp_text) == -1) {
                    printf("Invalid Email entered! No changes were made (Wrong format).\n");
                    break;
                }
                strcpy(emp[index].email, temp_text);
                savedata();
                printf("Email updated and saved successfully!\n");
                break;

            case 4:
                printf("\nEnter Employee Bank Name: ");
                scanf(" %49[^\n]", temp_bank);
                clear_buffer();

                if (validate_bank_name(temp_bank) == -1) {
                    printf("Invalid bank name! Bank name consists of letters only. No changes made.\n");
                    break;
                }

                printf("\nEnter Employee Bank Account Number: ");
                scanf(" %49[^\n]", temp_acc);
                clear_buffer();

                if (validate_bank_account(temp_acc) == -1) {
                    printf("Invalid account number! Use 8 to 34 letters/digits. No changes made.\n");
                    break;
                }

                /* Both validated successfully, now update memory and save */
                strcpy(emp[index].bank_name, temp_bank);
                strcpy(emp[index].bank_account, temp_acc);
                savedata();
                printf("Bank Name and Account number updated and saved successfully!\n");
                break;

            case 5:
                printf("\nEnter Employee Bank Account Number: ");
                scanf(" %49[^\n]", temp_acc);
                clear_buffer();

                if (validate_bank_account(temp_acc) == -1) {
                    printf("Invalid account number! Use 8 to 34 letters/digits, no spaces or special characters.\n");
                    break;
                }
                strcpy(emp[index].bank_account, temp_acc);
                savedata();
                printf("Bank Account Number updated and saved successfully!\n");
                break;

            case 6:
                printf("Enter new Basic Salary (in PKR): ");
                if (read_float(&amount) == 0 && validate_basic_salary(amount) == 0) {
                    emp[index].basic_salary = amount;
                    emp[index].taxes = emp[index].basic_salary * TAX_RATE;
                    calculate_net_salary(&emp[index]);

                    savedata();
                    printf("Salary updated and saved successfully!\n");
                } else {
                    printf("Invalid salary entered! No changes were made.\n");
                }
                break;

            case 7:
                printf("\nEnter new Allowance (in PKR): ");
                if (read_float(&amount) == 0 && validate_allowance(amount) == 0) {
                    emp[index].allowance = amount;
                    calculate_net_salary(&emp[index]);

                    savedata();
                    printf("Allowance updated and saved successfully!\n");
                } else {
                    printf("Invalid Allowance entered! No changes were made.\n");
                }
                break;

            case 8:
                printf("\nEnter new Bonus amount (in PKR): ");
                if (read_float(&amount) == 0 && validate_bonuses(amount) == 0) {
                    emp[index].bonuses = amount;
                    calculate_net_salary(&emp[index]);

                    savedata();
                    printf("Bonus updated and saved successfully!\n");
                } else {
                    printf("Invalid Bonus entered! No changes were made.\n");
                }
                break;

            case 9:
                printf("\nEnter Overtime Pay (in PKR): ");
                if (read_float(&amount) == 0 && validate_overtime(amount) == 0) {
                    emp[index].overtime_pay = amount;
                    calculate_net_salary(&emp[index]);

                    savedata();
                    printf("Overtime updated and saved successfully!\n");
                } else {
                    printf("Invalid Overtime pay entered! No changes were made.\n");
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

/* =====================================================================
                            DISPLAY ALL RECORDS
===================================================================== */
/* One employee per row, 14 columns. The table is TABLE_WIDTH (190) characters wide,
   so the console window must be at least that wide or the rows will wrap.
   printf formats used below:
     %-16s   text, left-aligned in 16 characters (used for the column titles)
     %-16.16s same, but also CUTS longer text to 16 characters so it can never push the next column away
     %12.2f   number with 2 decimals, right-aligned in 12 characters (right-aligned numbers line up)
   Every column is separated by 2 spaces. */

void displayAllRecords(void) {
    int i;

    if (employee_count == 0) {
        printf("\nNo employee records found.\n");} 
    else {
        printf("\n");
        printLine(TABLE_WIDTH);

        printf("%-16s %-8s %-12s %-10s %-20s %-15s %-12s %-14s %12s %9s %12s %9s %9s %10s\n",
            "NAME", "ID", "ROLE", "DEPARTMENT", "EMAIL", "DATE OF JOINING",
            "BANK NAME", "BANK ACCOUNT", "BASIC SALARY", "ALLOWANCE",
            "OVERTIME PAY", "TAXES", "BONUSES", "NET SALARY");

        printLine(TABLE_WIDTH);

        for (i = 0; i < employee_count; i++) {
            printf("%-16.16s %-8.8s %-12.12s %-10.10s %-20.20s %-15.15s %-12.12s %-14.14s %12.2f %9.2f %12.2f %9.2f %9.2f %10.2f\n",
                emp[i].name, emp[i].id, emp[i].designation, emp[i].department,
                emp[i].email, emp[i].date_joining, emp[i].bank_name, emp[i].bank_account,
                emp[i].basic_salary, emp[i].allowance, emp[i].overtime_pay,
                emp[i].taxes, emp[i].bonuses, emp[i].net_salary);}

        printLine(TABLE_WIDTH);
        printf("Total employees: %d\n", employee_count);}
}

/* =====================================================================
                            GENERATE PAYSLIP
===================================================================== */

/* Asks for an employee ID and prints a formatted payslip: employee details, earnings, deductions and net salary. */
void generatePayslip(void) {
    char id[20];
    char period[30];
    int index, len;
    float gross, net;
    Employee *e;
    time_t now = time(NULL);

    printf("Enter employee ID for the payslip: ");
    scanf(" %19[^\n]", id);
    clear_buffer();
    index = search(id);
    if (index == -1) {
        printf("\nEmployee not found.\n");
        return;}
    e = &emp[index];

    /* strftime turns the current date into text: %B = month name, %Y = year (e.g. "November 2026") */
    strftime(period, sizeof(period), "%B %Y", localtime(&now));

    gross = e->basic_salary + e->allowance + e->overtime_pay + e->bonuses;
    net = gross - e->taxes;
    len = (int)strlen(e->bank_account);

    printf("\n");
    printLine(48);
    /* %*s takes its width from the next argument: 24 + half the text length centres the month in the 48-wide slip */
    printf("%27s\n%*s\n", "PAYSLIP", 24 + (int)strlen(period) / 2, period);
    printLine(48);
    printf("%-16s: %s\n", "Employee ID", e->id);
    printf("%-16s: %s\n", "Name", e->name);
    printf("%-16s: %s\n", "Designation", e->designation);
    printf("%-16s: %s\n", "Department", e->department);
    printf("%-16s: %s\n", "Joining Date", e->date_joining);
    printf("%-16s: %s\n", "Bank", e->bank_name);

    /* Privacy: show only the last 4 characters of the account number (e->bank_account + len - 4 starts 4 from the end) */
    if (len > 4) {
        printf("%-16s: ****%s\n", "Account", e->bank_account + len - 4);
    } else {
        printf("%-16s: %s\n", "Account", e->bank_account);}
    printLine(48);
    printf("EARNINGS\n");
    printf("  %-28s %14.2f\n", "Basic Salary", e->basic_salary);
    printf("  %-28s %14.2f\n", "Allowance", e->allowance);
    printf("  %-28s %14.2f\n", "Overtime Pay", e->overtime_pay);
    printf("  %-28s %14.2f\n", "Bonus", e->bonuses);
    printf("  %-28s %14.2f\n", "Gross Salary", gross);
    printLine(48);
    printf("DEDUCTIONS\n");
    printf("  %-28s %14.2f\n", "Taxes", e->taxes);
    printLine(48);
    printf("%-30s %14.2f\n", "NET SALARY", net);
    printLine(48);
}

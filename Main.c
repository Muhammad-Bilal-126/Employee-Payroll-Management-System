#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "employee.h"

#include <time.h>
#include "validation.h" //INPUT VALIDATORS (returns -1 for an invalid and 0 for a valid input.)  NOTE: it recieves input as an argument from input() function.
#include "helper.h"

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

/* Login details (hard-coded for this project; real software would never store a password in the source code). */
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "1234"
#define MAX_LOGIN_ATTEMPTS 3

/* Global variables */
Employee emp[Max_Employee];
int employee_count = 0;

/* Function prototypes */
int search(char arr[]);                             // searches for an employee in memory, prints a message if not found, returns its index (or -1).

void display(char arr[]);                           // displays an employess data.
void updateEmployee(void);                          // update an employees certain record field like email, department, etc.
void deleteEmployee(void);                          // delete an employee record from file.
int input(void);                                   // take valid input and return to add_employee to save in the file
int add_employee(void);                             // appends the new employee to the file; memory is updated accordingly.
void displayAllRecords(void);                       // displays all records, one employee per row.
void generatePayslip(void);                         // generates pay slip for an employee, based on his id.
int login(void);                                    // asks for username and password (3 attempts). returns 1 if correct, 0 if not.


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
            printf("Invalid ID! An ID can have letters and digits only.\n");
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
                            SEARCH / DISPLAY
===================================================================== */


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

    if (savedata() == 1) {
        printf("Employee deleted successfully.\n");
    } else {
        printf("Error occurred while deleting employee.\n");
    }
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
                if (savedata() == 1) {
                    printf("Designation updated and saved successfully!\n");
                } else {
                    printf("Error occurred while updating designation.\n");
                }
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
                if (savedata() == 1) {
                    printf("Department updated and saved successfully!\n");
                } else {
                    printf("Error occurred while updating department.\n");
                }
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
                if (savedata() == 1) {
                    printf("Email updated and saved successfully!\n");
                } else {
                    printf("Error occurred while updating email.\n");
                }
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
                if (savedata() == 1) {
                    printf("Bank Name and Account number updated and saved successfully!\n");
                } else {
                    printf("Error occurred while updating bank information.\n");
                }
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
                if (savedata() == 1) {
                    printf("Bank Account Number updated and saved successfully!\n");
                } else {
                    printf("Error occurred while updating bank account number.\n");
                }
                break;

            case 6:
                printf("Enter new Basic Salary (in PKR): ");
                if (read_float(&amount) == 0 && validate_basic_salary(amount) == 0) {
                    emp[index].basic_salary = amount;
                    emp[index].taxes = emp[index].basic_salary * TAX_RATE;
                    calculate_net_salary(&emp[index]);

                    if (savedata() == 1) {
                        printf("Salary updated and saved successfully!\n");
                    } else {
                        printf("Error occurred while updating salary.\n");
                    }
                } else {
                    printf("Invalid salary entered! No changes were made.\n");
                }
                break;

            case 7:
                printf("\nEnter new Allowance (in PKR): ");
                if (read_float(&amount) == 0 && validate_allowance(amount) == 0) {
                    emp[index].allowance = amount;
                    calculate_net_salary(&emp[index]);

                    if (savedata() == 1) {
                        printf("Allowance updated and saved successfully!\n");
                    } else {
                        printf("Error occurred while updating allowance.\n");
                    }
                } else {
                    printf("Invalid Allowance entered! No changes were made.\n");
                }
                break;

            case 8:
                printf("\nEnter new Bonus amount (in PKR): ");
                if (read_float(&amount) == 0 && validate_bonuses(amount) == 0) {
                    emp[index].bonuses = amount;
                    calculate_net_salary(&emp[index]);

                    if (savedata() == 1) {
                        printf("Bonus updated and saved successfully!\n");
                    } else {
                        printf("Error occurred while updating bonus.\n");
                    }
                } else {
                    printf("Invalid Bonus entered! No changes were made.\n");
                }
                break;

            case 9:
                printf("\nEnter Overtime Pay (in PKR): ");
                if (read_float(&amount) == 0 && validate_overtime(amount) == 0) {
                    emp[index].overtime_pay = amount;
                    calculate_net_salary(&emp[index]);

                    if (savedata() == 1) {
                        printf("Overtime updated and saved successfully!\n");
                    } else {
                        printf("Error occurred while updating overtime pay.\n");
                    }
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
            "NAME", "ID", "DESIGNATION", "DEPARTMENT", "EMAIL", "DATE OF JOINING",
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

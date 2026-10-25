
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EMPLOYEES 500
#define FILE_NAME "employees.dat"

struct Employee {
    int id;
    char name[50];
    char department[50];
    double basicSalary;
    double allowance;
    double deduction;
};

struct Employee employees[MAX_EMPLOYEES];
int employeeCount = 0;

/* Read a line safely and remove the newline */
void readLine(char *text, int size) {
    if (fgets(text, size, stdin) != NULL) {
        size_t len = strlen(text);

        if (len > 0 && text[len - 1] == '\n') {
            text[len - 1] = '\0';
        } else {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
    }
}

/* Validate integer input */
int getInt(const char *message, int min, int max) {
    char input[100];
    char *end;
    long value;

    while (1) {
        printf("%s", message);
        readLine(input, sizeof(input));

        value = strtol(input, &end, 10);

        while (isspace((unsigned char)*end)) end++;

        if (input[0] != '\0' && end != input &&
            *end == '\0' && value >= min && value <= max) {
            return (int)value;
        }

        printf("Invalid input. Try again.\n");
    }
}

/* Validate non-negative salary amounts */
double getAmount(const char *message) {
    char input[100];
    char *end;
    double value;

    while (1) {
        printf("%s", message);
        readLine(input, sizeof(input));

        value = strtod(input, &end);

        while (isspace((unsigned char)*end)) end++;

        if (input[0] != '\0' && end != input &&
            *end == '\0' && value >= 0.0 &&
            value <= 1000000000000.0) {
            return value;
        }

        printf("Enter a valid non-negative amount.\n");
    }
}

/* Find employee using ID */
int findEmployee(int id) {
    int i;

    for (i = 0; i < employeeCount; i++) {
        if (employees[i].id == id)
            return i;
    }

    return -1;
}

/* Save records to a file */
void saveData(void) {
    FILE *file = fopen(FILE_NAME, "wb");

    if (file == NULL) {
        printf("Error saving employee records.\n");
        return;
    }

    if (fwrite(&employeeCount, sizeof(int), 1, file) != 1 ||
        fwrite(employees, sizeof(struct Employee),
               employeeCount, file) != (size_t)employeeCount) {
        printf("Error writing data to file.\n");
    } else {
        printf("Records saved successfully.\n");
    }

    fclose(file);
}

/* Load records when the program starts */
void loadData(void) {
    FILE *file = fopen(FILE_NAME, "rb");
    int count;

    if (file == NULL)
        return;

    if (fread(&count, sizeof(int), 1, file) != 1 ||
        count < 0 || count > MAX_EMPLOYEES) {
        printf("Invalid data file. Starting with empty records.\n");
        fclose(file);
        return;
    }

    if (fread(employees, sizeof(struct Employee),
              count, file) != (size_t)count) {
        printf("Could not read saved records.\n");
        employeeCount = 0;
        fclose(file);
        return;
    }

    employeeCount = count;
    fclose(file);

    printf("%d employee record(s) loaded.\n", employeeCount);
}

/* Calculate net salary */
double netSalary(struct Employee *e) {
    return e->basicSalary + e->allowance - e->deduction;
}

/* Display one employee's payroll */
void displayEmployee(struct Employee *e) {
    printf("\nEmployee ID   : %d\n", e->id);
    printf("Name          : %s\n", e->name);
    printf("Department    : %s\n", e->department);
    printf("Basic Salary  : %.2f\n", e->basicSalary);
    printf("Allowance     : %.2f\n", e->allowance);
    printf("Deduction     : %.2f\n", e->deduction);
    printf("Net Salary    : %.2f\n", netSalary(e));
}

/* Add a new employee */
void addEmployee(void) {
    struct Employee e;
    int id;

    if (employeeCount >= MAX_EMPLOYEES) {
        printf("Employee record limit reached.\n");
        return;
    }

    id = getInt("Enter employee ID: ", 1, 2147483647);

    if (findEmployee(id) != -1) {
        printf("This employee ID already exists.\n");
        return;
    }

    e.id = id;

    do {
        printf("Enter employee name: ");
        readLine(e.name, sizeof(e.name));
        if (strlen(e.name) == 0)
            printf("Name cannot be empty.\n");
    } while (strlen(e.name) == 0);

    do {
        printf("Enter department: ");
        readLine(e.department, sizeof(e.department));
        if (strlen(e.department) == 0)
            printf("Department cannot be empty.\n");
    } while (strlen(e.department) == 0);

    e.basicSalary = getAmount("Enter basic salary: ");
    e.allowance = getAmount("Enter allowance: ");
    e.deduction = getAmount("Enter deduction: ");

    if (e.deduction > e.basicSalary + e.allowance) {
        printf("Deduction cannot exceed gross salary.\n");
        return;
    }

    employees[employeeCount] = e;
    employeeCount++;

    saveData();
    printf("Employee added successfully.\n");
}

/* Display all employee records */
void displayAll(void) {
    int i;

    if (employeeCount == 0) {
        printf("No employee records found.\n");
        return;
    }

    printf("\n%-8s %-20s %-15s %12s %12s %12s %12s\n",
           "ID", "Name", "Department", "Basic",
           "Allowance", "Deduction", "Net Salary");

    printf("-------------------------------------------------------------------------------\n");

    for (i = 0; i < employeeCount; i++) {
        printf("%-8d %-20.20s %-15.15s %12.2f %12.2f %12.2f %12.2f\n",
               employees[i].id,
               employees[i].name,
               employees[i].department,
               employees[i].basicSalary,
               employees[i].allowance,
               employees[i].deduction,
               netSalary(&employees[i]));
    }
}

/* Search for an employee */
void searchEmployee(void) {
    int id, index;

    id = getInt("Enter employee ID to search: ", 1, 2147483647);
    index = findEmployee(id);

    if (index == -1) {
        printf("Employee not found.\n");
    } else {
        displayEmployee(&employees[index]);
    }
}

/* Update an existing employee */
void updateEmployee(void) {
    int id, index;
    struct Employee *e;

    id = getInt("Enter employee ID to update: ", 1, 2147483647);
    index = findEmployee(id);

    if (index == -1) {
        printf("Employee not found.\n");
        return;
    }

    e = &employees[index];

    printf("Enter new details:\n");

    do {
        printf("Enter name: ");
        readLine(e->name, sizeof(e->name));
        if (strlen(e->name) == 0)
            printf("Name cannot be empty.\n");
    } while (strlen(e->name) == 0);

    do {
        printf("Enter department: ");
        readLine(e->department, sizeof(e->department));
        if (strlen(e->department) == 0)
            printf("Department cannot be empty.\n");
    } while (strlen(e->department) == 0);

    e->basicSalary = getAmount("Enter basic salary: ");
    e->allowance = getAmount("Enter allowance: ");

    do {
        e->deduction = getAmount("Enter deduction: ");

        if (e->deduction > e->basicSalary + e->allowance)
            printf("Deduction cannot exceed gross salary.\n");

    } while (e->deduction > e->basicSalary + e->allowance);

    saveData();
    printf("Employee updated successfully.\n");
}

/* Delete an employee */
void deleteEmployee(void) {
    int id, index, i;

    id = getInt("Enter employee ID to delete: ", 1, 2147483647);
    index = findEmployee(id);

    if (index == -1) {
        printf("Employee not found.\n");
        return;
    }

    for (i = index; i < employeeCount - 1; i++) {
        employees[i] = employees[i + 1];
    }

    employeeCount--;
    saveData();

    printf("Employee deleted successfully.\n");
}

/* Generate payroll summary */
void payrollSummary(void) {
    int i;
    double totalGross = 0, totalDeduction = 0, totalNet = 0;

    if (employeeCount == 0) {
        printf("No employee records available.\n");
        return;
    }

    for (i = 0; i < employeeCount; i++) {
        totalGross += employees[i].basicSalary +
                      employees[i].allowance;
        totalDeduction += employees[i].deduction;
        totalNet += netSalary(&employees[i]);
    }

    printf("\n========== PAYROLL SUMMARY ==========\n");
    printf("Total Employees : %d\n", employeeCount);
    printf("Total Gross Pay : %.2f\n", totalGross);
    printf("Total Deductions: %.2f\n", totalDeduction);
    printf("Total Net Payroll: %.2f\n", totalNet);
    printf("=====================================\n");
}

/* Main menu */
int main(void) {
    int choice;

    loadData();

    do {
        printf("\n===== EMPLOYEE PAYROLL MANAGEMENT =====\n");
        printf("1. Add Employee\n");
        printf("2. Display All Employees\n");
        printf("3. Search Employee\n");
        printf("4. Update Employee\n");
        printf("5. Delete Employee\n");
        printf("6. Payroll Summary\n");
        printf("7. Save Records\n");
        printf("0. Exit\n");

        choice = getInt("Enter your choice: ", 0, 7);

        switch (choice) {
            case 1:
                addEmployee();
                break;
            case 2:
                displayAll();
                break;
            case 3:
                searchEmployee();
                break;
            case 4:
                updateEmployee();
                break;
            case 5:
                deleteEmployee();
                break;
            case 6:
                payrollSummary();
                break;
            case 7:
                saveData();
                break;
            case 0:
                saveData();
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 0);

    return 0;
}

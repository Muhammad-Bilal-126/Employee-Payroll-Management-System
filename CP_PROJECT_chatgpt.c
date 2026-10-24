#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_EMP 100
#define FILENAME "payroll.txt"

// 1. STRUCTURE - Covers Criterion 2 (Structure)
struct Employee {
    int id;
    char name[50];
    char department[30];
    char designation[30];
    float basicSalary;
    float houseAllowance;
    float medicalAllowance;
    float tax;
    float netSalary; // Calculated field
};

struct Employee emp[MAX_EMP];
int count = 0;

// Function Prototypes - Modular Design (Criterion 1)
void addEmployee();
void displayAll();
void searchEmployee();
void updateEmployee();
void deleteEmployee();
void saveToFile();
void loadFromFile();
void generateReport();
void calculateSalary(struct Employee *e); // Pointer usage
void clearInput();

int main() {
    int choice;
    loadFromFile(); // Load data on start

    while(1) {
        printf("\n===== EMPLOYEE PAYROLL SYSTEM =====\n");
        printf("1. Add Employee\n");
        printf("2. Display All Employees\n");
        printf("3. Search Employee\n");
        printf("4. Update Employee\n");
        printf("5. Delete Employee\n");
        printf("6. Generate Salary Report (Sort by Net Salary)\n");
        printf("7. Save & Exit\n");
        printf("Enter choice: ");
        if(scanf("%d", &choice) != 1) {
            printf("Invalid input! Numbers only.\n");
            clearInput();
            continue;
        }

        switch(choice) {
            case 1: addEmployee(); break;
            case 2: displayAll(); break;
            case 3: searchEmployee(); break;
            case 4: updateEmployee(); break;
            case 5: deleteEmployee(); break;
            case 6: generateReport(); break;
            case 7: saveToFile(); printf("Data saved. Goodbye!\n"); return 0;
            default: printf("Invalid choice. Try again.\n");
        }
    }
}

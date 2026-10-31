#include<stdio.h>
#include<string.h>
/*
functions list:
1) aik functions sarri details ko user se input lene ke liye --> Shehryar
2) aik function add employee record ke liye jisme woh file open ho, 
   input lene ka function call ho, woh data file mein append ho (append karna hai write nhi) 
   aur file save hokar close hogaye with data validation --> Shehryar
3) Aik function employee ko search karne ka based on employee id ---> Bilal
4) Aik function sarre employees ko display karne ke liye ---> Qaim
5) Aik function kisi particular employee ka record delete karne ke liye based on employee id --->Amin
6) Aik function payslip generate karne ke liye --->Qaim
7) Aik function kisi particular employee ka record update karne ke liye, 
jis employee ka record update karna hoga user iski id dega --> Amin
8) Exit ka code direct main mein likh deingy ---> Qaim
*/
#define Max_Employee 100
typedef struct{
    char name[100];
    char id[20];
    char designation[100];
    char department[100];
    char email[100];
    char date_joining[10];
    float experience;
    char bank_name[50];
    char bank_account[50];
    float basic_salary;
    float allowance;
    float overtime_pay;
    float bonuses;
    float taxes;
    float net_salary;
}Employee;
    Employee emp[Max_Employee];
    int employee_count=0;
    int search(char arr[]);
    void display(int i);
    void updateEmployee(void);
   void deleteEmployee(void);

int main(){
    int option;
    do {
    printf("%110s\n","=== Employee Payroll Management System ===");
    printf("\n%70s\n","1-Add a new employee");
    printf("\n%80s\n","2-Display all employee records");
    printf("\n%79s\n","3-Update an employee's record");
    printf("\n%77s\n","4-Delete an employee record");
    printf("\n%81s\n","5-Search for an employee record");
    printf("\n%84s\n","6-Generate payslip for an employee");
    printf("\n%83s\n","7-Exit from the management system");
    printf("\n%81s","Which Option Do You Want to Choose: ");
    scanf("%d",&option);
    //Add login ability then show menu
    switch (option)
    {
        case (1): //Add a new employee's record
        {
          break;
        }
        case (2): //Display all employee records
        {
         break;
        }
        case (3): //Update an employee's record
        {
             updateEmployee();
          break;
        }
        case (4): //Delete an employee's record
        {
            deleteEmployee();
         break;
        }
        case (5): //Search for a particular employee
        {
            char employid[20];
            printf("Enter the employ id to search\n");
            scanf("%s",employid);
            search(employid);
          break;
        }
        case (6): //Generate a payslip for a particular employee 
        {  
          break;
        }
        case (7): //Exit from management system
        {
            printf("\n Exiting From Management System Program");
            break;
        }
        default:
           printf("\n Invalid Input");
    }
}while(option!=7);

    return 0;
}



void input(void)
{
    printf("Enter The Following Detail\n");
    printf("Name:");
    printf("\nID:");
    printf("\nRole:");
    printf("\nDepartment:");
    printf("\nEmail:");
    printf("\nJoining Date:");
    printf("\nExperience:");
    printf("\nBank Name:");
    printf("\nBank Account");
    printf("\nBasic Salary:");
    printf("\nAllowance:");
    printf("\nOvertime Pay:");
    printf("\nBonus:");
    printf("\nTaxes:");


}
 int search(char employid[])
{
    int i;
    for (i=0; i < employee_count; i++)
    {
         if( strcmp( emp[i].id, employid) == 0)
         {
            display(i);
            return i;
         }
         else
         {
            printf("Wrong ID Data Not Found\n");
            return -1;
         }
    }
}

void display(int i)
{
    
        printf("Name:%s",emp[i].name);
        printf("\nID:%s",emp[i].id);
        printf("\nDesignation:%s",emp[i].designation);
        printf("\nDepartment:%s",emp[i].department);
        printf("\nEmail:%s",emp[i].email);
        printf("\nJoining Date:%s",emp[i].date_joining);
        printf("\nExperience:%f",emp[i].experience);
        printf("\nBank Name:%s",emp[i].bank_name);
        printf("\nBank Account:%s",emp[i].bank_account);
        printf("\nBasic Salary:%f",emp[i].basic_salary);
        printf("\nAllowance:%f",emp[i].allowance);
        printf("\nOvertime Pay:%f",emp[i].overtime_pay);
        printf("\nBonus:%f",emp[i].bonuses);
        printf("\nTaxes:%f",emp[i].taxes);
        printf("Final Salary:%f",emp[i].net_salary);
}

void deleteEmployee(void) {
    char delete_id[20];
    int index = -1;
    printf("Enter employee ID to delete: ");
    scanf("%19s", delete_id);
    while (getchar() != '\n'); 

    index = search(delete_id); 

    if (index == -1) {
        printf("Employee not found. Deletion Cancelled.\n");
        return;
    }
    char confirm;
    printf("\nAre you sure you want to permanently delete this employee? (y/n): ");
    scanf(" %c", &confirm);
    if (confirm != 'y' && confirm != 'Y') {
        printf("Deletion cancelled.\n");
        return;
    }

    for (int i = index; i < employee_count - 1; i++) {
        emp[i] = emp[i + 1];
    }

    employee_count--;
    saveData(); 
    printf("Employee deleted successfully.\n");
}

void updateEmployee(void) {
    char search_id[20];
    int index = -1;
    int choice;

    printf("Enter employee ID to update: ");
    scanf("%19s", search_id);
    while (getchar() != '\n');

    index = search(search_id);

    if (index == -1) {
        printf("Employee not found. Updation Cancelled.\n");
        return;
    }
do {
    printf("\n=== Update Fields for %s ===", emp[index].name);
    printf("\n1. Update Designation");
    printf("\n2. Update Department");
    printf("\n3. Update Email");
    printf("\n4. Update Experience (Years)");
    printf("\n5. Update Basic Salary");
    printf("\nChoose a field to update (1-5) press '0' to exit: ");
    scanf("%d", &choice);
    while (getchar() != '\n');

    switch (choice) {
        case 1:
        {
            printf("Enter new Designation: ");
            scanf("%99[^\n]", emp[index].designation);
            saveData();
            printf("Designation updated and saved successfully!\n");
            break; }
        case 2:
        {
            printf("Enter new Department: ");
            scanf("%99[^\n]", emp[index].department);
            saveData();
            printf("Department updated and saved successfully!\n");
            break; }
        case 3:
        {
            printf("Enter new Email: ");
            scanf("%99s", emp[index].email);
            saveData();
            printf("Email updated and saved successfully!\n");
            break; }
        case 4:
        {
            printf("Enter updated Experience (years): ");
            scanf("%f", &emp[index].experience);
            saveData();
            printf("Experience updated and saved successfully!\n");
            break; }
        case 5:
        {
            printf("Enter new Basic Salary: ");
            scanf("%f", &emp[index].basic_salary);
            emp[index].net_salary = emp[index].basic_salary + emp[index].allowance + emp[index].bonuses + emp[index].overtime_pay - emp[index].taxes;
            saveData();
            printf("Salary updated and saved successfully!\n");
            break; }
            case 0:
            {
            printf("Exiting Update Menu");
            break; }
        default:
            printf("Invalid choice! No changes were made.\n");
    }
}while(choice!= 0);
}
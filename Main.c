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
    char role[100];
    char department[100];
    char email[100];
    char date_joining[10];
    
    char bank_account[50];
    float basic_salary;
    float allowance;
    float overtime_pay;
    float bonuses;
    float taxes;
    float net_salary;
}Employee;
int main(){
    Employee emp[Max_Employee];
    int employee_count=0;
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
          break;
        }
        case (4): //Delete an employee's record
        {
         break;
        }
        case (5): //Search for a particular employee
        {
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
#include<stdio.h>
#include<string.h>
/*
functions list:
1) aik functions sarri details ko user se input lene ke liye
2) aik function add employee record ke liye jisme woh file open ho, input lene ka function call ho, woh data file mein append ho (append karna hai write nhi) 
   aur file save hokar close hogaye with data validation
3) Aik function employee ko search karne ka based on employee id
4) Aik function sarre employees ko display karne ke liye
5) Aik function kisi particular employee ka record delete karne ke liye based on employee id
6) Aik function payslip generate karne ke liye
7) Aik function kisi particular employee ka record update karne ke liye, jis employee ka record update karna hoga user iski id dega
8) Exit ka code direct main mein likh deingy
Iske illawa kuch add karna ho toh tuesday ko lab ke baad discuss karleingy 
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
int main(){
    Employee emp[Max_Employee];
    int employee_count=0;
    int option;
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
    while(1)
    {   
        if(!(option>=1 && option <= 7))
        {
        printf("\nInvalid Input. Try Again\n");
        continue;
        }
        else if(option == 1) //Add a new employee's record
        {

        }
        else if(option == 2) //Display all employee records
        {

        }
        else if(option == 3) //Update an employee's record
        {

        }
        else if(option == 4) //Delete an employee's record
        {

        }
        else if(option == 5) //Search for a particular employee
        {
            char employid[20];
            printf("Enter the employ id to search\n");
            scanf("%s",employid[]);
            search(employid[])


        }
        else if(option == 6) //Generate a payslip for a particular employee 
        {

        }
        else if(option == 7) //Exit from management system
        {
            break;
        }
    }
    return 0;
}

//TRY


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
        printf("\nBonus:%f",emp[i].bonus);
        printf("\nTaxes:%f",emp[i].taxes);
        printf("Final Salary:%f",emp[i].net_salary);
}

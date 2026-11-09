#include<stdio.h>
#include<string.h>
#include <ctype.h>
#include<math.h>
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
    char date_joining[20];
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
    // GLOBAL VARIABLES.
    Employee emp[Max_Employee];
    int employee_count = 0;

    int search(char arr[]);
    void display(int i);
    void updateEmployee(void);
    void deleteEmployee(void);
    void savedata();
    void input(void);
    int add_employee (void);

    // ----------- input helpers -----------------

    void clear_buffer(void);
    int read_float(float *value);
    int only_letters_and_spaces(const char *text);

    // ----------- field validators: each returns 0 if input is valid, -1 if invalid -------------
    int validate_name(const char *name);
    int validate_id(const char *id);
    int validate_designation(const char *designation);
    int validate_department(const char *department);
    int validate_email(const char *email);
    int validate_experience(float years);
    int validate_bank_name(const char *bank_name);
    int validate_bank_account(const char *account);
    int validate_basic_salary(float amount);
    int validate_allowance(float amount);
    int validate_overtime(float amount);
    int validate_bonuses(float amount);
    int validate_taxes(float taxes, float gross_salary);

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

        // fetching no. of employees from file directly.

        FILE *file;                                                           
        file = fopen ("values.txt","r");         
        if (file != NULL) {
            fscanf(file, "%d", &employee_count);
            fclose(file);
        }

        printf("Total no. of employees are: %d\n", employee_count);

        //Add login ability then show menu
        switch (option)
        {
            case (1):{                          //Add a new employee's record
                add_employee ();
                break;
            }
            case (2):{                          //Display all employee records
                break;
            }
            case (3): {                         //Update an employee's record
                updateEmployee();
                break;
            }
            case (4):{                          //Delete an employee's record
                deleteEmployee();
                break;
            }
            case (5):{                          //Search for a particular employee
                char employid[20];
                printf("Enter the employ id to search\n");
                scanf(" %s",employid);
                clear_buffer();
                search(employid);
                break;
            }
            case (6):{                           //Generate a payslip for a particular employee  
                break;
            }
            case (7):{                           //Exit from management system
                printf("\n Exiting From Management System Program");
                break;
            }
            default:
            printf("\n Invalid Option Chosen!");
        }

    }while(option!=7);
    return 0;
}

// =====================================================================================================
//                                              INPUT
// =====================================================================================================
/*
   Every field follows the same pattern:
    takes the input ----------> validates it -----------> if invalid retakes input (status = -1)
    continues until a valid input is given (status = 0)    
*/

void input(void){
    Employee *e = &emp[employee_count];     // shortcut to the record currently being filled
    int status;                             // indicates if input is valid or not (-1 for invalid, 0 for valid)

    printf("Enter The Details as prompted\n");

    // ---------------------------------------------- Name ------------------------------------------------------
    do {
        printf("\nName as per CNIC: ");
        scanf(" %99[^\n]", e->name);        // %99 = max 99 characters (leave last index is for '\0'). [^\n] also reads spaces until enter.
        clear_buffer();                     // throw away anything extra typed beyond the limit, so it doesn't leak into the next scanf.
        status = validate_name(e->name);
        if (status == -1)
            printf("Invalid name! Use letters and spaces only.\n");
    } while (status == -1);

    // ----------------------------------------------- ID -----------------------------------------------
    do {
        printf("\nID: ");
        scanf(" %19[^\n]", e->id);
        clear_buffer();
        status = validate_id(e->id);
        if (status == -1)
            printf("Invalid ID! Use letters, digits and '-' only (no spaces).\n");
    } while (status == -1);

    // ------------------------------------------- Designation ------------------------------------------
    do {
        printf("\nEmployee Designation: ");
        scanf(" %99[^\n]", e->designation);
        clear_buffer();
        status = validate_designation(e->designation);
        if (status == -1)
            printf("Invalid designation! Use letters and spaces only.\n");
    } while (status == -1);

    // -------------------------------------------- Department ------------------------------------------
    do {
        printf("\nDepartment: ");
        scanf(" %99[^\n]", e->department);
        clear_buffer();
        status = validate_department(e->department);
        if (status == -1)
            printf("Invalid department! Use letters and spaces only.\n");
    } while (status == -1);

    // ---------------------------------------------- Email ---------------------------------------------
    do {
        printf("\nEmail: ");
        scanf(" %99[^\n]", e->email);
        clear_buffer();
        status = validate_email(e->email);
        if (status == -1)
            printf("Invalid email! Example of a valid format: name@example.com\n");
    } while (status == -1);

    // ------------------------------------------- Joining Date -----------------------------------------
    // (not validated)
    printf("\nJoining Date strictly in format (dd/mm/yy): ");
    scanf(" %19[^\n]", e->date_joining);
    clear_buffer();

    // ------------------------------------------- Experience -------------------------------------------
    do {
        printf("\nExperience (in years): ");
        status = read_float(&e->experience);                       // -1 if the user typed something that isn't a number
        if (status == 0)
            status = validate_experience(e->experience);
        if (status == -1)
            printf("Invalid experience! Enter a number between 0 and 60.\n");
    } while (status == -1);

    // ------------------------------------------- Bank Name --------------------------------------------
    do {
        printf("\nBank Name: ");
        scanf(" %49[^\n]", e->bank_name);
        clear_buffer();
        status = validate_bank_name(e->bank_name);
        if (status == -1)
            printf("Invalid bank name! Use letters and spaces only.\n");
    } while (status == -1);

    // ---------------------------------------- Bank Account No. ----------------------------------------
    do {
        printf("\nBank Account Number: ");
        scanf(" %49[^\n]", e->bank_account);
        clear_buffer();
        status = validate_bank_account(e->bank_account);
        if (status == -1)
            printf("Invalid account number! Use 8 to 34 letters/digits, no spaces or symbols.\n");
    } while (status == -1);

    // ------------------------------------------ Basic Salary ------------------------------------------
    do {
        printf("\nBasic Salary (in PKR): ");
        status = read_float(&e->basic_salary);
        if (status == 0)
            status = validate_basic_salary(e->basic_salary);
        if (status == -1)
            printf("Invalid salary! Enter a number greater than 0.\n");
    } while (status == -1);

    // -------------------------------------------- Allowance -------------------------------------------
    do {
        printf("\nAllowance (in PKR) : ");
        status = read_float(&(*e).allowance);
        if (status == 0)                               // solely a float was given as input.
            status = validate_allowance(e->allowance);
        if (status == -1)                              // input had some other characters also apart from the number, so marked as invalid.
            printf("Invalid allowance! Enter a number (0 or more).\n");
    } while (status == -1);

    // ------------------------------------------- Overtime Pay -----------------------------------------
    do {
        printf("\nOvertime Pay (in PKR): ");
        status = read_float(&e->overtime_pay);
        if (status == 0)
            status = validate_overtime(e->overtime_pay);
        if (status == -1)
            printf("Invalid overtime pay! Enter a number (0 or more).\n");
    } while (status == -1);

    // ---------------------------------------------- Bonus ---------------------------------------------
    do {
        printf("\nBonus (in PKR): ");
        status = read_float(&e->bonuses);
        if (status == 0)
            status = validate_bonuses(e->bonuses);
        if (status == -1)
            printf("Invalid bonus! Enter a number (0 or more).\n");
    } while (status == -1);

    // ---------------------------------------------- Taxes ---------------------------------------------
    do {
        printf("\nTaxes (in PKR): ");
        status = read_float(&e->taxes);
        if (status == 0)
            status = validate_taxes(e->taxes, e->basic_salary + e->allowance + e->overtime_pay + e->bonuses);
        if (status == -1)
            printf("Invalid taxes! Enter a number (0 or more) that doesn't exceed the gross salary.\n");
    } while (status == -1);

    // Automatically calculate Net Salary
    e->net_salary = e->basic_salary + e->allowance + e->overtime_pay + e->bonuses - e->taxes;
}

int add_employee(void) {                                                        
    if (employee_count < Max_Employee) {               // CHECKING IF ANY MORE EMPLOYEES CAN BE ADDED 

        input();                                       // TAKING INPUT (every field is already validated inside input()).
        
        FILE *fp;
        fp = fopen("Employee Payroll Records.csv", "a");                     // OPENING FILE IN APPEND MODE.
        if (fp != NULL) {                                                    // FILE ERROR.

            // APPENDING INPUT TAKEN FROM STRUCTURE TO THE CSV FILE. 
            fprintf(fp, "%s, %s, %s, %s, %s, %s, %.2f, %s, %s, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n",  
                emp[employee_count].name, emp[employee_count].id, emp[employee_count].designation, 
                emp[employee_count].department, emp[employee_count].email, emp[employee_count].date_joining, 
                emp[employee_count].experience, emp[employee_count].bank_name, emp[employee_count].bank_account, 
                emp[employee_count].basic_salary, emp[employee_count].allowance, emp[employee_count].overtime_pay, 
                emp[employee_count].bonuses, emp[employee_count].taxes, emp[employee_count].net_salary);
 
            employee_count++ ;                                                // 1 EMPLOYEE ADDED.
            
            fclose(fp);        

            fp = fopen("values.txt", "w");                                    // Storing no. of employees permanently in a file.
            if (fp != NULL) {
                fprintf(fp, "%d", employee_count);
                fclose(fp);
                printf("\nEmployee appended successfully in the file!\n");
                return 0;
            }
            return -1;                                                        // couldn't update values.txt

        } else {
            printf("\nError! unable to open file!\n");                        // FP = NULL
            return -1;
        }
    } else {
        printf("\nMaximum employee limit has been reached, no more employee entry possible.\n");                  // employee_count = MAX_EMPLOYEE
        return -1;
    }
}

// =====================================================================================================
//                                           INPUT HELPERS
// =====================================================================================================

// Discards everything left in the input buffer up to and including the next '\n'.
void clear_buffer(void){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Reads one float from the user. Returns 0 on success, -1 if the input wasn't a proper number
// (e.g. "abc" or "12abc"). Always leaves the input buffer clean, so the retry loop can't get stuck.
int read_float(float *value){
    int c;

    if (scanf(" %f", value) != 1) {         // sth numeric red so scanf returned 1 ---> (1 != 1) --> condition false --> buffer not cleared.
        clear_buffer();                     // if nothing numeric red by scanf(),---> scanf() returns 0 ---> condition true ---> then it clears the buffer. 
        return -1;                          // if input is sth like "12abc\n", then scanf reads 12 but abc\n remains in buffer and needs to be removed.
    }
    // if input is sth like "12abc\n", then scanf reads 12 but abc\n remains in buffer and needs to be removed.
    
    while ((c = getchar()) == ' ' || c == '\t');     // removes any trailing spaces '    ' from buffer.
    if (c != '\n' && c != EOF) {                     // Since, neither a new line nor end of buffer --> condition true --> clear the buffer
        clear_buffer();
        return -1;                                   // also return -1, to indicate to input() function that the input was not puerly correct.
    } 
    return 0;
}

// This function is used by name, designation, department and bank name:
// only letters and spaces are allowed, and at least one letter must be present.
int only_letters_and_spaces(const char *text){
    int i;

    for (i = 0; text[i] != '\0'; i++) {
        // Cast to unsigned char to safely handle negative extended ASCII values.
        if (isalpha((unsigned char)text[i]) == 0 && text[i] != ' ')    // use of unsigned char removes the sign bit, so safe comparison, no crash in program.
            return -1;                                                 // isalpha() checks whether a character is a letter (returns 0 if not a letter).
    }

    return 0;
}

// =====================================================================================================
//                                        FIELD VALIDATORS
//                                  return 0 = valid, -1 = invalid
// =====================================================================================================

int validate_name(const char *name){
    return only_letters_and_spaces(name);
}

int validate_id(const char *id){
    int i;

    if (id[0] == '\0')
        return -1;
    for (i = 0; id[i] != '\0'; i++) {
        if (isalnum((unsigned char)id[i]) == 0 && id[i] != '-')         // isalnum checks for a letter or digit (returns 0 if not a letter or digit).
            return -1;                                                  // given an exception of hyphen '-' in id.
    }
    return 0;
}

int validate_designation(const char *designation){
    return only_letters_and_spaces(designation);
}

int validate_department(const char *department){
    return only_letters_and_spaces(department);
}

int validate_email(const char *email){
    int i;

    if (strlen(email) < 5)                              // email can't be of less than 5 characters logically (a@b.c).
        return -1;

    for (i = 0; i < strlen(email); i++) {               // there should be no spaces in email.
        if (isspace((unsigned char)email[i]) )          // if a space in email, isspace() returns 1, condition satisfies, so an invalid input.
            return -1;
    }

    const char *at = strchr(email, '@');                // strchr scans through the email string left to right, looking for the @ character. 
    if (at == NULL)                                     // unsigned char removes the sign bit, so safe comparison, no crash in program.
        return -1;                                      // If it doesn't find the @, it returns NULL (which means "nothing found").

    if (strchr(at + 1, '@') != NULL)                    // there should be only ONE '@' symbol
        return -1;

    if (at == email)                                    // the '@' cannot be the first character
        return -1;

    const char *dot = strchr(at + 1, '.');              // email must have a dot '.' somewhere after the '@'
    if (dot == NULL)
        return -1;

    if (dot == at + 1)                                  // the '.' can't be immediately after the '@' (e.g. user@.com)
        return -1;

    if (email[strlen(email) - 1] == '.')                          // the '.' cannot be the very last character
        return -1;

    return 0;
}

int validate_experience(float years){
    if (years < 0 || years > 60)
        return -1;
    return 0;
}

int validate_bank_name(const char *bank_name){
    return only_letters_and_spaces(bank_name);
}

int validate_bank_account(const char *account){
    int i;

    if (strlen(account) < 8 || strlen(account) > 34)                  // account no. / IBAN length range 
        return -1;
    for (i = 0; i < strlen(account); i++) {
        if (!isalnum((unsigned char)account[i]))
            return -1;
    }
    return 0;
}

int validate_basic_salary(float amount){
    
    if (amount <= 0)                                    // basic salary must be positive
        return -1;
    return 0;
}

int validate_allowance(float amount){

    if (amount < 0)
        return -1;
    return 0;

}

int validate_overtime(float amount){
    if (amount < 0)
        return -1;
    return 0;
}

int validate_bonuses(float amount){
    if (amount < 0)
        return -1;
    return 0;
}

int validate_taxes(float taxes, float gross_salary){
    
    if (taxes < 0)
        return -1;
    if (taxes > gross_salary)                           // otherwise the net salary would become negative
        return -1;
    return 0;
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
   void savedata() {
    FILE *fp = fopen("Employee Payroll Records.csv", "w");
    if (fp != NULL) {
        for (int i = 0; i < employee_count; i++) {
            fprintf(fp, "%s, %s, %s, %s, %s, %s, %.2f, %s, %s, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", 
                emp[i].name, emp[i].id, emp[i].designation, 
                emp[i].department, emp[i].email, emp[i].date_joining, 
                emp[i].experience, emp[i].bank_name, emp[i].bank_account, 
                emp[i].basic_salary, emp[i].allowance, emp[i].overtime_pay, 
                emp[i].bonuses, emp[i].taxes, emp[i].net_salary);
        }
        fclose(fp);
    }
FILE *v_fp = fopen("values.txt", "w");
    if (v_fp != NULL) {
        fprintf(v_fp, "%d", employee_count);
        fclose(v_fp);
    }
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
    //saveData(); 
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
            //saveData();
            printf("Designation updated and saved successfully!\n");
            break; }
        case 2:
        {
            printf("Enter new Department: ");
            scanf("%99[^\n]", emp[index].department);
            //saveData();
            printf("Department updated and saved successfully!\n");
            break; }
        case 3:
        {
            printf("Enter new Email: ");
            scanf("%99s", emp[index].email);
            //saveData();
            printf("Email updated and saved successfully!\n");
            break; }
        case 4:
        {
            printf("Enter updated Experience (years): ");
            scanf("%f", &emp[index].experience);
            //saveData();
            printf("Experience updated and saved successfully!\n");
            break; }
        case 5:
        {
            printf("Enter new Basic Salary: ");
            scanf("%f", &emp[index].basic_salary);
            emp[index].net_salary = emp[index].basic_salary + emp[index].allowance + emp[index].bonuses + emp[index].overtime_pay - emp[index].taxes;
            //saveData();
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

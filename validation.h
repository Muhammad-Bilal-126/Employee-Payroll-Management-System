
#ifndef VALIDATION_H
#define VALIDATION_H

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

#endif
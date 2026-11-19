#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "validation.h"
#include <math.h>
#include <time.h>

#include "helper.h"


/* =====================================================================
                            VALIDATORS
===================================================================== */

int validate_name(const char *name) {
    return only_letters_and_spaces(name);
}

int validate_id(const char *id) {
    int i;

    for (i = 0; id[i] != '\0'; i++) {
        if (isalnum((unsigned char)id[i]) == 0) {
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
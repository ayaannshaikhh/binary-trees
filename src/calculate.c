#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

struct node {
  struct node *left;
  char *data;
  struct node *right;
};

struct var {
    char name[20];
    double value;
};

void trim(char *op) {
    int length = strlen(op);
    if (length < 2) {
        return;
    }

    if (op[0] == '(' && op[length - 1] == ')') {
        int depth = 0;
        for (int i = 0; i < length - 1; i++) {
            if (op[i] == '(') {
                depth++;
            } else if (op[i] == ')') {
                depth--;
            }
            if (depth == 0 && i < length - 1) {
                return;
            }
        }
        // Copies the first value AFTER the first bracket and the 1st value before the last bracket
        // length - 2 = length - 1 - bracket pos to get value after last bracket
        // op + 1 = op + bracket pos to get value after bracket pos
        // strncpy(destination, source, size)
        strncpy(op, op + 1, length - 2);
        op[length - 2] = '\0'; // add null terminator to end of operation to determine end
    }
}

// Check for negatives in argv[2]
int checkNegativeValues(char *vars) {
    int length = strlen(vars);

    for (int i = 0; i < length; i++) {
        if (vars[i] == '-') {
            return 0;
        }
    }
    return 1;
}

void lowerVariableXIfCapitalized(char *arg) {
    int length = strlen(arg);

    for (int i = 0; i < length; i++) {
        if (arg[i] == 'X') {
            arg[i] = tolower(arg[i]); // will convert X to x
        }
    }
}

int checkBracketsForSyntaxError(char *op) {
    int length = strlen(op);
    int depth = 0;

    // Checking if number of open parentheses = number of closed parentheses
    for (int i = 0; i < length; i++) {
        if (op[i] == '(') {
            depth++;
        } else if (op[i] == ')') {
            depth--;
            // If depth is less than 0, means a ) preceded a ( and returns -1
            // Depth decrements so it would be -1 if ) comes before a (
            if (depth < 0) {
                return 0;
            }
        }
    }

    // If unequal, return error
    if (depth != 0) {
        return 0;
    }

    return 1;
}

int checkCharactersForSyntaxError(char *op) {
    const char *var = "x";
    const char *allowed = "x1234567890+-*/().";
    const char *nums = "1234567890";
    for (int i = 0; op[i] != '\0'; i++) {
        if (strchr(allowed, op[i]) == NULL) {
            return 0;
        }

        // Checks if the current character we're at is "x"
        // Then checks if the value after is 0, returns 0 because that shouldn't happen
        // Then checks if there is a second digit after the x, means that there are more than 10 variables
        if (strchr(var, op[i]) != NULL) {
            if(op[i+1] == 0) {
                return 0;
            } else if (strchr(nums, op[i+2]) != NULL) {
                return 0;
            }
        }
    }
    return 1;
}

/*
int divisionByZero(char *op, char *vars) {
    int length = strlen(op);
    for (int i = 0; i < length; i++) {
        if (op[i] == '/') {
            if (op[i+1] == 'x') {
                char var[3];
                var[0] = op[i+1];
                var[1] = op[i+2];
                var[2] = '\0';

                char copy[256];
                strcpy(copy, vars);
                char *value = strtok(copy, ";");

                while (value != NULL) {
                    char name[10], val[20];
                    sscanf(value, "%[^=]=%s", name, val);
                    if (strcmp(name, var) == 0 && atof(val) == 0.0) {
                        return 1;
                    }
                    value = strtok(NULL, ";");
               }
            }
        }
    }
    return 0;
}
    
*/

int checkFloatsForRange(char *op, char *vars) {
    float min = 0.00;
    float max = 9.99;
    const char *nums = "0123456789";
    for (int i = 0; op[i] != '\0'; i++) {
        // checks if i > 0, op[i] is a decimal and if the value before is a number
        if (i > 0 && op[i] == '.' && (strchr(nums, op[i-1]) != NULL)) {
            if (i < 1 || op[i+1] == '\0' || op[i+2] == '\0') {
                continue;
            }
            if (strchr(nums, op[i+1]) == NULL || strchr(nums, op[i+2]) == NULL) {
                return 0;
            }
            char temp[5];
            temp[0] = op[i-1]; // first digit
            temp[1] = op[i]; // dot
            temp[2] = op[i+1]; // first decimal
            temp[3] = op[i+2]; // second decimal
            // temp[4] = op[i+3] represents the 3rd decimal spot if one exists

            if (op[i+3] != '\0' && strchr(nums, op[i+3]) != NULL) { // checks if there is a third decimal number
                if (op[i+3] >= 5) { // checks if the third digit is greater than or equal to 5
                    temp[i+2] = temp[i+2] + 1; // increases the 2nd digit by 1 (rounding)
                }
                return 0; // return false: float is NOT in range (should be 2 decimal points)
            } else {
                temp[4] = '\0';
                float tempFloat = atof(temp);
                if (tempFloat < min || tempFloat > max) {
                    return 0;
                }
            }
        }
    }

    for (int j = 0; j < strlen(vars); j++) {
        if (j > 0 && vars[j] == '.' && (strchr(nums, vars[j-1]) != NULL)) {
            if (j < 1 || vars[j+1] == '\0' || vars[j+2] == '\0') {
                continue;
            }
            if (strchr(nums, vars[j+1]) == NULL || strchr(nums, vars[j+2]) == NULL) {
                return 0;
            }

            char temp[5];
            temp[0] = vars[j-1]; // first digit
            temp[1] = vars[j]; // dot
            temp[2] = vars[j+1]; // first decimal
            temp[3] = vars[j+2]; // second decimal

            if (vars[j+3] != '\0' && strchr(nums, vars[j+3])) { // checks if there is a third decimal number
                return 0; // return false: float is NOT in range
            } else {
                temp[4] = '\0';
                float tempFloat = atof(temp);
                if (tempFloat < min || tempFloat > max) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int findRootPos(char *op) {
    int length = strlen(op);
    int depth = 0;
    int minDepth = 9999;
    int rootPos = -1;

    for (int i = 0; i < length; i++) {
        if (op[i] == '(') {
            depth++;
        } else if (op[i] == ')') {
            depth--;
        } else if (op[i] == '+' || op[i] == '-' || op[i] == '*' || op[i] == '/') {
            if (depth <= minDepth) {
                minDepth = depth;
                rootPos = i;
            }
        }
    }
    return rootPos;
}

double lookupVar(const char *name, struct var vars[], int varCount) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            return vars[i].value;
        }
    }

    fprintf(stderr, "Error: undefined argument.\n");
    exit(1);
}

double evaluate(char *op, struct var vars[], int varCount) {
    char expr[256];
    strncpy(expr, op, sizeof(expr) - 1);
    expr[sizeof(expr)-1] = '\0';
    trim(expr);

    int rootPos = findRootPos(expr);

    if (rootPos == -1) {
        if (isdigit(expr[0]) || expr[0] == '.' || expr[0] == '-' || expr[0] == '+') {
            return atof(expr);
        } else {
            return lookupVar(expr, vars, varCount);
        }
    }

    char root = expr[rootPos];
    char leftOfRoot[200], rightOfRoot[200];

    strncpy(leftOfRoot, expr, rootPos);
    leftOfRoot[rootPos] = '\0';
    strcpy(rightOfRoot, expr + rootPos + 1);

    trim(leftOfRoot);
    trim(rightOfRoot);

    double leftValue = evaluate(leftOfRoot, vars, varCount);
    double rightValue = evaluate(rightOfRoot, vars, varCount);

    if (root == '/' && rightValue == 0.0) {
        fprintf(stderr, "Error: division by zero.\n");
        exit(1);
    }

    switch (root) {
        case '+': return leftValue + rightValue;
        case '-': return leftValue - rightValue;
        case '*': return leftValue * rightValue;
        case '/': return leftValue / rightValue;
    }
    return 0.0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf( stderr, "Error: undefined argument.\n");
        return 1;
    }

    char *op = argv[1];
    char *variables = argv[2];

    lowerVariableXIfCapitalized(op);
    lowerVariableXIfCapitalized(variables);

    int bracketCheck = checkBracketsForSyntaxError(op);
    int characterCheck = checkCharactersForSyntaxError(op);

    // Check for negatives in argv[2]
    int negativeCheck = checkNegativeValues(variables);

    if (bracketCheck == 0) {
        fprintf( stderr, "Error: syntax error.\n" );
        return -1;
    }

    if (characterCheck == 0) {
        fprintf( stderr, "Error: syntax error.\n" );
        return -1;
    }

    if (negativeCheck == 0) {
        fprintf( stderr, "Error: syntax error. negative \n" );
        return -1;
    }

    int floatCheck = 1;
    if (argc >= 3) {
        floatCheck = checkFloatsForRange(op, variables);
    } else {
        floatCheck = checkFloatsForRange(op, "");
    }

    if (floatCheck == 0) {
    fprintf(stderr, "Error: syntax error.\n");
    return -1;
    }

    struct var vars[20];
    int varCount = 0;

    if (argc >= 3) {
    char *token = strtok(argv[2], ";");
    while (token != NULL) {
        char *eq = strchr(token, '=');
        if (eq) {
            *eq = '\0';
            strcpy(vars[varCount].name, token);
            vars[varCount].value = atof(eq + 1);
            varCount++;
            }
        token = strtok(NULL, ";");
        }
    }

    double result = evaluate(argv[1], vars, varCount);
    printf("%4.2f\n", result);
    return 0;
}

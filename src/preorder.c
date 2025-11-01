#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct node {
  struct node *left;
  char *data;
  struct node *right;
};

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
    const char *allowed = "x1234567890+-*/().";
    for (int i = 0; op[i] != '\0'; i++) {
        if (strchr(allowed, op[i]) == NULL) {
            return 0;
        }
    }
    return 1;
}

// Custom strdrup function to avoid any warnings or errors generated via compilation
char* my_strdup(const char *src) {
    if (src == NULL) {
        return NULL;
    }

    char *copy = malloc(strlen(src) + 1);

    if (copy != NULL) {
        strcpy(copy, src);
    }

    return copy;
}

struct node* create(const char *data) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->data = my_strdup(data);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct node* insertLeftNode(struct node* root, char value) {
    char str[2];
    str[0] = value;
    str[1] = '\0';
    root->left = create(str);
    return root->left;
}

struct node* insertRightNode(struct node* root, char value) {
    char str[2];
    str[0] = value;
    str[1] = '\0';
    root->right = create(str);
    return root->right;
}

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

int findRoot(char *op) {
    int depth = 0;
    int minDepth = 9999;
    int rootPos = -1;

    for (int i = 0; i < strlen(op); i++) {
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

struct node* buildTree(char *op) {
    if (op == NULL || strlen(op) == 0) {
        return NULL;
    }

    trim(op);

    int rootPos = findRoot(op);
    if (rootPos == -1) {
        return create(op);
    }

    char str[2] = { op[rootPos], '\0' };
    struct node *tree = create(str);

    char left[256], right[256];
    strncpy(left, op, rootPos);
    left[rootPos] = '\0';
    strcpy(right, op + rootPos + 1);

    trim(left);
    trim(right);

    tree->left = buildTree(left);
    tree->right = buildTree(right);

    return tree;
}

void preorder(struct node *root) {
    if (!root) {
        return;
    }

    printf("%s ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void freeTree(struct node *root) {
    if (!root) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root->data);
    free(root);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf( stderr, "Error: undefined argument.\n");
        return 1;
    }

    char *op = argv[1];
    lowerVariableXIfCapitalized(op);

    int bracketCheck = checkBracketsForSyntaxError(op);
    int characterCheck = checkCharactersForSyntaxError(op);
    
    
    if (bracketCheck == 0) {
        fprintf( stderr, "Error: syntax error.\n" );
        return -1;
    }

    if (characterCheck == 0) {
        fprintf( stderr, "Error: syntax error.\n" );
        return -1;
    }

    struct node *tree = buildTree(op);
    preorder(tree);
    printf("\n");

    freeTree(tree);

    return 0;
}

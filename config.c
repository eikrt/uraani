#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
Node *createNode(char type, char *value) {
        Node *node = (Node *)malloc(sizeof(Node));
        if (node == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
        }
        node->type = type;
        node->value = value;
        node->next = NULL;
        node->child = NULL;
        return node;
}

void freeNodes(Node *node) {
        if (node == NULL) return;
        freeNodes(node->next);
        freeNodes(node->child);
        free(node);
}

void printSExpression(Node *node) {
        if (node == NULL) return;
        if (node->type == 'L') {
                printf("(");
                printSExpression(node->child);
                printf(")");
                if (node->next != NULL) {
                        printf(" ");
                        printSExpression(node->next);
                }
        } else if (node->type == 'S') {
                printf("%s", node->value);
                if (node->next != NULL) {
                        printf(" ");
                        printSExpression(node->next);
                }
        }
}

// Function to
// parse
// S-expressions
Node *parseSExpression(const char *expr, int *index) {
        char ch;
        Node *head = NULL;
        Node *current = NULL;

        while ((ch = expr[*index]) != '\0') {
                if (isspace(ch)) {
                        // Skip whitespace
                        (*index)++;
                } else if (ch == '(') {
                        // Start of a list
                        (*index)++;
                        Node *newNode = createNode('L', NULL);
                        if (head == NULL) {
                                head = newNode;
                        } else {
                                current->next = newNode;
                        }
                        current = newNode;
                        current->child = parseSExpression(expr, index);
                } else if (ch == ')') {
                        // End of a list
                        (*index)++;
                        return head;
                } else {
                        // Symbol
                        int start = *index;
                        while (expr[*index] != '\0' && !isspace(expr[*index]) &&
                               expr[*index] != '(' && expr[*index] != ')') {
                                (*index)++;
                        }
                        int length = *index - start;
                        char *value = (char *)malloc(length + 1);
                        if (value == NULL) {
                                fprintf(stderr,
                                        "Error: Memory allocation failed\n");
                                exit(EXIT_FAILURE);
                        }
                        strncpy(value, &expr[start], length);
                        value[length] = '\0';
                        Node *newNode = createNode('S', value);
                        if (head == NULL) {
                                head = newNode;
                        } else {
                                current->next = newNode;
                        }
                        current = newNode;
                }
        }

        return head;
}


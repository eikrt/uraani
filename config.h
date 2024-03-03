typedef struct Node Node;

struct Node {
        char type;    // 'L' for list, 'S' for symbol
        char *value;  // Value of the node
        Node *next;   // Next node in the list
        Node *child;  // First child node in the list (if the node is
};

Node *createNode(char type, char *value);
void freeNodes(Node *node);
void printSExpression(Node *node);
Node *parseSExpression(const char *expr, int *index);

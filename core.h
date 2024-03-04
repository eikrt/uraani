typedef struct Node Node;

struct Node {
    int x;
    int y;
    int width;
    int height;    
    int id;    
    char* name;
    Node* children;
};

typedef struct Tile {
    int x;
    int y;
    int size;
} Tile;

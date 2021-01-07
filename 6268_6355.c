#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node Node;
struct Node {
    char *word;
    Node *left;
    Node *right;
};

Node *newNode(char *word);

Node *insert(Node *root, char *word);

void print(Node *root);

Node *readDictionaryIntoTree();

int size(Node *root);

void check();

int height(Node *root);

Node *search(Node *root, char *key);

Node *getMin(Node *root);

Node *getSuccessor(Node *root, Node *lastNode);

Node *getPredecessor(Node *root, Node *lastNode);

Node *getMax(Node *root);

int max(int x, int y);

int main() {
    check();
}

Node *newNode(char *word) {
    Node *n = malloc(sizeof(Node));
    n->left = n->right = NULL;
    n->word = malloc(strlen(word) + 1);
    strcpy(n->word, word);
    return n;
}

Node *insert(Node *root, char *word) {
    if (!root) root = newNode(word);
    else if (strcasecmp(word, root->word) < 0) root->left = insert(root->left, word);
    else root->right = insert(root->right, word);
    return root;
}

Node *getMin(Node *root) {
    Node *min = NULL;
    while (root) {
        min = root;
        root = root->left;
    }
    return min;
}

Node *getMax(Node *root) {
    Node *max = NULL;
    while (root) {
        max = root;
        root = root->right;
    }
    return max;
}

void print(Node *root) {
    if (root) {
        printf("%s ", root->word);
        print(root->right);
        print(root->left);
    }
}

Node *readDictionaryIntoTree() {
    Node *root = NULL;
    char buffer[50];
    FILE *fp = fopen("EN-US-Dictionary.txt", "r");
    if (!fp) {
        printf("Failed to open dictionary");
        exit(0);
    } else {
        while (!feof(fp)) {
            fgets(buffer, 50, fp);
            buffer[strlen(buffer) - 1] = '\0';
            root = insert(root, buffer);
        }
    }
    return root;
}

int size(Node *root) {
    if (root == NULL)
        return 0;
    else
        return 1 + size(root->left) + size(root->right);

}

int max(int x, int y) {
    return x > y ? x : y;
}

int height(Node *root) {
    if (root == NULL)
        return -1;
    else
        return 1 + max(height(root->left), height(root->right));
}

void check() {
    Node *dictionary = readDictionaryIntoTree();
    printf("Dictionary Loaded Successfully...!\n");
    printf("..................................\n");
    printf("Size=%d\n", size(dictionary));
    printf("..................................\n");
    printf("Height=%d\n", height(dictionary));
    printf("..................................\n");
    printf("Enter a sentence :\n");
    char sentence[250];
    fgets(sentence, 250, stdin);
    sentence[strlen(sentence) - 1] = '\0';
    char *token = strtok(sentence, " ");

    while (token) {
        Node *result = search(dictionary, token);
        if (!strcasecmp(token, result->word))
            printf("%s - CORRECT\n", token);
        else {
            printf("%s - INCORRECT,Suggestions : ", token);
            print(result);
            printf("\n");
        }
        token = strtok(NULL, " ");
    }
}

Node *search(Node *root, char *key) {
    Node *found = newNode(key);
    Node *savedRoot = root;
    Node *leaf;
    while (root) {
        leaf = root;
        if (!strcasecmp(key, root->word))
            return found;
        else if (strcasecmp(key, root->word) < 0)
            root = root->left;
        else root = root->right;
    }
    Node *suggestions = insert(suggestions, leaf->word);
    suggestions = insert(suggestions, getSuccessor(savedRoot, leaf)->word);
    suggestions = insert(suggestions, getPredecessor(savedRoot, leaf)->word);
    return suggestions;
}

Node *getSuccessor(Node *root,Node *lastNode) {
    Node *successor = NULL;
    if (lastNode->right)
        return getMin(lastNode->right);
    while (root != lastNode) {
        if (strcasecmp(root->word, lastNode->word) > 0) {
            successor = root;
            root = root->left;
        } else
            root = root->right;

    }
    return successor;
}

Node *getPredecessor(Node *root, Node *lastNode) {

    Node *predecessor = NULL;
    if (lastNode->left)
        return getMax(lastNode->left);
    while (root != lastNode) {
        if (strcasecmp(root->word, lastNode->word) < 0) {
            predecessor = root;
            root = root->right;
        } else
            root = root->left;

    }
    return predecessor;
}
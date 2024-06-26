#include <stdio.h>
#include <stdlib.h>
#include <klee/klee.h>

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct BST {
    struct TreeNode *root;
};

struct TreeNode *createNode(int _val) {
    struct TreeNode *newNode = (struct TreeNode *) malloc(sizeof(struct TreeNode));
    newNode->val = _val;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void insert(struct BST *tree, int val) {
    if (tree->root == NULL) {
        tree->root = createNode(val);
        return;
    }

    struct TreeNode *node = tree->root;
    while (node != NULL) {
        if (val < node->val) {
            if (node->left == NULL) {
                node->left = createNode(val);
                return;
            } else {
                node = node->left;
            }
        } else {
            if (node->right == NULL) {
                node->right = createNode(val);
                return;
            } else {
                node = node->right;
            }
        }
    }
}

int search(struct BST *tree, int val) {
    struct TreeNode *node = tree->root;
    while (node != NULL) {
        if (val == node->val) {
            return 1;
        } else if (val < node->val) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return 0;
}

void inorderTraversal(struct TreeNode *node) {
    if (node == NULL) {
        return;
    }
    inorderTraversal(node->left);
    printf("%d ", node->val);
    inorderTraversal(node->right);
}

void printInorder(struct BST *tree) {
    inorderTraversal(tree->root);
}

struct TreeNode *minValueNode(struct TreeNode *node) {
    struct TreeNode *current = node;
    while (current != NULL && current->left != NULL) {
        current = current->left;
    }
    return current;
}

struct TreeNode* findMin(struct TreeNode* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

struct TreeNode* deleteNode(struct TreeNode* root, int key) {
    if (root == NULL) return root;

    if (key < root->val) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->val) {
        root->right = deleteNode(root->right, key);
    } else {
        if (root->left == NULL) {
            struct TreeNode* temp = root->right;
            //free(root);
            return temp;
        } else if (root->right == NULL) {
            struct TreeNode* temp = root->left;
            //free(root);
            return temp;
        }

        struct TreeNode* temp = findMin(root->right);
        root->val = temp->val;
        root->right = deleteNode(root->right, temp->val);
    }
    return root;
}

int main() {
    int argc = 1;
    const int max_argc = 11; // 设置一个合理的最大值
    char *argv[max_argc];
    for (int i = 0; i < max_argc; i++) {
        argv[i] = malloc(11);
    }
    klee_make_symbolic(&argc, sizeof(argc), "argc");
    klee_assume(argc >= 1);
    klee_assume(argc <= 10);


    for (int i = 1; i < argc; i++) {
        char name[20];
        sprintf(name, "argv%d", i);
        klee_make_symbolic(argv[i], 11, name);
    }


    struct BST *tree = malloc(sizeof(struct BST));
    tree->root = NULL;

    for (int i = 1; i < argc; i++) {
        //int insertVal = atoi(argv[i]);
        int insertVal = *argv[i];
        insert(tree, insertVal);
    }

    // Inorder traversal of the BST:
    printInorder(tree);
    printf("\n");

    for (int i = 1; i < argc; i ++) {
        int deleteVal = *argv[i];
        tree->root = deleteNode(tree->root, deleteVal);
    }

    // Inorder traversal of the BST:
    printInorder(tree);
    printf("\n");
    return 0;
}

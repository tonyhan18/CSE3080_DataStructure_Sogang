// C program to delete a node from AVL Tree 
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h> 
#include<stdlib.h> 
#include <time.h>

// An AVL tree node 
typedef struct Node
{
    int key;
    int height;
    int count;
    struct Node* left;
    struct Node* right;
    
}Node;


// A utility function to get height of the tree 
int height(struct Node* N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
struct Node* newNode(int key)
{
    struct Node* node = (struct Node*)
        malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  // new node is initially added at leaf 
    node->count = 1;
    return(node);
}

// A utility function to right rotate subtree rooted with y 
// See the diagram given above. 
struct Node* rightRotate(struct Node* y)
{
    struct Node* x = y->left;
    struct Node* T2 = x->right;

    // Perform rotation 
    x->right = y;
    y->left = T2;

    // Update heights 
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return new root 
    return x;
}

// A utility function to left rotate subtree rooted with x 
// See the diagram given above. 
struct Node* leftRotate(struct Node* x)
{
    struct Node* y = x->right;
    struct Node* T2 = y->left;

    // Perform rotation 
    y->left = x;
    x->right = T2;

    //  Update heights 
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return new root 
    return y;
}

// Get Balance factor of node N 
int getBalance(struct Node* N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

struct Node* insert(struct Node* node, int key)
{
    /* 1.  Perform the normal BST rotation */
    if (node == NULL)
        return(newNode(key));

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else {
        node->count++;
        return node;
    }

    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
        height(node->right));

    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then there are 4 cases 

    // Left Left Case 
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right Right Case 
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left Right Case 
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case 
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

/* Given a non-empty binary search tree, return the
   node with minimum key value found in that tree.
   Note that the entire tree does not need to be
   searched. */
struct Node* minValueNode(struct Node* node)
{
    struct Node* current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}

// Recursive function to delete a node with given key 
// from subtree with given root. It returns root of 
// the modified subtree. 
struct Node* deleteNode(struct Node* root, int key)
{
    // STEP 1: PERFORM STANDARD BST DELETE 

    if (root == NULL)
        return root;

    // If the key to be deleted is smaller than the 
    // root's key, then it lies in left subtree 
    if (key < root->key)
        root->left = deleteNode(root->left, key);

    // If the key to be deleted is greater than the 
    // root's key, then it lies in right subtree 
    else if (key > root->key)
        root->right = deleteNode(root->right, key);

    // if key is same as root's key, then This is 
    // the node to be deleted 
    else
    {
        if (--(root->count) == 0) {
            // node with only one child or no child 
            if ((root->left == NULL) || (root->right == NULL))
            {
                struct Node* temp = root->left ? root->left :
                    root->right;

                // No child case 
                if (temp == NULL)
                {
                    temp = root;
                    root = NULL;
                }
                else // One child case 
                    *root = *temp; // Copy the contents of 
                                   // the non-empty child 
                free(temp);
            }
            else
            {
                // node with two children: Get the inorder 
                // successor (smallest in the right subtree) 
                struct Node* temp = minValueNode(root->right);

                // Copy the inorder successor's data to this node 
                root->key = temp->key;

                // Delete the inorder successor 
                root->right = deleteNode(root->right, temp->key);
            }
        }       
    }

    // If the tree had only one node then return 
    if (root == NULL)
        return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE 
    root->height = 1 + max(height(root->left),
        height(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to 
    // check whether this node became unbalanced) 
    int balance = getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases 

    // Left Left Case 
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case 
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case 
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case 
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

int modifiedsearch(Node* root,int data) {
    Node* p = root;
    if (!p) return 0;

    while (p) {
        if (data == p->key) return 1;
        if (data < p->key) {
            if (!p->left) break;
            p = p->left;
        }
        else {
            if (!p->right) break;
            p = p->right;
        }
    }

    return 0;
}

int main(int argc, char* argv[]) {
    //FILE* fp1 = fopen(argv[1], "r");
    FILE* fp1 = fopen("input.txt", "r");
    FILE* fp2 = fopen("db_result.txt", "w");

    char op;
    int x;
    struct Node* root = NULL;

    clock_t start_time = clock();
    while (1) {
        if ((op = fgetc(fp1)) == EOF) break;
        fscanf(fp1, "%d", &x);


        //만약 중복 데이터라면 노드의 count 수만 증가
        if (op == 'i') {
            insert(root,x);
        }
        //데이터를 찾고 갯수 감소후 찾은 데이터의 갯수가 0이라면 삭제
        else if (op == 'd') {
            if (!deleteNode(root, x)) {
                fprintf(fp2, "ignore\n");
            }
        }
        //데이터 찾기
        else if (op == 's') {
            if (modifiedsearch(root,x)) {
                fprintf(fp2, "true\n");
            }
            else {
                fprintf(fp2, "false\n");
            }
        }
    }
    printf("%lf\n", (double)(clock() - start_time) / CLOCKS_PER_SEC);

    fclose(fp1);
    fclose(fp2);
}
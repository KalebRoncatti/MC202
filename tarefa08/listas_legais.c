#include <stdio.h>
#include <stdlib.h>
//#include <time.h>

/* 
There are some controversies about choosing RB trees as "left-sided". However, for our applications
it will fit well enough since we are not going to remove anything and the number of cases that we need
to treat in the insertion algorithm is lower than we would have to do with RB trees directly */

// LLRB BS Tree
enum Color {RED, BLACK};
typedef struct node {
    long int element; // Element from the "Nice list"
    int frequency; // Since we cannot add the same element twice in the same tree we will need to count
    enum Color color;
    struct node *left;
    struct node *right;
} node;

// pointer towards a tree
typedef node *p_node;

// Verify if a current node is red
int is_red(p_node curr){
    if (curr == NULL){
        return 0; // A null node is black by definition of a LLRB tree
    }
    if (curr->color == RED)
        return 1;
    return 0;
}

// Verify if a current node is black
int is_black(p_node curr){
    if (curr == NULL || curr->color == BLACK)
        return 1;
    return 0;
}

/* Now let's implement the basic operations for a Left-Leaning Red-Black (LLRB) Tree. 
Before writing the insertion algorithm we need some operations such as:
- rotating tree to the left
- rotating tree to the right
- "Upping" the red

These operations will help us to "correct" the tree so it will become a LLRB again 
when inserting new elements
*/

p_node rotate_left(p_node root){
    p_node new_root = root->right;

    root->right = new_root->left;
    new_root->left = root;
    new_root->color = root->color;
    root->color = RED;
    return new_root;
}

p_node rotate_right(p_node root){
    p_node new_root = root->left;

    root->left = new_root->right;
    new_root->right = root;
    new_root->color = root->color;
    root->color = RED;
    return new_root;
}

int up_red(p_node root){
    root->color = RED;
    root->right->color = BLACK;
    root->left->color = BLACK;
    return 0;
}

p_node insert_balanced(p_node root, long int element){
    p_node new_elem;
    // Insertion criteria since we're using a recursive algorithm
    if (root == NULL){
        new_elem = malloc(sizeof(node));
        new_elem->element = element;
        new_elem->left = NULL;
        new_elem->right = NULL;
        new_elem->color = RED;
        new_elem->frequency = 1;
        return new_elem; // our insertion function will always return the tree root, but since we're going to
        }                // "walk through" the left and the right stocking in root->left and root->right
                         // recursively, when we finally need to insert the new element its fine to return
                          // new element directly  

        // goes to the right or left recursively and accordingly to a binary search tree condition
        if (element > root->element)
            root->right = insert_balanced(root->right, element);
        else
            root->left = insert_balanced(root->left, element);

        // we need a step further from a simple BST to correct the tree so it stays in the form
        // Left-Leaning Red Black tree

        // For a LLRB tree, a red node can ONLY be the left children of its father, so if the we have a situation
        // where the left children is black and the right children is red, we need to rotate the tree/subtree to the left
        // so the left children will become red instead of the right one
        if (is_red(root->right) && is_black(root->left))
            root = rotate_left(root);
        // another rule for LLRB trees is that, if a node is red, both of its children MUST BE BLACK
        // and since we already verified the possibility of having a "red children on the right",
        // we can verify if two consecutive nodes in the left are red, and if they are, we rotate to the
        // right to solve this problem
        if (is_red(root->left) && is_red(root->left->left))
            root = rotate_right(root);
        // now, we solve the color's problem by "upping the red". The root problem (the original root must be black)
        // will be solved afterwards. This way we will ensure that the color distribution is as it must be for a LLRB tree
        if (is_red(root->left) && is_red(root->right))
            up_red(root);
        return root;
    // IT IS VERY IMPORTANT TO SEE THAT, NONE OF THE OPERATIONS THAT WE PERFORMED TO CORRECT THE TREE
    // WILL CHANGE THE BLACK HEIGHT PROPERTIE NOR THE SEARCHING PROPERTIE OF A LLRB TREE
}

// Correcting the problem that our root MUST ALWAYS be black
p_node insert_LLRBtree(p_node root, long int element){
    root = insert_balanced(root, element);
    root->color = BLACK;
    return root;
}

// Search an element in the BST
p_node search_element(p_node root, long int element){
    while (root != NULL && element != root->element)
        if (element < root->element)
            root = root->left;
        else
            root = root->right;
    return root;
}

// Print using the post-order
int print_post_order(p_node root){
    if (root != NULL){
        print_post_order(root->left);
        print_post_order(root->right);
        printf("%ld/%d///",root->element,root->frequency);
    }
    return 0;
}

// Cleaning a BST
void clean_BST(p_node root){
    if (root != NULL){
        clean_BST(root->left);
        clean_BST(root->right);
        free(root);
    }
}

// Verify repetition
int verify_repeat(p_node *root, long int element){
    p_node r = *root;
    if (r == NULL){
        return 0;
    }
    p_node elem = search_element(r, element);
    if (elem != NULL){
        elem->frequency = elem->frequency + 1;
        return 1;
    }
    return 0;
}

// This function will help us to find the quantity of numbers that shall be removed from
// the list so it can become a "lista legal"
// Basically, we need to think about the difference between the frequency and the real number

void remove_to_become_legal(p_node root, int *min){
    
    if (root != NULL){
        long int diff = root->frequency - root->element;
        // if the difference is < 0, we need to remove ALL the terms with value root->element
        if (diff < 0)
            *min = *min + root->frequency;
        // if the difference is > 0, we can remove the difference
        if (diff > 0)
            *min = *min + diff;
        // recursion
        if (root->left != NULL){
            remove_to_become_legal(root->left, min);
        }
        if (root->right != NULL){
            remove_to_become_legal(root->right, min);
        }
    }
}


int main(){

    int n, k, repeat, operation;
    int min = 0;
    long int element, j, m;
    p_node root = NULL;

    scanf("%d %d", &n, &k);
// Scanning the data veryfing if the number that we are trying to insert is repeated
// if it is, we sum 1 in the frequency. Note that we are giving the root's address to the function
// verify_repeat
    for (int l = 0; l < n; l++){
        scanf("%ld", &element);
        repeat = verify_repeat(&root, element);
        // if we have no repetition we can insert it normally
        if (repeat == 0)
            root = insert_LLRBtree(root,element);
    }

    for (int l= 0; l < k; l++){
        scanf("%d", &operation);
        // Insert a number such as we did with the entries
        if (operation == 1){
            scanf("%ld", &j);
            repeat = verify_repeat(&root, j);
            if (repeat == 0)
                root = insert_LLRBtree(root, j);
        }
        // print the number of repetitions
        else if (operation == 2){
            scanf("%ld", &m);
            p_node repet_times = search_element(root, m);
            if (repet_times != NULL)
                printf("%d\n",repet_times->frequency);
            else
                printf("%d\n", 0);
        }
        // use function to print how many elements we could remove so it can become a "lista legal"
        else if (operation == 3){
            remove_to_become_legal(root, &min);
            printf("%d\n", min);
            min = 0;
        }

    }
    // cleaning BST
    clean_BST(root);
    return 0;
}
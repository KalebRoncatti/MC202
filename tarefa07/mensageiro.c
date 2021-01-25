#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Observação : Eu aloquei estaticamente todos as strings pois estava tendo problemas com a alocação dinâmica
                na hora de dar free. Tentei resolver nas monitorias mas não consegui resolver com nenhum PAD,
                acabei optando por deixar estaticamente alocado. Mas a ideia de alocar dinamicamente é verificar
                o strlen(string) para cada string que será inserida e usar isso à nosso favor.*/

// BST with a position/order for a given token and the token
typedef struct node{
    int position;
    char token[10000];
    struct node *right;
    struct node *left;
} node;

// Pointer towards the BST
typedef node *p_node;

// Insert element/token algorithm
p_node insert_BST(p_node root, int position, char *token, int N_x_token){
    p_node new_tree;
    if (root == NULL){
        new_tree = malloc(sizeof(node));
        new_tree->left = NULL;
        new_tree->right = NULL;
        new_tree->position = position;
        strcpy(new_tree->token,token); 
        return new_tree;
    }
    // Call the function recursively so it can still return the original root
    if (position < root->position){
        root->left = insert_BST(root->left, position, token, N_x_token);
    }
    else{
        root->right = insert_BST(root->right, position, token, N_x_token);
    }
    return root;
}

// Removing sucessor. This algorithm is used for the case where we want to remove
// an element from a tree and this element has two subtrees, a left one and a right one
int remove_suc(p_node root){
    p_node parent = root;
    p_node sucessor = root->right;

    while (sucessor->left != NULL){
        parent = sucessor;
        sucessor = sucessor->left;
    }

    if (parent->left == sucessor){
        parent->left = sucessor->right;
    }
    else{
        parent->right = sucessor->right;
    }

   root->position = sucessor->position;
   strcpy(root->token,sucessor->token);
   free(sucessor);
   return 0;
}

// Removing a given number from the binary tree
p_node remove_BST(p_node root, int position){
	p_node current = root;
	if (current == NULL){
		return NULL;
	}
    // if the number that we are searching is lower than the tree, go to the left recursively
	if (position < current->position){
		current->left = remove_BST(current->left, position);
	}
    // else: if it is bigger, go to the right recursively
	else{if (position > current->position){
		current->right = remove_BST(current->right,position);					
		}
        // if we found it we need to verify if we have subtrees in the left or in the right
        // if we have no subtrees, we return the side that has a subtree
        // if we have subtrees, we remove the sucessor in the right
		else{if (current->left == NULL){
                p_node temp = current->right;
                free(current);
                return temp;
			}
            else{if (current->right == NULL){
                    p_node aux2 =  current->left;
                    free(current);
                    return aux2;
                }
                else{
                    remove_suc(current);
                }
             }
		}
	}
    return root;
}

// Search a given position in the BST
p_node search_position(p_node root, int position){
    while (root != NULL && position != root->position)
        if (position < root->position)
            root = root->left;
        else
            root = root->right;
    return root;
}

/* The general idea for the next two functions is: to find three numbers such that a + b + c = autority
   we need to vary a and b and use the binary search to find c. To do it using binary trees we need to
   use Recursion. The best way of perfoming this:
        - We need to find something that the sum gives the value of AUTORITY. We then fix a value (b) and find a number (AUTORITY - b)
        using a binary search.
        - Now, we need to find something that the sum gives (AUTORITY - b). So we fix a value (a) and find a number (AUTORITY - b - a)
        using a binary search.
        - When we will go through the list by using inorder,pre-order or post-order, it will recursively vary the values of a and b
        until we find the good c such that (a + b + c) = AUTORITY.
    Note that, if I had used the inorder algorithm, the binary tree would be responsible itself for getting the the values of the
    triade in order. However, I used a pre-order algorithm because for me it makes more sense look to the root first. Because of this,
    I had to "pick by hand" the right order of triade elements to be concatenated in a new string using "verify triade"

    - root and root_original are the fixed root values. 
    - b is the number fixed in triade so we'll search a such that a + b + c = autority
    - autor_minus_b is the value of (autority - b) that will come from the triade function
    - root_a is the actual element of the tree, we will go through the tree using this variable
    - *p_first is a pointer points toward the first element of the triade. I used the "call by reference" technique so I could
    pass the memory allocated place instead of the variable and things would change globally 
*/
p_node pair(p_node root, int b, int autor_minus_b, p_node root_a, p_node root_original, p_node *p_first){
    p_node c = NULL;
    if (root_a != NULL){

        int a = root_a->position;
        c = search_position(root_original, autor_minus_b - a);
        if (c != NULL){
            *p_first = root_a;
            return c;
        }
        else{
            p_node p_left = pair(root, b, autor_minus_b, root_a->left, root_original, p_first);
            if (p_left != NULL)
                return p_left;
                    
            p_node p_right = pair(root, b, autor_minus_b, root_a->right, root_original, p_first);
            if (p_right != NULL)
                return p_right;
        }
    }
    return NULL;
}

/*  - root is where we are going to "walk" in the tree.
    - autority number is the actual number of the autority.
    - root original is the real root of the tree that will be fixed.
    - *p_first is a pointer of a pointer that points toward the first element of the triade.
    - *p_second is a pointer of a pointer that points toward the second element of the triade.
    - The function will return p_third for the third element of the triade.
*/
p_node triad(p_node root, int autority_number, p_node root_original, p_node *p_first, p_node *p_second){
    p_node c = NULL;

    if (root != NULL){
        int b = root->position;

        c = pair(root, b, autority_number - b, root_original, root_original, p_first);
        if (c != NULL){
            *p_second = root;

            return c;
        }
        else{
            p_node t_left = triad(root->left, autority_number, root_original, p_first, p_second);
            if (t_left != NULL)
                return t_left;
                    
            p_node t_right = triad(root->right, autority_number, root_original, p_first, p_second);  
            if (t_right != NULL)
                return t_right;
        }
    }
    return NULL;
}

// Print using the post-order
int print_post_order(p_node root){
    if (root != NULL){
        print_post_order(root->left);
        print_post_order(root->right);
        printf("%d/",root->position);
    }
    return 0;
}

// Print strings inorder
int print_inorder(p_node root){
    if (root != NULL){
        print_inorder(root->left);
        printf("%s",root->token);
        print_inorder(root->right);
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

// Swaping two pointers of pointer
void swap(p_node *p1, p_node *p2){
    p_node aux = *p1;
    *p1 = *p2;
    *p2 = aux;
}

// Verify if we have the right order
void verify_triad(p_node *p_first, p_node *p_second, p_node *p_third){

    p_node aux, aux_2, aux_3;
    aux = *p_first;
    aux_2 = *p_second;
    aux_3 = *p_third;
 
    if (aux->position > aux_3->position){
        aux = *p_third;
        aux_3 = *p_first;
        swap(p_first, p_third);
    }

    if (aux->position > aux_2->position){
        aux = *p_second;
        aux_2 = *p_first;
        swap(p_first, p_second);
    }

    if (aux_2->position > aux_3->position){
        aux_2 = *p_third;
        aux_3 = *p_second;
        swap(p_second, p_third);
    }
}

int main(){
    int m, n, position, autority_number;
    char token[10000];
    // scan m and n till we find EOF
    while (scanf("%d %d", &m, &n) != EOF) {
        // p_fst, sec and trd will be used to find the triade
        p_node root = NULL;
        p_node p_trd = NULL;
        p_node p_fst, p_sec;

        // scan the number and the token
        // insert in the tree
        for (int k = 0; k < m; k++){
            scanf("%d \"%[^\"]\"",&position, token);
            root = insert_BST(root, position, token, 5);
        }
        

        for (int k = 0; k < n; k++){
            // scan the autority
            scanf("%d", &autority_number);

            // search the triad for this autority
            p_trd = triad(root, autority_number, root, &p_fst, &p_sec);

            // verify if p_fst,p_sec and p_trd are sorted in the crescent order
            // otherwise we would not be able to concatenate the tokens
            // note that, if my triad and pair algorithms were done inorder instead of preorder
            // I would had solve this problem
            verify_triad(&p_fst,&p_sec,&p_trd);

            // Concatenated char
            char new_token[10000];
            strcpy(new_token,p_fst->token);
            strcat(new_token,p_sec->token);
            strcat(new_token,p_trd->token);

            // Insert this char along with its number (autority) in the root
            root = insert_BST(root, autority_number,new_token, 0);

            // define the numbers that will be removed
            // it is important to define this int's before feeding remove_BST directly because
            // we could lose reference for the pointers p_fst, p_sec and p_trd.
            int first = p_fst->position;
            int second = p_sec->position;
            int third = p_trd->position;

            // Remove the three elements form the tree
            root = remove_BST(root, first);
            root = remove_BST(root, second);
            root = remove_BST(root, third);
    
        }
        // print and clean tree
        print_inorder(root);
        printf("\n");
        clean_BST(root);
    }

    return 0;
}
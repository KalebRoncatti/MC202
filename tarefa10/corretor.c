#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 1003 // Number of possible mappings for the hash function

/* 
Remark: Note that, the code must function for all values of M. However, let's say that my algorithm
is not the best that one could build. This way, for some M values the code will not pass in some tests
because of time over the limit allowed by the bot at GitLab
*/

// Creating the "linked list node" for our hash
typedef struct node{
    char word[26];  // each word from our dictionary
    int data;  // it will be explained later
    struct node *nxt; // next node
} node;

// Creating a pointer towards a node
typedef node *p_node;

// Creating our hash table
typedef struct{
    p_node v[M];
} hash;

// pointer towards a hash
typedef hash *p_hash;

// Using the basis 256 to map our words into integers. We could use other basis, the idea is that
// each word will have a mapping between 0 and M and we will add this word to the position s in the
// hash. If there are already some elements there we will need to add in the beginning of the linked list
int hash_map(char *word){
    int s = 0;
    for (int k = 0; k < strlen(word); k++)
        s = (256*s + word[k]) % M;
    return s;
}

// Creating a hash of linked lists with M elements
p_hash create_hash(){
    p_hash h = malloc(sizeof(hash));
    for (int k = 0; k < M; k++){
        h->v[k] = calloc(1,sizeof(node));
    }
    return h;
}

// Adding an element to the beginning of a linked list
p_node add_elem_LL(p_node list, int data, char *word){
    p_node new;
    new = malloc(sizeof(node));
    new->data = data;
    strcpy(new->word, word);
    new->nxt = list;
    return new;
}

// Inserting an element to the hash
p_hash insert_hash(p_hash h, char *word, int data){
    int map = hash_map(word);
    h->v[map] = add_elem_LL(h->v[map], data, word);
    return h;
}

// cleaning a linked list from memory
void clean_linked_list(p_node number){
    if (number != NULL){
        clean_linked_list(number->nxt);
        free(number);
    }
}

/* 
In this lab we need to verify if a given word has:
    - 0 errors
    - 1 error
    - 2 or more errors
So, we need to: 
    - Detect if the word is equal something that we have already in the dictionary (0 errors)
    - Detect if there is exactly one mistake (1 error)
    - If we are not in the two first cases above, we have the third case (2 or more errors). It means
    that we do not need to detect directly the errors for this case.

Our approach to solve these cases were:
    - 0 errors: if the word is exactly the same, we will find the mapping and strcmp will result in zero
    - 1 error:
        - We can insert some variations of the word in the dictionary that could result in exactly one problem
        - These cases are described in the two following cases
    - 2 errors:
        - otherwise
*/

/*
 This function will add some variations of a word that could engender exactly one error,
 for example:
    - word: casa
        - add: asa, csa, cas 
    - We will then add these variations to the dictionary but we will mark these words 
    (with the data attribute of the hash) in a way that we can afterwards detect that 
    we have found 1 error. 
*/
p_hash insert_var_dic(p_hash h, char *word){
    char string[26];

    for (int k = 0; k < strlen(word); k++){
        int m = 0;
        for (int l = 0; l < strlen(word); l++){
            if (l != k){
                string[m] = word[l];
                m++;
            }
        }
        string[m] = '\0';
        h = insert_hash(h, string, 0);
    }

    return h;
}

/*
 This function will add other variations of a word that could engender two errors (but a very common one),
 for example:
    - word: casa
        - add: acsa, csaa, caas
    - We will then add these variations to the dictionary but we will mark these words 
    (with the data attribute of the hash) in a way that we can afterwards detect that 
    we have found 2 error. 
*/
p_hash insert_inv_dic(p_hash h, char *word){
    char string[26];
    char aux;
    char aux2;
    for (int i = 0; i < strlen(word)-1; i++){
        int m = 0;
        if (word[i] != word[i+1]){
            aux = word[i];
            aux2 = word[i+1];
            for (int l = 0; l < strlen(word); l++){
                if (l != i){
                    if (l != i + 1){
                        string[m] = word[l];
                        m++;
                    }
                }
                else{
                    string[m] = aux2;
                    string[m+1] = aux;
                    m = m + 2;
                }
            }
        }
        string[m] = '\0';
        h = insert_hash(h, string, 3);
    }
    return h;
}

// Searching a word in the hash and verifying if we have zero, one, or two/more errors.
int search_hash(p_hash h, char *word){
    // we will first map the word. Remember that we have added A LOT OF VARIATIONS IN OUR DICTIONARY
    // that are not right written, but WE HAVE MARKED WHEN THIS HAPPENS
    int map = hash_map(word);
    // we are going to walk in the linked lists, so we need to fix a temporary. Once we walked in a
    // linked list we cannot come back, so let's walk on this temp
    p_node temp = h->v[map];

    while (temp != NULL){
        // compare the words
        int comp = strcmp(temp->word, word);
        // if we find the word and the data attribute is one, WE TYPED THE RIGHT WORD !! BRAVO !
        if (comp == 0 && temp->data == 1)
            return 0;
        // if we find the word and the data attribute is zero, WE FOUND A VARIATIONS WITH ONE ERROR THAT
        // WE HAVE MARKED
        if (comp == 0 && temp->data == 0)
            return 1;
        // if we find the word and the data attribute is three, WE FOUND A VARIATION WITH TWO ERRORS
        // THAT WE HAVE MARKED
        if (comp == 0 && temp->data == 3)
            return -1;
        temp = temp->nxt;
    }

    /* 
    if we got here, it means that we still did not find the word nor its variations in the dictionary,
    so we have now two options:
        - We are typing a random word that is not in the dictionary
        - We have exactly one error and are typing a word that has strlen(word) bigger than the right 
        one in the dictionary. To solve this we need to test the variations of this word and search in the
        dictionary
    */

   // searching the variations, for example:
   // if word = carsa and the right one in the dictionary is casa
   // we need to test such as we added the variations with one error before: arsa, crsa, casa
    char string[26];
    for (int k = 0; k < strlen(word); k++){
        int m = 0;
        for (int l = 0; l < strlen(word); l++){
            if (l != k){
                string[m] = word[l];
                m++;
            }
        }
        string[m] = '\0';
        int map = hash_map(string);
        temp = h->v[map];
        
        /* now, if we compare temp->word with the string variation and it is zero, it means 
         that we found the word in the dictionary. here we will also be able to detect errors such as:
            - The right word in the dictionary is : casa, and we typed: caca
            - casa variations with one error: asa, csa, caa, cas
            - caca variations with one element less: aca, cca, caa, cac
            Note that strcmp(cac,cac) == 0.
            // this only works because we have already remove the cases were we have mistakes of the type:
                - The right word is: casa and we type acsa
                - If we did not remove this case above, the next logical proposition would consider this
                error as one error.
        */

        while (temp != NULL){
            if (strcmp(temp->word,string) == 0)
                return 1;
            temp = temp->nxt;
        }
    }
    // otherwise, there are two or more errors
    return -1;
}


int main(){
    int n; // number of words in the dictionary
    int q; // number of words that we will type
    char word[26];
    p_hash dic_hash = create_hash();
    int word_search;

    scanf("%d %d", &n, &q); // scan numbers

    for (int k = 0; k < n; k++){
        scanf("%s", word); // scan a word
        // insert all variations (case one error) 
        dic_hash = insert_var_dic(dic_hash, word); 
        // insert all variations (case two errors) 
        dic_hash = insert_inv_dic(dic_hash, word); 
        // insert the right word in the dictionary and set its data as 1 to make the difference between
        // the cases above 
        dic_hash = insert_hash(dic_hash, word, 1);
        
    }
    
    // loop in the number of words that we want to verify
    for (int i = 0; i < q; i++){
        scanf("%s", word);
        // search for errors in the hash
        word_search = search_hash(dic_hash, word);

        switch (word_search){
            case 0:
            printf("verde\n");
            break;

            case 1:
            printf("amarelo\n");
            break;

            case -1:
            printf("vermelho\n");
            break;
        }
    }
    // cleaning hash
    for (int k = 0; k < M; k++){
        clean_linked_list(dic_hash->v[k]);
    } 
    free(dic_hash);

    return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Creating the linked list
typedef struct node{  
    int data;
    struct node *nxt;
} node;

// Pointer that points towards a node
typedef struct node *p_node; 

// Creating empty list
p_node create_empty_list(){
    return NULL;
}

// cleaning a linked list from memory
void clean_linked_list(p_node number){
    if (number != NULL){
        clean_linked_list(number->nxt);
        free(number);
    }
}

// Adding new CHAR elements to the list
p_node add_element(p_node list, char element){
    p_node new;
    new = malloc(sizeof(node));
    new->data = element;
    new->nxt = list;
    return new;
}
// Adding new INT elements to the list
p_node add_element_int(p_node list, int element){
    p_node new;
    new = malloc(sizeof(node));
    new->data = element;
    new->nxt = list;
    return new;
}

// Reading entry 1 in the linked list form
p_node read_entry1(p_node number_1){
    number_1 = create_empty_list();
    char elem;
    while(1){
        scanf("%c", &elem);
        if (elem == ' '){
            break;
        }
        if (elem != '\n'){ // When we'll select the operation we'll tap ENTER (\n). This \n element would be included in the list
            number_1 = add_element(number_1, elem);
        }
    }
    return number_1;
}

// Reading entry 2 in the linked list form
p_node read_entry2(p_node number_2){
    number_2 = create_empty_list();
    char elem;
    while(1){
        scanf("%c", &elem);
        if (elem == '\n'){
            break;
        }
        number_2 = add_element(number_2, elem);
    }
    return number_2;
}

// Printing linked list of char elements
int print_linked_list(p_node list){
    if (list != NULL){
        printf("%c \n", list->data);
        print_linked_list(list->nxt);
    }
    return 0;
}

// Inverting a string
void invert_string(char string[]){
    int k, i, j;
    for (i = 0, j = strlen(string)-1; i < j; i++, j--){
        k = string[i];
        string[i] = string[j];
        string[j] = k;
    }
}

// Printing linked list of int elements recursively
int print_linked_list_intrec(p_node list){
    if (list != NULL){
        printf("%d", list->data);
        print_linked_list_intrec(list->nxt);
    }
    return 0;
}

// copying a linked list
p_node copy_linked_list(p_node number){
    p_node new_list, auxiliar; 
    new_list = malloc(sizeof(node));
    new_list->data = number->data;
    auxiliar = new_list;
    number = number->nxt;
    while (number != NULL){
        auxiliar->nxt = malloc(sizeof(node));
        auxiliar = auxiliar->nxt;
        auxiliar->data = number->data;
        number = number->nxt;
    }
    auxiliar->nxt = NULL;
    return new_list; 
}

// Printing linked list of int elements eliminating unnecessary zeros
int print_linked_list_int(p_node number){
    p_node current, aux;
    int acc = 0;
    int acc1 = 0;
    aux = copy_linked_list(number);

    while(number != NULL){
        if (number->data == 0){
            acc = acc + 1;
        }
        number = number->nxt;
    }
    while(aux != NULL && aux->data == 0){
        acc1 = acc1 + 1;
        aux = aux->nxt;
    }
    if (acc == acc1 && acc != 0 && aux == NULL){
        printf("%d",0);
    }
    else{
        for (current = aux; current != NULL; current = current->nxt){
            printf("%d",current->data);
        }
    }
    clean_linked_list(aux);
    return 0;
}

// inversing a linked list
p_node invert_linked_list(p_node number){
    p_node current, aux;
    p_node inv = NULL;
    current = number;
    while (current != NULL){
        aux = current;
        current = aux->nxt;
        aux->nxt = inv;
        inv = aux;
    }
    return inv;
}

// Verify if one number is bigger than the other
// if number_1 >= number_2 it returns 1
// if number_1 < number_2 it returns 0
int verify_signal(p_node number_1, p_node number_2){
    char string1[26] = {'\0'};
    char string2[26] = {'\0'};
    int i = 0;
    while (number_1 != NULL || number_2 != NULL){
        if (number_1 == NULL && number_2 != NULL){
            string1[i] = 0;
            string2[i] = number_2->data;
            number_2 = number_2->nxt;
        }
        if (number_1 != NULL && number_2 == NULL){
            string1[i] = number_1->data;
            string2[i] = 0;
            number_1 = number_1->nxt;
        }
        if (number_1 != NULL && number_2 != NULL){
            string1[i] = number_1->data;
            string2[i] = number_2->data;
            number_1 = number_1->nxt;
            number_2 = number_2->nxt;
        }
        i = i+1;
    }
    invert_string(string1);
    invert_string(string2);
    if (strlen(string1) > strlen(string2)){
        return 1;
    }
    if (strlen(string1) < strlen(string2)){
        return 0;
    }
    for (int k = 0; k < strlen(string1); k++){
        int a = string1[k] - string2[k];
        if (a > 0){
            return 1;
        }
        if (a < 0){
            return 0;
        }
    }
    return 1;
}

// sum of big CHAR numbers using the algorithm that we use by hand
// value is one auxiliar variable that helps us to verify if we should "subir o 1 para a esquerda"
p_node sum_function(p_node number_1, p_node number_2){
    int aux, aux2, n1, n2;
    int value = 0;
    p_node sum_new;
    sum_new = create_empty_list();
    // verify if we are still looking at a number and take into account the cases where one number has more elements than the other
    // These numbers are char's and when converted to int's they become another number accordingly to the ASCII table, that's why
    // we must subtract '0'
    while (number_1 != NULL || number_2 != NULL){
        if (number_1 == NULL && number_2 != NULL){
            n1 = 0;
            n2 = number_2->data - '0';
        }
        if (number_2 == NULL && number_1 != NULL){
            n1 = number_1->data - '0';
            n2 = 0;
        }
        if (number_1 != NULL && number_2 != NULL){
            n1 = number_1->data - '0';
            n2 = number_2->data - '0';
        }
        aux = n1+n2;
        if (aux >= 10){
            aux2 = aux - 10;
            if (value == 1){
                aux2 = aux2 + 1;
            }
            sum_new = add_element_int(sum_new, aux2);
            value = 1;
        }
        else{
            if (value == 1){
                aux = aux + 1;
            }
            if (aux == 10){
                sum_new = add_element_int(sum_new, 0);
                value = 1;
            }
            if (aux < 10){
                sum_new = add_element_int(sum_new, aux);
                value = 0;
            }
        }
        // if statetment's that take into account when the numbers have a different number of elements
        if (number_1 == NULL && number_2 != NULL){
            number_2 = number_2->nxt;
        }
        if (number_1 != NULL && number_2 == NULL){
            number_1 = number_1->nxt;
        }
        if (number_1 != NULL && number_2 != NULL){
            number_1 = number_1->nxt;
            number_2 = number_2->nxt;
        }
    }
    // statement that add one if the "leftest sum" by hand is bigger than 10
    if (value == 1){
        sum_new = add_element_int(sum_new, 1);
    }
    return sum_new;
}

// sum of big INT numbers using the algorithm that we use by hand
// value is one auxiliar variable that helps us to verify if we should "subir o 1 para a esquerda"
p_node sum_function_int(p_node number_1, p_node number_2){
    int aux, aux2, n1, n2;
    int value = 0;
    p_node sum_new;
    sum_new = create_empty_list();
    // verify if we are still looking at a number and take into account the cases where one number has more elements than the other
    // These numbers are char's and when converted to int's they become another number accordingly to the ASCII table, that's why
    // we must subtract '0'
    while (number_1 != NULL || number_2 != NULL){
        if (number_1 == NULL && number_2 != NULL){
            n1 = 0;
            n2 = number_2->data;
        }
        if (number_2 == NULL && number_1 != NULL){
            n1 = number_1->data;
            n2 = 0;
        }
        if (number_1 != NULL && number_2 != NULL){
            n1 = number_1->data;
            n2 = number_2->data;
        }
        aux = n1+n2;
        if (aux >= 10){
            aux2 = aux - 10;
            if (value == 1){
                aux2 = aux2 + 1;
            }
            sum_new = add_element_int(sum_new, aux2);
            value = 1;
        }
        else{
            if (value == 1){
                aux = aux + 1;
            }
            if (aux == 10){
                sum_new = add_element_int(sum_new, 0);
                value = 1;
            }
            if (aux < 10){
                sum_new = add_element_int(sum_new, aux);
                value = 0;
            }
        }
        // if statetment's that take into account when the numbers have a different number of elements
        if (number_1 == NULL && number_2 != NULL){
            number_2 = number_2->nxt;
        }
        if (number_1 != NULL && number_2 == NULL){
            number_1 = number_1->nxt;
        }
        if (number_1 != NULL && number_2 != NULL){
            number_1 = number_1->nxt;
            number_2 = number_2->nxt;
        }
    }
    // statement that add one if the "leftest sum" by hand is bigger than 10
    if (value == 1){
        sum_new = add_element_int(sum_new, 1);
    }
    return sum_new;
}

p_node subtraction_function(p_node number_1, p_node number_2){
    // here the acc variable was used to "borrow from other numbers"
    p_node sub;
    int n1, n2, aux, aux2;
    int acc = 0;
    sub = create_empty_list();
    while (number_1 != NULL || number_2 != NULL){
        // being careful if one number has more elements than the other
        if (number_2 == NULL && number_1 != NULL){
            n1 = number_1->data - '0';
            n2 = 0;
        }
        if (number_1 != NULL && number_2 != NULL){
            n1 = number_1->data - '0';
            n2 = number_2->data - '0';
        }

        aux = n1 - n2;
        // algorithm to perform the subtraction
        if (aux < 0 && acc == 0){
            aux2 = aux + 10;
            sub = add_element_int(sub, aux2);
            acc = 1;
        }
        else{if(aux < 0 && acc == 1){
            aux2 = aux + 9;
            sub = add_element_int(sub,aux2);
            }
            else{if (acc == 0){
                    if (aux >= 0){
                        sub = add_element_int(sub,aux); 
                    }
                }
                if (acc == 1){
                    if (aux-1 >= 0){
                        sub = add_element_int(sub,aux-1);
                        acc = 0;
                    }
                    if (aux-1 < 0){
                        aux2 = aux - 1 + 10;
                        sub = add_element_int(sub,aux2);
                    } 
                }
            }
        }
        // if statetment's that take into account when the numbers have a different number of elements so we don't get segmentation fault
        if (number_1 != NULL && number_2 == NULL){
            number_1 = number_1->nxt;
        }
        if (number_1 != NULL && number_2 != NULL){
            number_1 = number_1->nxt;
            number_2 = number_2->nxt;
        }
    }
    return sub;
}

p_node product_function(p_node number_1, p_node number_2){
    p_node aux, help, prod, help_inv, prod_inv, auxiliar;
    int n1,n2, product, rest, div_int, acc;
    prod = create_empty_list();
    help = create_empty_list();
    acc = 0;

    while (number_2 != NULL){
        div_int = 0;
        auxiliar = copy_linked_list(number_1); // duplicate the list
        aux = auxiliar;  // duplicate the reference (they both point to the same place) 
                         //so when we run along the list we won't lose reference for freeing purposes
        while (aux != NULL){
            n1 = aux->data - '0';
            n2 = number_2->data - '0';
            product = n1*n2;
            rest = (product + div_int)%10;
            div_int = (product + div_int)/10;
            help = add_element_int(help,rest);
            aux = aux->nxt;
        }
        clean_linked_list(auxiliar); // because if we clean aux, we will be cleaning NULL only
        acc = acc + 1;
        if (div_int != 0){
            help = add_element_int(help,div_int);
        }
        // the linked lists help and prod are now the GOOD order,
        // when we do our operations, we need to take things backwards,
        // to result in something on the right order, this way we need
        // to re-invert these linked lists
      //print_linked_list_intrec(help);
    //printf("\n");
        help_inv = invert_linked_list(help);
        prod_inv = invert_linked_list(prod);
        prod = sum_function_int(help_inv,prod_inv);
        clean_linked_list(help_inv);
        clean_linked_list(prod_inv);
        help = create_empty_list();
        for (int k = 0; k < acc; k++){
            help = add_element_int(help, 0);
        }   
        number_2 = number_2->nxt;
    }
    clean_linked_list(help);
    clean_linked_list(aux);
    return prod;
}

int main(){
    int n_cases;
    char operation;
    p_node number_1, number_2, sum, sub, prod;
    scanf("%d", &n_cases);

    for (int i = 0; i < n_cases; i++){
        scanf(" %c", &operation);
        // sum of big numbers
        if (operation == '+'){
            number_1 = read_entry1(number_1);
            number_2 = read_entry2(number_2);
            sum = sum_function(number_1, number_2);
            print_linked_list_int(sum);
            printf("\n");
            clean_linked_list(sum);
            clean_linked_list(number_1);
            clean_linked_list(number_2);
        }
        // subtraction of big numbers
        if (operation == '-'){
            number_1 = read_entry1(number_1);
            number_2 = read_entry2(number_2);
            // analyse if number_1 > number_2 or not
            if (verify_signal(number_1,number_2) == 1){
                sub = subtraction_function(number_1, number_2);
                print_linked_list_int(sub);
                printf("\n");
                clean_linked_list(sub);
                clean_linked_list(number_1);
                clean_linked_list(number_2);
            }
            else{
                sub = subtraction_function(number_2, number_1);
                print_linked_list_int(sub);
                printf("\n");
                clean_linked_list(sub);
                clean_linked_list(number_1);
                clean_linked_list(number_2);
            }
        }
        // product of big numbers
        if (operation == '*'){
            number_1 = read_entry1(number_1);
            number_2 = read_entry2(number_2);
            prod = product_function(number_1,number_2);
            print_linked_list_int(prod);
            printf("\n");
            clean_linked_list(prod);
            clean_linked_list(number_1);
            clean_linked_list(number_2);
        }
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Gostaria de fazer algumas observações para os monitores que irão corrigir:
// - A ideia do lab foi bem bacana, porém, ao meu ver, as instruções estavam muito vagas a respeito de quais estruturas de dado usar.
// - Pede-se para usar deque que foi definido da seguinte forma "Um deque é uma generalização de fila que contém operações de inserção
//   e remoção tanto no início quanto do final. Cada uma dessas operações deve ser implementada em tempo constante".
// - Porém, não especificaram mais nada de como gostariam desses elementos estruturados, então a ideia deste código foi:
//   - Para ARMAZENAR as entradas: LISTA LIGADA SIMPLES ADAPTADA (sem null no fim e a lista anda para frente) de deques.
//   - Para INSERIR nas filas de atendimento: Apenas deques.
// - Porém, o deque que eu acabei usando é um deque circular com cabeça cuja estrutura será explicada ao longo do código.
// - Ao longo do código, quando não tinha mais tempo de voltar atrás, eu percebi que minha estrutura era ruim, pois
// na minha lista ligada eu só conseguia "andar para frente", então acabei tendo que compensar na lógica de programação

// - MAS NOTE QUE, MEUS DEQUES E SUAS OPERAÇÕES FORAM IMPLEMENTADAS EM O(1) COMO SOLICITADO.

// - EM POUCOS MOMENTOS (apenas com prioritários) EU TIVE QUE PERCORRER A LISTA LIGADA ADAPTADA PARA RESOLVER O PROBLEMA DA ESTRUTURA
// NÃO SER ADEQUADA PARA O PROBLEMA


// Creating the linked list with two reference pointers, next and before
typedef struct node{  
    int data;   // GETTING DATA: Type of doctor (will be used to gather the data)
                // INSERTING IN THE DOCTOR'S QUEUE: The "arrival position" of each person
    char name[51]; // Name (it will be used when a person will enter into a queue for a doctor)
    int card;  // normal or prioritario (it will be used when a person will enter into a queue for a doctor)
    struct node *nx; // next node
    struct node *bf; // before node
} node;
// Pointer that points towards a node
typedef struct node *p_node;


// Double-ended queue (deques) with a head, some information, and an auxiliar pointer 
typedef struct{
    p_node head;
} deq;
// Pointer that points towards a queue


// Linked list of deques to take data from different patients
// I adapted the structure of the linked list so it can go FORWARD
typedef struct pat{
    deq *deque;  
    char name[51]; 
    int finish; // if a person finished already all of its appointments
    int card; // card type (normal or prioritario)
    int position; // position in the arrival for sorting reasons
    int departure_time; // when all the appointments are finished
    struct pat *nxt;
} pat;


//                                           --------------------------------
//                                           |                              |
// Example for a deque with two elements:    -->      -->      -->       ----
//                                               HEAD     FIRST    SECOND
//                                           ---      <--      <--       <---
//                                           |                              |
//                                           --------------------------------


// Create a circular deque 
int create_circular_deq(deq *d){
    d->head = malloc(sizeof(node));
    d->head->nx = d->head;
    d->head->bf = d->head;
    return 0;
}

// Insert element in the end of the deque with O(1)
int insert_element_end(deq *d, int x, char *name, int card, int threshold){
    p_node new;
    new = malloc(sizeof(node));
    new->data = x;
    if (threshold == 1){
        strcpy(new->name, name);
        new->card = card;
    }
    new->nx = d->head;
    new->bf = d->head->bf;
    d->head->bf = new;
    new->bf->nx = new;
    return 0;
    }

// Insert element in the beginning of the deque with O(1)
int insert_element_beg(deq *d, int x, char *name, int card, int threshold){
    p_node new;
    new = malloc(sizeof(node));
    new->data = x;
    if (threshold == 1){
        strcpy(new->name, name);
        new->card = card;
    }
    new->bf = d->head;
    new->nx = d->head->nx;
    d->head->nx = new;
    new->nx->bf = new;
    return 0; 
}

// Remove element from the end of the deque with O(1)
int remove_element_end(deq *d){
    if ((d->head->nx) == d->head){
        return 0;
    }
    p_node erase;
    erase = d->head->bf;
    erase->bf->nx = d->head;
    d->head->bf = erase->bf;
    free(erase);
    return 0;
}

// Remove element from the beginning of the deque with O(1)
int remove_element_beg(deq *d){
    if ((d->head->nx) == d->head){
        return 0;
    }
    p_node erase;
    erase = d->head->nx;
    erase->nx->bf = d->head;
    d->head->nx = erase->nx;
    free(erase);
    return 0;
}

// Size of the deque (Here we cannot "escape" from O(n))
int size_deq(deq *d){
    p_node end = d->head->nx;
    int size = 0;
    while(end != d->head){
        size = size + 1;
        end = end->nx;
    }
    return(size);
}

// Clean our whole deque without memory leakage
void clean_deq(deq *d, int threshold){
    if (d->head->nx == d->head){
        free(d->head);
        free(d);
    }
    else{
        p_node end;
        end = d->head->nx;
        while (end != d->head){
            p_node erase = end;
            end = end->nx;
            free(erase);
        }
        free(d->head);
        free(d);
    }
}

// Print our deque from beginning
void print_deq_beg(deq *d){
    p_node end;
    end = d->head->nx;
    while (end != d->head){
        printf("%d /",end->data);
        end = end->nx;
    }
    printf("\n");
}

// This function will return us the type as integers (NORMAL = 0/PRIORITARIO = 1)
int card_type(char *type){
    if (type[0] == 'n'){
        free(type);
        return 0;
    } 
    if (type[0] == 'p'){
        free(type);
        return 1;
    }
    free(type);
    return 3;
}

// This function will add a new element to the ADAPTED LINKED LIST OF DEQUES
// Note that, when we will free all the allocated memory used in this linked list
// we won't find any NULL element, so we will need an accumulative variable
int insert_deq_end(pat *p){
    pat *new_p = malloc(sizeof(pat));
    new_p->deque = malloc(sizeof(deq));
    p->nxt = new_p;
    return 0;
}


// Cleaning the adapted linked list of deques without memory leakage
// We say threshold = 1 when we use the deques
//        threshold = 0 when we do not use the deques
void clean_list_deqs(pat *p,int pos,int threshold){
    pat *end_p;
    int i = 0;
    end_p = p;
    while (i < pos){
        pat *erase_p; 
        erase_p = end_p;
        end_p = end_p->nxt;
        if (threshold==1){
            clean_deq(erase_p->deque, 0);
        }
        // The way that we used it in the future for threshold = 0 will not allocate memory for the first element
        // of erase_p->deque and it was resulting in problems with valgrind
        if (threshold==0 && i != 0){
            free(erase_p->deque);
        }
        free(erase_p);
        i = i + 1;
    }
    // In my formulation (main function) we allocate more memory without need (one stack in the last turn of the loop while !=EOF)
    // The next two free's help solving this problem, "eliminating" the mallocs of sizeof(pat) and sizeof(deq) (symbol * at line 321)
    free(end_p->deque);
    free(end_p);
}

// This function will be useful to search a person with a given position in the original linked list of deques
void research_list(deq *speciality, pat *beg, int a){
    while(beg->position != a){
        beg = beg->nxt;
    }
    insert_element_beg(beg->deque, 0, beg->name, beg->card, 1);
}   

// Inserting in the doctor's queue: FOI NESTA FUNÇÃO QUE TIVE QUE COMPENSAR EM LÓGICA A ESTRUTURA RUIM QUE USEI
// In the main code we are going to count time per iteration, and the stopping criterion will be when ALL PEOPLE
// finished ALL of their appointments. Note that I will count time accordingly to the quantity of elements in the
// initial deque (the deque inside of the linked list)

// Variables:
//  aux -> reference for the first patient (linked list of deques)
//  auxiliar-> the current patient (linked list of deques)
//  speciality-> queue for a type of doctor equals to ID (deque)
//  size-> size of the doctor's queue
//  tot_doctors -> how many doctors we have in total for a speciality
// Essentially:

// - We will verify which doctor the patient wants to visit in a given moment
// - We will then separate in two cases: 
//      -If the quantity of patients that are already in the desired queue is equal or bigger than the maximal capacity
//      -If the quantity of patients in the desired queue is lower than the maximal capacity of a given speciality
// - Inside of each one of these cases, we will again separate in two cases:
//      - If the person is preferencial
//      - If the person is normal    
deq *insert_queue_doctor(pat *aux, pat *auxiliar, deq* speciality, int ID, int size, int tot_doctors){
    int position;  
    if(auxiliar->deque->head->nx->data == ID){
        if (size >= tot_doctors){
            if(auxiliar->card == 0){
                // Insert the patient's order in the END of the speciality queue 
                insert_element_end(speciality,auxiliar->position,auxiliar->name,auxiliar->card, 1);
                // Remove the appointment form the original queue (note that this queue is inside the linked list)
                remove_element_beg(auxiliar->deque);
                // Now, When people are inserted in the speciality queue and 
                // this queue was already in the maximal capacity for that speciality:
                //     - We are counting time adding 0 elements to the auxiliar->deque of a given person
                //     -Only when auxiliar->deque is null that the appointments are finished
                //     - We need to add the right quantity of zeros so the time will be counted right
                //     - We need the "plus 2" in the loop because we will be subtracting one appointment
                //      from each auxiliar->deque. 
                for(int k = 0; k < ((size-tot_doctors)/tot_doctors + 2); k++){
                    insert_element_beg(auxiliar->deque,0,auxiliar->name,auxiliar->card,1);
                }
            }
            if (auxiliar->card == 1){
                // Let's treat the case with preferential's
                pat *beg = aux;
                // Insert the patient's order in the BEGINNING of the speciality queue 
                insert_element_beg(speciality,auxiliar->position,auxiliar->name,auxiliar->card, 1);
                // Compute the new size of the speciality queue
                int size_new = size_deq(speciality);
                // Define a helper node that points towards each patient in the speciality queue
                p_node help = speciality->head->nx;
                // Remove one element from auxiliar deque and insert the first 0 to count time
                remove_element_beg(auxiliar->deque);
                insert_element_beg(auxiliar->deque,0,auxiliar->name,auxiliar->card,1);

                for (int k = 0; k <= size_new; k++){
                    // Now, things will be easier seen with an example:
                    // - We have two neurologists that can see only two people every 10 minutes:
                    //      -Suppose that we had 10 people waiting in the queue for a neurologest BEFORE inserting the preferential
                    //      -Now, when the NEW preferential person is inserted, we have 11 people and the present patient is the first
                    //      -Then, the previous 2nd, 4th, 6th, 8th and 10th will need to wait more 10 minutes
                    //      to see a neurologist, because they were all "moved in one position towards the end of the queue"
                    //      -We need to come back to the respective auxiliar->deque's to insert the 0 element's that counts the time
                    if (k == tot_doctors){
                        position = help->data;
                        research_list(speciality,beg,position);
                    }
                    if (k % tot_doctors == 0 && k != 0 && k != tot_doctors && k < size_new){
                        position = help->nx->data;
                        research_list(speciality,beg,position);
                    }
                    if (k != tot_doctors){
                        help = help->nx;
                    }
                }
            }
        }
        // If the size of the appointment's queue is lower than the maximal capacity for that speciality
        if (size < tot_doctors){
            // If we have a normal person we insert in the end
            if(auxiliar->card == 0){
                insert_element_end(speciality,auxiliar->position,auxiliar->name,auxiliar->card, 1);
            }
            // If we have a preferential one we insert in the beginning
            if (auxiliar->card == 1){
                insert_element_beg(speciality,auxiliar->position,auxiliar->name,auxiliar->card, 1);
            }
            // We remove the element from the beginning of auxiliar->deque and inser a 0 one that will be removed afterwards
            remove_element_beg(auxiliar->deque);
            insert_element_beg(auxiliar->deque,0,auxiliar->name,auxiliar->card,1);
        }
    }
    return speciality;
}

// Removing from the doctor's queue
deq *remove_queue_doctor(deq* speciality, int tot_doctors){
    for (int k = 0; k < tot_doctors; k++){
        remove_element_beg(speciality);
    }
    return speciality;
}
////////////////////////////////////////////////////////////////////////////////////////////////////qq
int main(){
    char quot; // first quotation mark
    pat *p = malloc(sizeof(pat));  // linked list of deques
    p->deque = malloc(sizeof(deq));
    pat *auxiliar; // auxiliar element that will point to the first patient so we don't lose reference
    int pos = 0; // number of patients
    auxiliar = p; // points to the first queue (first patient), so we do not lose reference

// While loop that scan's the first " and stops once we find an EOF
    while(scanf("%c", &quot) != EOF){
        int i = 0;
        char *type = (char *)calloc(20,sizeof(char));
        p->card = 0;
        // Scan name
        while(scanf("%c",&p->name[i])){
            if (p->name[i] == '"'){
                p->name[i] = '\0';
                break;
            }
            i = i + 1;
        }
        // Scan type (priority or normal)
        scanf("%s", type);
        p->card = card_type(type);
        p->finish = 0;

        // Position of each person for sorting purposes
        p->position = (pos+1);
        create_circular_deq(p->deque);
        char string_help[10] = "help";
        int counter = 0;
        // Read appointment's for a given person
        while(2){
            int x;
            char aux;
            scanf("%d%c",&x,&aux);
            counter = counter + 1;
            insert_element_end(p->deque, x, string_help, 0, 0);
            if (aux == '\n'){
                break;
            }
        }
        pos = pos + 1;
        insert_deq_end(p);  // *
        // Goes to the next element in the adapted linked list, to add the respective names, cards and deques
        p = p->nxt;
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Now that we have all the data stored in a linked lists of queues, we can work into the doctor's dynamic


pat *aux = auxiliar; // Element that points to the first element of the data (to the first patient)
pat *departure_time = malloc(sizeof(pat)); // We will compute the departure time for each person
pat *departure_time_aux = departure_time;  // points to the first element (the first person that will leave CECOM)

// Allocate the memory to each speciality
int time = 0;
deq *clinico = malloc(sizeof(deq));
deq *radio = malloc(sizeof(deq));
deq *enf = malloc(sizeof(deq));
deq *oto = malloc(sizeof(deq));
deq *pneumo = malloc(sizeof(deq));
deq *dermo = malloc(sizeof(deq));
deq *neuro = malloc(sizeof(deq));
deq *onco = malloc(sizeof(deq));
deq *psic = malloc(sizeof(deq));
// Create a circular deque
create_circular_deq(clinico);
create_circular_deq(radio);
create_circular_deq(enf);
create_circular_deq(oto);
create_circular_deq(pneumo);
create_circular_deq(dermo);
create_circular_deq(neuro);
create_circular_deq(onco);
create_circular_deq(psic);

int acc = 0; // Variable that will count how many people are getting out

// Loop will break when there will be no more doctor appointments
    while(2){

        int var = 0; // acc variable that will indicate when a deque is over

        // Within this loop we verify all the patients
        for (int k = 0; k < pos; k++){
            // statement to add 1 if we have no more appointments
            // we will finish everything when there will be no more appointments for any of the patients
            // (when all the elements for each auxiliar->deque will be removed)
            if(auxiliar->deque->head == auxiliar->deque->head->nx){
                var = var + 1;
                if (auxiliar->finish == 0){            
                    auxiliar->finish = 1;
                    strcpy(departure_time->name,auxiliar->name);

                    departure_time->departure_time = time;
                    if (var != pos){
                        insert_deq_end(departure_time);
                        acc = acc + 1;
                        departure_time = departure_time->nxt;
                    }
                 }
            }
            // If we finished, there is no need to search for more appointments
            if (auxiliar->finish == 1){
            }
            // Else, we try to insert the given person in the respective queue
            else{
                clinico = insert_queue_doctor(aux,auxiliar, clinico, 1, size_deq(clinico), 10);
                radio = insert_queue_doctor(aux,auxiliar, radio, 2, size_deq(radio), 2);
                enf = insert_queue_doctor(aux,auxiliar, enf, 3, size_deq(enf), 5);
                oto = insert_queue_doctor(aux,auxiliar, oto, 4, size_deq(oto), 3);
                pneumo = insert_queue_doctor(aux,auxiliar, pneumo, 5, size_deq(pneumo), 4);
                dermo = insert_queue_doctor(aux,auxiliar, dermo, 6, size_deq(dermo), 7);
                neuro = insert_queue_doctor(aux,auxiliar, neuro, 7, size_deq(neuro),2);
                onco = insert_queue_doctor(aux,auxiliar, onco, 8, size_deq(onco),1);
                psic = insert_queue_doctor(aux,auxiliar, psic, 9, size_deq(psic), 4);
            }
            // We remove an element from each auxiliar->deque to count the time
            remove_element_beg(auxiliar->deque);
            auxiliar = auxiliar->nxt;
        }
        // We remove the quantity of people that were answered from the speciality queue
        remove_queue_doctor(clinico,10);
        remove_queue_doctor(radio,2);
        remove_queue_doctor(enf,5);
        remove_queue_doctor(oto,3);
        remove_queue_doctor(pneumo,4);
        remove_queue_doctor(dermo,7);
        remove_queue_doctor(neuro,2);
        remove_queue_doctor(onco,1);
        remove_queue_doctor(psic,4);

        // come back to the first element on the linked list
        auxiliar = aux;
        // var == pos when the ID = 0 for all the patients, it means that all patients got in ALL their appointments already
        if (var == pos){
            break;
        }
        time = time + 10;
    }
    
    // Point again to the first person that will leave CECOM
    departure_time = departure_time_aux;
    // print everything
    for (int k = 0; k < pos; k++){
        printf("%02d:%02d %s\n",((departure_time->departure_time/60)+8),departure_time->departure_time%60,departure_time->name);
        if(k < (pos - 1)){
            departure_time = departure_time->nxt;
        }
    }

// Clean the linked list of deques and the speciality deques
    clean_list_deqs(aux,pos,1);
    clean_deq(clinico,1);
    clean_deq(radio,1);
    clean_deq(enf,1);
    clean_deq(oto,1);
    clean_deq(pneumo,1);
    clean_deq(dermo,1);
    clean_deq(neuro,1);
    clean_deq(onco,1);
    clean_deq(psic,1);
    clean_list_deqs(departure_time_aux,acc,0);

    return 0;
}
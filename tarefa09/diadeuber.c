#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defining our heap structure
typedef struct{
    char name[16]; // Customer name
    double stars;   // Customer evaluation (in starts)
    int x_dep, y_dep, x_arr, y_arr; // Coordinates of departure and arrival
} heap_element;

typedef struct {
    heap_element *v; // Pointer towards a heap element
    int n;            // Number of elements of the heap being occupied
    int size;        // maximum size of the heap
} heap;

// pointer towards a heap
typedef heap *p_heap;

// Creating a heap with maximum size
p_heap create_heap(int size){
    p_heap h = malloc(sizeof(heap));
    h->v = malloc(size * sizeof(heap_element));
    h->n = 0;
    h->size = size;
    return h; 
}

// Using the heap's structure, we can define some macros because our tree has univocal structure, so the parents
// and the children can be obtained using a general formula.
#define PARENT(k) ((k-1)/2) // Parent
#define C_LEFT(k) (2*k+1) // Left children
#define C_RIGHT(k) (2*k+2) // Right children

// Goes up in the heap verifying if the heap's properties are being maintened (parent star > children stars)
int up_heap(p_heap h, int pos){
    if (pos > 0 && h->v[PARENT(pos)].stars < h->v[pos].stars){

       // swap elements
        heap_element aux = h->v[pos];
        h->v[pos] = h->v[PARENT(pos)];
        h->v[PARENT(pos)] = aux;
       // go up recursively to fix the heap
        up_heap(h, PARENT(pos));
    }
    return 0;
}

// Goes down in the heap veryifing if the heap's properties are being maintened (parent star > children stars)
int down_heap(p_heap h, int pos){
    int bigger_children;
    if (C_LEFT(pos) < h->n){// Verifying if we are not 'getting out' the heap
        bigger_children = C_LEFT(pos); // if we aren't, sets bigger_children as the left one
        // Now, we need to do the same thing with the right children and, if we have one, we must verify
        // if is bigger than the other children star.
        if (C_RIGHT(pos) < h->n && h->v[C_LEFT(pos)].stars < h->v[C_RIGHT(pos)].stars) 
            bigger_children = C_RIGHT(pos);
        // Now that we know which child is bigger, we compare it to the parent and call the function recursively
        // so it will go through the whole heap
        if (h->v[pos].stars < h->v[bigger_children].stars){
            
            // swap elements
            heap_element aux = h->v[pos];
            h->v[pos] = h->v[bigger_children];
            h->v[bigger_children] = aux;

            // go down recursively to fix the heap
            down_heap(h, bigger_children);
        }
    }
    return 0;
}

// Inserting a new customer
int insert(p_heap h, heap_element he){
    h->v[h->n] = he;
    h->n++;
    up_heap(h, h->n - 1);
    return 0;
}

// Removing/Extracting the customer with bigger star evaluation
heap_element extract_max(p_heap h){

    heap_element elem = h->v[0];

    // swap the first element with the last one because the last one is the removed
    heap_element aux = h->v[0];
    h->v[0] = h->v[h->n - 1];
    h->v[h->n - 1] = aux;
  
    h->n--;
    // go down recursively to fix the heap
    down_heap(h, 0);
    return elem;
}

// changing priority
p_heap change_priority(p_heap h, int index, int value){
    if (h->v[index].stars < value){
        h->v[index].stars = value;
        up_heap(h, index);
    }
    else{
        h->v[index].stars = value;
        down_heap(h, index);
    }
    return h;
}

// manhattan distance
int d_manhattan(int x_dep, int y_dep, int x_arr, int y_arr){
    return abs(x_dep - x_arr) + abs(y_dep - y_arr);
}

// Removing/Extracting a customer with a given name

// The idea here is to reutilize the function extract_max; when we find the customer
// with a given name using strcmp, we set the number of stars for this customer as 100 
//(certainly it will become the max) and we then call extract_max so this element will be removed
heap_element remove_from_heap(p_heap h, char *name){
  
    int temp;
    heap_element elem = h->v[0];

    for (int k = 0; k < h->n; k++){
        temp = strcmp(name, h->v[k].name);
        if (temp == 0){
            h = change_priority(h, k, 1000);
            elem = extract_max(h);
            return elem;
        }
    }
    return elem;
}


int main(){
    heap_element temp;  // temporary element used to ADD NEW CUSTOMERS
    heap_element temp2; // temporary element used to EXTRACT THE PERSON THAT FINISHED ITS COURSE (extract maximal priority)
    heap_element temp3; // temporary element used to EXTRACT FROM THE HEAP WHEN A PERSON CANCEL ITS COURSE
    heap_element temp4; // temporary element used to STOCK THE PREVIOUS COURSE SO WE CAN:
                        //      - CALCULATE THE DISTANCE BETWEEN ONE CLIENT AND ANOTHER

    temp2.x_arr = 0;
    temp2.x_dep = 0;
    temp2.y_arr = 0;
    temp2.y_dep = 0;


    // action char
    char action;
    // creating our heap
    p_heap uber = create_heap(501);
    char name[16];    
    // quantity of people that canceled its course.
    int canceled = 0;
    // Distance that will generate profits (distance between arrival and departure coordinates for the same person)
    int profitable_dist = 0; 
    // Distance that the driver needs to travel between a customer and another one
    int dist_between_routes = 0;

    while(scanf("%c", &action) != EOF){
        
        // Adding a new customer
        if (action == 'A'){

            // scan in the temporary heap element
            scanf("%s %lf %d %d %d %d", temp.name, &temp.stars, &temp.x_dep, &temp.y_dep, &temp.x_arr, &temp.y_arr);
            // Insert in the uber heap
            insert(uber, temp);
            
            // this condition "mimetizes" the situation where the Uber Driver has only one customer and he is going on a course, 
            // we need to put the priority of this course above all the others to remove the right one afterwards. It means that, when we 
            // add a new customer and the number uber->n = 1, there are no more customers yet. So the driver goes in 
            // the direction of this customer regardless of the next customers that will eventually appear having a better evaluation
            if (uber->n == 1)
                uber = change_priority(uber, 0, 500);

            printf("Cliente %s foi adicionado(a)\n", temp.name);
        }

        // Finishing a course
        else if (action == 'F'){
            // Fixing the last finished course so we can calculate the distance between two customers afterwards
            temp4 = temp2;
            // Extract the maximal value. 
            temp2 = extract_max(uber);

            // Calculate the distances between two customers
            dist_between_routes = dist_between_routes + d_manhattan(temp2.x_dep, temp2.y_dep, temp4.x_arr, temp4.y_arr);
            // calculate the profitable distance (beginning till end for the same customer)
            profitable_dist = profitable_dist + d_manhattan(temp2.x_dep, temp2.y_dep, temp2.x_arr, temp2.y_arr);

            // Again, if we have one customer or more, we need to increase the priority the first customer that was in the position 0
            // so other new customers won't "steal" its position because of eventual better evaluation
            if (uber->n >= 1){
                uber = change_priority(uber, 0, 100);
            }
            printf("A corrida de %s foi finalizada\n", temp2.name);
        }

        // Canceling a course
        else if (action == 'C'){
            scanf("%s", name);
            temp3 = remove_from_heap(uber, name);
            printf("%s cancelou a corrida\n", temp3.name);
            // add one to the canceled so we can calculated the reimbursed value afterwards
            canceled = canceled + 1;
        }

    }
    // the km_total is the distance between clients + distance that we need to go through for each client
    int km_total = profitable_dist + dist_between_routes;
    // the gross profit is how much Uber pays for km from each customer + clients that canceled
    double rend_bruto = profitable_dist*1.4 + 7*canceled;
    // despesas is how much we spend going through all the kilometers + car rent
    double despesas = 0.4104*(km_total) + 57;

    printf("\nJornada finalizada. Aqui esta o seu rendimento de hoje\n");
    printf("Km total: %d\n", km_total);
    printf("Rendimento bruto: %.2lf\n",rend_bruto);
    printf("Despesas: %.2lf\n", despesas);
    printf("Rendimento liquido: %.2lf\n",.75*rend_bruto - despesas);

    free(uber->v);
    free(uber);
    return 0;
}
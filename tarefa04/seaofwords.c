#include <stdlib.h>
#include <stdio.h>

/* Creating a char matrix */
char **matrix_creation(int lines, int columns){
    char **m = (char **)calloc(lines,sizeof(char *));
    for (int aux = 0; aux < lines; aux++){
        m[aux] = (char *)calloc(columns,sizeof(char));
    }
    return m;
}
/* Creating an int matrix */
int **matrix_creationd(int lines, int columns){
    int **m = (int **)calloc(lines,sizeof(int *));
    for (int aux = 0; aux < lines; aux++){
        m[aux] = (int *)calloc(columns,sizeof(int));
    }
    return m;
}
/* Counting elements of string */
int count_elements_string(char string[]){
    int k = 0;
    while(string[k] != '\0'){
        k = k+1;
    }
    return(k);
}
/* Next movement realized after fixing a letter, note that here we do not exclude the cases where we come back to a same letter
in the same word */
// Example: A B C D , if we apply {1,0} to this sea-of-words from the letter E, we are going to the letter A
//          E F G H , if we apply {-1,0} from the letter A, we are going to the letter E
//          I J K L , etc

void next_movement(int l, int c, int t, int *nl, int *nc){
    static int movement[4][2] = {{1,0}, {0, 1}, {-1, 0}, {0, -1}}; // Possible movements in the puzzle
    *nl = l + movement[t][0];
    *nc = c + movement[t][1];
}
/* Recursive function responsible for fixing a letter in the sea-of-words and checking
 if from this letter we are able to find the keyword accordingly to the possible movements*/
// hw is the hunting-words (sea-of words matrix)
// aux is a matrix that will help us identifying if a letter was already used or not
// acc is an accumulative variable that helps us to reach the end of the recursive function
// keyword is the word that we are going to search
// n_lines is the number of lines in the sea-of-words
// m_columns is the number of columns in the sea-of-words
// l and c are ,respectively, the line and column that we are going to fix in the sea-of words and search 
int sea_words_rec(char **hw, int **aux, int acc, char *keyword, int n_lines, int m_columns, int l, int c){
    int nl, nc;
    if (acc == count_elements_string(keyword) - 1){
        return 1;
    } 

    if (hw[l][c] == keyword[acc]){
        aux[l][c] = 1;
        for (int k = 0; k < 4; k++){
            next_movement(l, c, k, &nl, &nc);
            if ((nl >= 0) && (nc >= 0) && (nl < n_lines) && (nc < m_columns) && 
            aux[nl][nc] != 1 && hw[nl][nc] == keyword[acc+1]){
                aux[nl][nc] = 1;
                if (sea_words_rec(hw, aux, acc + 1, keyword, n_lines, m_columns, nl, nc)){
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* Setting all matrix elements as zero */
int set_zero(int **matrix, int n_lines, int m_columns){
    for (int k = 0; k < n_lines; k++){
        for (int j = 0; j < m_columns; j++){
            matrix[k][j] = 0;
        }
    }
    return 0;
}
/* Function responsible for looping and applying the recursive function in all the fixed terms from the sea-of-words */
int sea_words(char **hw, int **aux, char *keyword, int n_lines, int m_columns){
    
    for (int v = 0; v < n_lines; v++){
        for (int w = 0; w < m_columns; w++){
            if (hw[v][w] == keyword[0]){     // verify if the first term is the same for the keyword and the sea-of-words 
                 set_zero(aux, n_lines, m_columns);
                 if (sea_words_rec(hw, aux, 0, keyword, n_lines, m_columns, v, w)){
                     return 1;
                 }
           
            }
        }
    }
    return 0;
}

int main(){
    // Reading integers and creating sea-of-words matrix
    int n_lines, m_columns, q_words;
    scanf("%d %d %d", &n_lines, &m_columns, &q_words);
    char **hunting_words = matrix_creation(n_lines, m_columns);
    int **aux = matrix_creationd(n_lines, m_columns);

    // Reading sea-of-words matrix
    for (int l = 0; l < n_lines; l++){
        for (int c = 0; c < m_columns; c++){
            scanf(" %c", &hunting_words[l][c]);
        }
    }

    // Creating a keyword vector that we are going to search and scanning/searching it one by one
    for (int k = 0; k < q_words; k++){
        char *keyword = malloc(21*(sizeof(char)));
        scanf("%s", keyword);
    
    // Applying the function that search the keyword
        int a = sea_words(hunting_words, aux, keyword, n_lines, m_columns);
        if (a == 1){
            printf("sim \n");
        }
        if (a == 0){
            printf("nao \n");
        }
        free(keyword);
    }
    // "Freeing" arrays to avoid memory leakage
    for (int i = 0; i < n_lines; i++){
        char* help = hunting_words[i];
        int *help2 = aux[i];
        free(help);
        free(help2);
    }
    free(hunting_words);
    free(aux);

    return 0;
}
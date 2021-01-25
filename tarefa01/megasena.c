#include <stdio.h>
#include <stdlib.h>
int **Matrix_Creation(int lines,int columns){  // Function used to create a matrix allocating the memory dinamically
    int **matrix = (int **)calloc(lines,sizeof(int *));
    for (int aux=0; aux < lines; aux++){
        matrix[aux] = (int *)calloc(columns,sizeof(int));
    } 
    return(matrix);
}
int main() {
    int m_gambler; // Number of gamblers (5 <= m <= 1000)
    double n_prize; // Prize value (1000000.00 <= n <= 300000000.00)
    scanf("%i %lf", &m_gambler, &n_prize); 
    double sena, quina, quadra; // Prize of each modality according to how many gamblers got the numbers right
    int one, two, three, four, five, six; // Six numbers that won the lottery (from 1 to 60)
    int **bet_matrix = Matrix_Creation(6*m_gambler,10); // Matrix with all the megasena's bet
    int **win_matrix = Matrix_Creation(6,10); // Matrix with the numbers that won the big prize converted into dummies
    int *vector = (int *)calloc(m_gambler,sizeof(int)); // Vector with the numbers of right choices for each gambler
    int acc = 0; // accumulative variable
    for (int i=0; i < (6*m_gambler); i++){
        scanf("%i %i %i %i %i %i %i %i %i %i", &bet_matrix[i][0], &bet_matrix[i][1], // Scanning each bet matrix
        &bet_matrix[i][2], &bet_matrix[i][3], &bet_matrix[i][4], &bet_matrix[i][5],
        &bet_matrix[i][6],&bet_matrix[i][7],&bet_matrix[i][8],&bet_matrix[i][9]);
    }
    scanf("%i %i %i %i %i %i", &one, &two, &three, &four, &five, &six); // Scanning the numbers of the big prize
    for (int l=0; l < 6; l++){  // Maybe this isn't the best approach, but it converts the numbers of the prize in the dummies format 0/1
        for (int k=0; k < 10; k++){
            acc = acc + 1;
            if (one == acc || two == acc || three == acc || four == acc || five == acc || six == acc){ 
                win_matrix[l][k] = 1;
            } 
            else{
                win_matrix[l][k] = 0;
            }
        }
    }
    for (int k=0; k < m_gambler; k++){ // In this loop we compare the prize numbers with the numbers of each gambler
        for (int l=0; l < 6; l++){     
            for (int m=0; m < 10; m++){
                if (bet_matrix[(k*6 + l)][m] == win_matrix[l][m] && win_matrix[l][m]==1){
                    vector[k] = vector[k] + 1; // Accumulative variable that counts how many numbers of each gambler were well chosen
                }                              // vector = [6, 5, 4, ..., 3, 5] (m_gambler elements)
            }                                  //           '  '  ', ... '   ' (corresponding to)
        }                                      //         sen qui qua  nada  quin
    }
    int *final_answer = (int *)calloc(3,sizeof(int)); // Vector that will count how many winners of the sena, quina and quadra we have
    for (int k = 0; k < m_gambler; k++){              // final_answer = [# sena, # quina, # quadra]
        if (vector[k] == 6){                          // These variables are important for us to divide equally the prize afterwards
            final_answer[0] = final_answer[0] + 1;
        }
        if (vector[k] == 5){
            final_answer[1] = final_answer[1] + 1;
        }
        if (vector[k] == 4){
            final_answer[2] = final_answer[2] + 1;
        }
    }
    sena = (n_prize * 0.62) / final_answer[0]; // Assuming that we'll always have at least one winner per type of prize (sena, quina, quadra)
    quina = (n_prize* 0.19) / final_answer[1]; // so we won't divide by zero
    quadra = (n_prize * 0.19) / final_answer[2];
    for (int k=0; k < m_gambler; k++){ // This loop runs on vector because we NEED the information of each gambler 
        if (vector[k] == 6){
            printf("%0.2lf \n", sena);
        }
        if (vector[k] == 5){
            printf("%0.2lf \n", quina);
        }
        if (vector[k] == 4){
            printf("%0.2lf \n", quadra);
        }
        if (vector[k] != 6 && vector[k] != 5 && vector[k] != 4){
            printf("%0.2lf \n", 0.00);
        }
    }
    free(bet_matrix); // Opening space
    free(win_matrix);
    free(vector);
    free(final_answer);
}
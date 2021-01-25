#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct FakeNews {
char *term;
double *percentage;
double maximal;
double minimal;
double mean;
double std;
int category;
} FakeNews;

/* Encontra a porcentagem máxima */
int maximal_pct (FakeNews *FN, int N_term, int M_days){
    for (int k = 0; k < N_term; k++){
        double max = 0;
        for (int j = 0; j < M_days; j++){
            if (max < FN[k].percentage[j]){
                max = FN[k].percentage[j];
            }
        FN[k].maximal = max;
        }
    }
    return 0;
}

/* Encontra a porcentagem mínima */
int minimal_pct (FakeNews *FN, int N_term, int M_days){
    for (int k = 0; k < N_term; k++){
        double min = 100;
        for (int j = 0; j < M_days; j++){
            if (min > FN[k].percentage[j]){
                min = FN[k].percentage[j];
            }
        FN[k].minimal = min;
        }
    }
    return 0;
}
/* Encontra o percentual médio */
int mean_pct (FakeNews *FN, int N_term, int M_days){
    for (int k = 0; k < N_term; k++){
        double mean = 0;
        for (int j = 0; j < M_days; j++){
            mean = mean + FN[k].percentage[j];
        }
        FN[k].mean = mean / M_days;
    }
    return 0;
}

/* Encontra o desvio-padrão */
int std_pct (FakeNews *FN, int N_term, int M_days){
    for (int k = 0; k < N_term; k++){
        double acc = 0;
        for (int j = 0; j < M_days; j++){
            acc = acc + pow(FN[k].percentage[j] - FN[k].mean, 2);
        }
        FN[k].std = sqrt(acc / M_days);
    }
    return 0;
}

/* Verifica categoria */
int verify_category (FakeNews *FN, int term){
    if (FN[term].mean >= 60 && FN[term].std > 15){
        return 0; // Bot
    }
    if (FN[term].mean >= 60 && FN[term].std <= 15){
        return 1; // Evento surpreendente
    }
    if (FN[term].mean < 60 && FN[term].maximal >= 80 && FN[term].minimal > 20){
        return 2; // Evento normal
    }
    if (FN[term].mean < 60 && FN[term].maximal >= 80 && FN[term].minimal <= 20){
        return 3; // Evento local
    }
    if (FN[term].mean < 60 && FN[term].maximal < 80){
        return 4; // Evento irrelevante
    }
    return 5; // Nenhum dos eventos acima
}

/* Conta quantos tem em cada categoria */
int elements_category (FakeNews *FN, int N_term, int categor){
    int counter = 0;
    if (categor == 0){
        for (int k = 0; k < N_term; k++){
            if (FN[k].category == 0){
                counter = counter + 1;
            }
        }
    }
    if (categor == 1){
        for (int k = 0; k < N_term; k++){
            if (FN[k].category == 1){
                counter = counter + 1;
            }
        }
    }
    if (categor == 2){
        for (int k = 0; k < N_term; k++){
            if (FN[k].category == 2){
                counter = counter + 1;
            }
        }
    }
    if (categor == 3){
        for (int k = 0; k < N_term; k++){
            if (FN[k].category == 3){
                counter = counter + 1;
            }
        }
    }
    if (categor == 4){
        for (int k = 0; k < N_term; k++){
            if (FN[k].category == 4){
                counter = counter + 1;
            }
        }
    }
    return counter;
}

/* Printa os elementos que pertencem à mesma categoria (Bot, ...) */
int print_all_same_category(FakeNews *FN, int N_term, int categor){
    for (int k = 0; k < N_term; k++){
        if (FN[k].category == categor){
            printf("%s ", FN[k].term);
        }
    }
    printf("\n");
    return 0;
}

int main() {
// Data acquisition and dynamic allocation of memory
    int N_term, M_days;
    scanf("%d %d", &N_term, &M_days);
    FakeNews *FN = (FakeNews *)calloc(N_term,sizeof(FakeNews));
    for (int k = 0; k < N_term; k++){
         FN[k].term = (char *)calloc(25,sizeof(char));
         scanf("%s", FN[k].term);
         FN[k].percentage = (double *)calloc(M_days,sizeof(double));
        for (int j = 0; j < M_days; j++){
            scanf("%lf", &FN[k].percentage[j]);
        }
    }
// Calculating the features and storing in the struct
    maximal_pct(FN, N_term, M_days);
    minimal_pct(FN, N_term, M_days);
    mean_pct(FN,N_term, M_days);
    std_pct(FN,N_term, M_days);
// Printing the features
    for (int k = 0; k < N_term; k++){
        printf("%s %0.2lf %0.2lf %0.2lf %0.2lf \n", FN[k].term, FN[k].maximal, 
        FN[k].minimal, FN[k].mean, FN[k].std);
    }
    printf("\n");
    printf("RESULTADO: \n");

    for (int k = 0; k < N_term; k++){
        FN[k].category = verify_category(FN, k);
    }
// Printing the categories accordingly
    printf("Bot (%d): ", elements_category(FN, N_term, 0));
    print_all_same_category(FN, N_term, 0);
    printf("Surpreendente (%d): ", elements_category(FN, N_term, 1));
    print_all_same_category(FN, N_term, 1);
    printf("Normal (%d): ", elements_category(FN, N_term, 2));
    print_all_same_category(FN, N_term, 2);
    printf("Local (%d): ", elements_category(FN, N_term, 3));
    print_all_same_category(FN, N_term, 3);
    printf("Irrelevante (%d): ", elements_category(FN, N_term, 4));
    print_all_same_category(FN, N_term, 4);

// Avoiding memory leakage
    for (int k = 0; k < N_term; k++){
        free(FN[k].term);
        free(FN[k].percentage);
    }
    free(FN);

    return 0;
}
#include <stdio.h>
#include "professor_carlos.h"

int main(){
    Turma t[51]; // Defining the set of classes
    int n;       // Number of classes
    int k;       // Number of questions
    int question; // Auxiliar variable used in the if's statements to find the right question
    scanf("%d %d", &n, &k);
    // Scanning and stocking the students in their respective classes
    for (int i = 0; i < n ; i++){
        scanf("%d", &t[i].qtd);
        for (int z = 0; z < t[i].qtd; z++){
            scanf("%s %s %d %d %d", t[i].alunos[z].nome, t[i].alunos[z].sobrenome, &t[i].alunos[z].nascimento.dia, 
            &t[i].alunos[z].nascimento.mes, &t[i].alunos[z].nascimento.ano);
        }
    }
    // Scanning the number of the question and the necessary parameters/printing its response 
    for (int loop = 0; loop < k; loop++){
        scanf("%d", &question);

        if (question == 1){
            int j;
            Aluno younger_in_sameclass;
            scanf("%d",&j);
            younger_in_sameclass = procura_novo_na_turma(t, n, j);
            printf("%s \n", younger_in_sameclass.nome);
        }
        
        if (question == 2){
            int j;
            Aluno older_in_sameclass;
            scanf("%d",&j);
            older_in_sameclass = procura_velho_sona_turma(t,n,j);
            printf("%s \n", older_in_sameclass.sobrenome);
        }

        if (question == 3){
            Aluno older_in_ALLclasses;
            older_in_ALLclasses = procura_velho_todas_turmas(t, n);
            printf("%s \n", older_in_ALLclasses.nome);
        }

        if (question == 4){
            Aluno younger_in_ALLclasses;
            younger_in_ALLclasses = procura_novo_todas_turmas(t, n);
            printf("%s \n", younger_in_ALLclasses.sobrenome);
        }

        if (question == 5){
            char substring[15];
            int number_substring;
            scanf("%s",substring);
            number_substring = conta_substrings(t, n, substring);
            printf("%d \n", number_substring); 
        }

        if (question == 6){
            int j;
            Aluno new_student;
            int nb_students_add;
            scanf("%d", &j);
            scanf("%s %s %d %d %d", new_student.nome, new_student.sobrenome, &new_student.nascimento.dia, 
            &new_student.nascimento.mes, &new_student.nascimento.ano);
            nb_students_add = add_aluno(t, new_student, j);
            printf("%d \n", nb_students_add);
        }

        if (question == 7){
            int j;
            int nb_students_remove;
            scanf("%d", &j);
            nb_students_remove = remove_aluno(t, j);
            printf("%d \n", nb_students_remove);
        }
    }
}
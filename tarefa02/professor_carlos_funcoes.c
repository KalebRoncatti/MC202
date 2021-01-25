#include <stdio.h>
#include "professor_carlos.h"

/* Função responsável por ordernar lexicograficamente os termos. Note que esta função é específica para o nosso
   tipo de struct. A função é baseada em loops nos nomes. Essa função será usado nos códigos que usam lexicografia
   como um critério para ordenação. */
int lexicographic_order(Turma t[], int j, int k, int name){
    for (int index = 0; t[j].alunos[k].nome[index] != '\0'; index++){
        if (t[j].alunos[k].nome[index] - t[j].alunos[name].nome[index] < 0){
            return k;
        }
        if (t[j].alunos[k].nome[index] - t[j].alunos[name].nome[index] > 0){
            return name;
        }
    }
    for (int index1 = 0; t[j].alunos[k].sobrenome[index1] != '\0'; index1++){
        if (t[j].alunos[k].sobrenome[index1] - t[j].alunos[name].sobrenome[index1] < 0){
            return k;
        }
        if (t[j].alunos[k].sobrenome[index1] - t[j].alunos[name].sobrenome[index1] > 0){
            return name;
        }
    }
    return k;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Retorna o mais novo numa turma específica */
Aluno procura_novo_na_turma(Turma t[], int qtd_turmas, int j){
    int aux_year = 0; // Auxiliar variable that will be used to accumulate the birth YEAR of the YOUNGEST person
    int aux_month;  // Auxiliar variable that will be used to accumulate the birth MONTH of the YOUNGEST person
    int aux_day;    // Auxiliar variable that will be used to accumulate the birth DAY of the YOUNGEST person
    int name;       // Auxiliar variable that will be used to carry the INDEX of the YOUNGEST person
    /*  The idea of the code to find the youngest person in a given class (j) is to loop in the class j and compare: 
       The year of birth. If the lowest year of birth is the same we compare the month. If the month is the same we
       compare the day and finally if the birth day is equal we go to the lexicographic order classification.
        Note that we used the else(if())) statement's because we just desire the loop "entering" in the else when it
       does not enter in the if statement  */
    for(int k = 0; k < t[j].qtd; k++){
        if ((t[j].alunos[k].nascimento.ano - aux_year ) > 0){
            aux_year = t[j].alunos[k].nascimento.ano;
            aux_month = t[j].alunos[k].nascimento.mes;
            aux_day = t[j].alunos[k].nascimento.dia;
            name = k;
        }
        else{if ((aux_year - t[j].alunos[k].nascimento.ano) == 0){
                if ((t[j].alunos[k].nascimento.mes - aux_month > 0 )){
                    name = k;
                    aux_month = t[j].alunos[k].nascimento.mes;
                }
                else{if ((t[j].alunos[k].nascimento.mes - aux_month == 0)){
                        if ((t[j].alunos[k].nascimento.dia - aux_day > 0 )){
                         name = k;
                         aux_day = t[j].alunos[k].nascimento.mes;
                        }
                        else{if ((t[j].alunos[k].nascimento.dia - aux_day == 0)){
                             int lexicogr_lower = lexicographic_order(t, j, k, name);
                             name = lexicogr_lower;
                            }
                        }
                    }
                }
            }
        }
    }
    return t[j].alunos[name];
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Reutiliza a função anterior para procurar o mais novo dentre todas as turmas */
Aluno procura_novo_todas_turmas(Turma t[], int qtd_turmas){
    Turma youngest_per_class[1]; // Set of classes created to gather the YOUNGER of each class
    Aluno youngest;
    int dummie = 0;
    /* Every turn in the loop will find the YOUNGEST of each class and add to the SET OF CLASSES */
    for (int j = 0; j < qtd_turmas; j++){
        youngest_per_class[0].alunos[j] = procura_novo_na_turma(t, qtd_turmas, j); // 
    }
    youngest_per_class[0].qtd = qtd_turmas; // Necessary because we use the attribute .qtd in the last function
    youngest = procura_novo_na_turma(youngest_per_class, dummie, 0);
    return youngest;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Retorna o mais VELHO numa turma específica */
Aluno procura_velho_sona_turma(Turma t[], int qtd_turmas, int j){
    int aux_year = 30000; // Auxiliar variable that will be used to accumulate the birth YEAR of the OLDEST person
    int aux_month;  // Auxiliar variable that will be used to accumulate the birth MONTH of the OLDEST person
    int aux_day;    // Auxiliar variable that will be used to accumulate the birth DAY of the OLDEST person
    int name;       // Auxiliar variable that will be used to carry the INDEX of the OLDEST person
    /* The idea of the code to find the OLDEST person in a given class (j) is to loop in the class j and compare: 
       The year of birth. If the biggest year of birth is the same we compare the month. If the month is the same we
       compare the day and finally if the birth day is equal we go to the lexicographic order classification */
    for(int k = 0; k < t[j].qtd; k++){
        if ((t[j].alunos[k].nascimento.ano - aux_year ) < 0){
            aux_year = t[j].alunos[k].nascimento.ano;
            aux_month = t[j].alunos[k].nascimento.mes;
            aux_day = t[j].alunos[k].nascimento.dia;
            name = k;
        }
        else{if ((aux_year - t[j].alunos[k].nascimento.ano) == 0){
                if ((t[j].alunos[k].nascimento.mes - aux_month < 0 )){
                    name = k;
                    aux_month = t[j].alunos[k].nascimento.mes;
                }
                else{if ((t[j].alunos[k].nascimento.mes - aux_month == 0)){
                        if ((t[j].alunos[k].nascimento.dia - aux_day < 0 )){
                         name = k;
                         aux_day = t[j].alunos[k].nascimento.mes;
                        }
                        else{if ((t[j].alunos[k].nascimento.dia - aux_day == 0)){
                             int lexicogr_lower = lexicographic_order(t, j, k, name);
                             name = lexicogr_lower;
                            }
                        }
                    }
                }       
            }
        }
    }
    return t[j].alunos[name];
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Reutiliza a função anterior para procurar o mais VELHO dentre TODAS as turmas */
Aluno procura_velho_todas_turmas(Turma t[], int qtd_turmas){
    Turma oldest_per_class[1];
    Aluno oldest;
    int dummie = 0;
    for (int j = 0; j < qtd_turmas; j++){
        oldest_per_class[0].alunos[j] = procura_velho_sona_turma(t, qtd_turmas, j);
    }
    oldest_per_class[0].qtd = qtd_turmas; 
    oldest = procura_velho_sona_turma(oldest_per_class, dummie, 0);
    return oldest;
}

/* Conta os elementos de uma string qualquer */
int count_elements_string(char string[]){
    int k = 0;
    while(string[k] != '\0'){
        k = k+1;
    }
    return(k);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Checa se existe uma substring num único nome */
int substring(Turma t[], int aluno, int j, char *padrao){
    Aluno specific_aluno = t[j].alunos[aluno];
    int max_travel = count_elements_string(specific_aluno.nome) - count_elements_string(padrao); // We can go through the loop until max_travel since it takes in account the size of the substring
    int subs = 0; // Counter of substrings
    /* This loop will go through the whole string until max_travel so we do not "cross" to the "trash"
       elements stocked in the string after the '\0' element "*/
    for (int k = 0; k <= max_travel; k++){
        int acc = 0;
        /* This loop will go through the substring and verify if it matches with a portion of the string */
        for (int l = 0; l < count_elements_string(padrao); l++){
            if (specific_aluno.nome[k+l] == padrao[l]){
                acc = acc + 1;
            }
        if (acc == count_elements_string(padrao)){
            subs += 1;
            }
        }
    }
    if (subs > 0){
        return 1;
    }
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Conta quantos alunos possuem uma substring específica em TODAS as turmas */
int conta_substrings(Turma t[], int qtd_turmas, char *padrao){
    int happen = 0; // Counts each occurrence of a substring in a name
    for (int j = 0; j < qtd_turmas; j++){
        for (int aluno = 0; aluno < t[j].qtd; aluno++){
            if (substring(t, aluno, j, padrao) == 1){
                happen = happen + 1;
            }
        }
    }
    return happen;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Inserindo um novo aluno na turma j */
int add_aluno(Turma t[], Aluno A, int j){
    t[j].alunos[t[j].qtd] = A;
    t[j].qtd = t[j].qtd + 1;
    return (t[j].qtd);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Removendo o último aluno da turma j, note que o array não muda de tamanho pois não estamos trabalhando com alocação dinâmica */
int remove_aluno(Turma t[], int j){
    Aluno deleted_student;
    deleted_student.nascimento.ano = 0;
    deleted_student.nascimento.mes = 0;
    deleted_student.nascimento.dia = 0;
    for (int k = 0; deleted_student.nome[k] != '\0'; k++){
        deleted_student.nome[k] = '0';
    }
    for (int k =0; deleted_student.sobrenome[k] != '\0'; k++){
        deleted_student.sobrenome[k] = '0';
    }
    t[j].alunos[(t[j].qtd - 1)] = deleted_student;
    t[j].qtd = t[j].qtd - 1;
    return(t[j].qtd);
}
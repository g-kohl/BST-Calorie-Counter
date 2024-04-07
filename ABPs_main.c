#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include "ABPs_lib.h"

int main(int argc, char* argv[]){
    FILE *input_table, *input_consumed, *output;//tabela nutricional, tabela de alimentos consumidos, saida
    char line[1000], delims[]={";\n\r"}, *foodName;
    FOOD newFood;
    nodeAVL *treeAVL=createAVL(), *consumedFood_AVL;//arvore AVL vazia, nodo AVL
    nodeBST *treeBST=createBST(), *treeSplay=createBST(), *consumedFood_BST;//ABP vazia, arvore Splay vazia, nodo ABP
    int grams=0, calories=0, totalCalories=0, totalNodes=0, comp_SPLAY_insert=0, comp_SPLAY_consult=0;//gramas, calorias, nodos, comparacoes ao inserir e consultar na Splay

    extern int comp_AVL, comp_BST, comp_SPLAY;//variaveis globais importadas
    extern int rotations_AVL, rotations_SPLAY;

    if(argc != 4){
        printf("Incorrect number of parameters. \nIt's expected two input files and one output file");
        return 1;
    }

    input_table = fopen(argv[1], "r");

    if(input_table == NULL){
        printf("Error opening the file %s", argv[1]);
        return 1;
    }

    while(fgets(line, 1000, input_table)){//percorre cada linha da tabela, separando o nome do alimento de suas respectivas calorias e insere o novo nodo nas arvores
        foodName = strlwr(strtok(line, delims));
        strcpy(newFood.name, foodName);

        newFood.calories = atoi(strtok(NULL, delims));

        treeAVL = insertAVL(treeAVL, newFood);
        treeBST = insertBST(treeBST, newFood);
        treeSplay = insertSplay(treeSplay, newFood);
        totalNodes ++;
    }

    printf("File %s opened succesfully\n", argv[1]);

    comp_SPLAY_insert = comp_SPLAY;

    input_consumed = fopen(argv[2], "r");

    if(input_consumed == NULL){
        printf("Error opening the file %s", argv[2]);
        return 1;
    }

    output = fopen(argv[3], "w");
    fprintf(output, "Calories calculated for %s using %s:\n", argv[2], argv[1]);

    while(fgets(line, 1000, input_consumed)){//percorre cada linha da tabela, separando o nome do alimento de suas respectivas gramas
        foodName = strlwr(strtok(line, delims));

        if(foodName == NULL || foodName[0] == '\r' || strlen(line) < 3)
            break;

        consumedFood_AVL = consultAVL(treeAVL, foodName);//busca o alimento nas arvores
        consumedFood_BST = consultBST(treeBST, foodName);
        treeSplay = splaying(treeSplay, foodName);

        if(consumedFood_AVL){//calcula quantas calorias foram consumidas baseando-se no numero de gramas
            grams = atoi(strtok(NULL, delims));
            calories = (consumedFood_AVL->food.calories * grams)/100;//regra de 3
            totalCalories += calories;

            fprintf(output, "\n%dg of %s (%d calories for 100g) = %d calories",
                    grams, consumedFood_AVL->food.name, consumedFood_AVL->food.calories, calories);//printa os dados daquele alimento
        }
    }

    comp_SPLAY_consult = comp_SPLAY - comp_SPLAY_insert;//separa o numero de comparacoes ao inserir e consultar na arvore Splay

    printf("\nFile %s opened succesfully\n", argv[2]);
    printf("\nFile %s opened succesfully\n", argv[3]);

    fprintf(output, "\n\nTotal calories of the day: %d", totalCalories);//printa as estatisticas de cada arvore
    fprintf(output, "\n\nAVL STATISTICS:");
    fprintf(output, "\nNumber of nodes: %d", totalNodes);
    fprintf(output, "\nHeight: %d", AVL_nodeHeight(treeAVL));
    fprintf(output, "\nNumber of rotations: %d", rotations_AVL);
    fprintf(output, "\nNumber of comparisons: %d", comp_AVL);

    fprintf(output, "\n\nBST STATISTICS:");
    fprintf(output, "\nNumber of nodes: %d", totalNodes);
    fprintf(output, "\nHeight: %d", heightBST(treeBST));
    fprintf(output, "\nNumber of comparisons: %d", comp_BST);

    fprintf(output, "\n\nSPLAY STATISTICS:");
    fprintf(output, "\nNumber of nodes: %d", totalNodes);
    fprintf(output, "\nHeight: %d", heightBST(treeSplay));
    fprintf(output, "\nNumber of rotations: %d", rotations_SPLAY);
    fprintf(output, "\nNumber of comparisons when inserting: %d", comp_SPLAY_insert);
    fprintf(output, "\nNumber of comparisons when consulting: %d", comp_SPLAY_consult);
    fprintf(output, "\nNumber of total comparisons: %d", comp_SPLAY);

    fclose(input_table);
    fclose(input_consumed);
    fclose(output);

    return 0;
}



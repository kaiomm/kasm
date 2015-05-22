/*
Este programa le valores numericos de um arquivo em formato de texto e escreve um novo arquivo
em binario colocando os numeros em formato 16bits

objetivo:   transformar um arquivo objeto do trabalho 1 para binario para ser usado com meu simulador
            de assembly inventado
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    FILE *fpIN,*fpOUT;
    char *file_in=(char*)malloc(256*sizeof(char));
    char *file_out=(char*)malloc(256*sizeof(char));
    char filename[256];
    short cur;

    if(argc<2){
        printf("\nParametros invalidos, digite o nome do arquivo de entrada: ");
        scanf("%s",file_in); getchar();
        printf("\nDigite o nome do arquivo de saida: ");
        scanf("%s",file_out); getchar();
    }
    else{
        file_in=argv[1];
        file_out=argv[2];
    }
    if((fpIN=fopen(file_in,"r"))==NULL){
        printf("\n\tERRO! Arquivo nao encontrado");
        return 100;
    }
    if((fpOUT=fopen(file_out,"w"))==NULL){
        printf("\n\tERRO! Nao foi possivel criar arquivo");
        return 101;
    }
    do{
        fscanf(fpIN, "%d",&cur);
        fprintf(fpOUT, "%c", cur>>8);
        fprintf(fpOUT, "%c", (cur<<8)>>8);
        printf("\n\t\t%5d %c 0x%04X",cur,16,cur);
    }while(!feof(fpIN));
    fclose (fpIN);
    fclose (fpOUT);
    return 0;
}

/*
Isto eh um programa que simula um processador hipotetico inventado pelos professores de
ciencia da computacao da Universidade de Brasilia

Esse computador hipotetico possui um registrador contador de programa(PC), um
acumulador (ACC) e 216 espacos de memoria, todos estes descritos, de 16 bits
(eu sei que isso nao eh nem um pouco otimizado, mas ei, nao fui eu que inventei
 esse computador, ele eh feito pra ser didatico, e nao funcional)

o processador hipotetico aceita o seguinte conjunto de instrucoes

ADD 01 2 ACC = ACC + mem(OP)
SUB 02 2 ACC = ACC - mem(OP)
MUL 03 2 ACC = ACC × mem(OP)
DIV 04 2 ACC = ACC ÷ mem(OP)
JMP 05 2 PC = OP
JMPN 06 2 Se ACC<0 então PC = OP
JMPP 07 2 Se ACC>0 então PC = OP
JMPZ 08 2 Se ACC=0 então PC = OP
COPY 09 3 mem(OP2) = mem(OP1)
LOAD 10 2 ACC = mem(OP)
STORE 11 2 mem(OP) = ACC
INPUT 12 2 mem(OP) = entrada
OUTPUT 13 2 saída = mem(OP)
STOP 14 1 Suspende a execução

este programa possui chamadas de console exclusivas do Windows, se quiser compilar em
outro sistema operacional, faca as mudancas necessarias.

testado no windows 7 e 8, mas deve funcionar em qualquer um.

versao 1.1

Kaio Marcondes  kaiomm@gmail.com

bibliografia: http://aprender.ead.unb.br/pluginfile.php/52452/mod_resource/content/0/aula4.pdf
obs: este link pode pedir uma senha de acesso

changelog:
1.1 - mudado alguns parametros visuais para exibicao do acumulador
1.0 - primeira versao funcional sem nenhum bug aparente
*/

/* ****TO DO****
(nao precisa ser feito, montador e ligador que se virem pra resolver isso)
verificar se o programa cabe na memoria
verificar se o programa acessa enderecos de memoria >216

*/

#include <stdio.h>
#define COLS 6
#define LINES 36

short *memory;
short ACC=0;
short PC=0;

char *current;
short opcode;

enum opcode{ERROR,ADD,SUB,MUL,DIV,JMP,JMPN,JMPP,JMPZ,COPY,LOAD,STORE,INPUT,OUTPUT,STOP};

void error(){
    printf("\n\tERROR: illegal memory access or undefined instruction!\n\n");
    getchar();
 }
short inputVirtual(){                   //input requisitado pelo programa
    short input;
    showInterface();
    printf("\n\n\tUser Input %c  ",16);
    scanf("%d",&input);
    getchar();  //gambiarra
    return input;
}
void outputVirtual(short output){       //output do programa
    showInterface();
    printf("\n\n\tProgram Output %c  %d\n",16,output);
    getchar();
}
void stopProgram(){                     //termina o programa com status 1, significando termino de sucesso
    showInterface();
    printf("\n\n\tProgram Finished!\n");
    getchar();
    exit(1);
}
void showInterface(){                   //mostrar memoria e registradores, esses numeros loucos sao os equivalentes
    int i,j,k;                          //ASCII dos simbolos que eu usei para desenhar as tabelas de memoria
    system("cls");

    for(i=0;i<COLS;i++) printf(" %c%c%c%c%c%c%c%c%c%c%c%c ",201,205,205,205,203,205,205,205,205,205,205,187); printf("\n");
    for(i=0;i<COLS;i++) printf(" %cAdd%cMemory%c ",186,186,186);                                              printf("\n");
    for(i=0;i<COLS;i++) printf(" %c%c%c%c%c%c%c%c%c%c%c%c ",204,205,205,205,206,205,205,205,205,205,205,185); printf("\n");
    for(i=0;i<LINES;i++){
        for(j=0;j<COLS;j++){
            k=j*LINES+i;
//           if(k!=0&&k!=180&&(k%10==0))
//            printf(" %c%c%c%c%c%c%c%c%c%c%c%c ",204,205,205,205,206,205,205,205,205,205,205,185);
            if(k==PC)
                printf("%c%c%03d%c% 6d%c%c",16,186,k,186,memory[k],186,17); //deixar espaco para os sinais e nao quebrar a tabela
            else
                printf(" %c%03d%c% 6d%c ",186,k,186,memory[k],186);
        }
        printf("\n");
    }
    for(i=0;i<6;i++) printf(" %c%c%c%c%c%c%c%c%c%c%c%c ",200,205,205,205,202,205,205,205,205,205,205,188); printf("\n");
    printf(" PC: %3d \t\tACC: %5d \t\tCURRENT: %s ",PC,ACC,current);
    if(opcode==COPY)      printf("%5d %5d\n",memory[PC+1],memory[PC+2]);
    else if(opcode==STOP) printf("\n");
    else                  printf("%5d\n",memory[PC+1]);
}
short readInstruction(short address){   //funcao de ler a instrucao apontada pelo PC
    switch(memory[address]){
        case ADD:    current="   ADD"; break;
        case SUB:    current="   SUB"; break;
        case MUL:    current="   MUL"; break;
        case DIV:    current="   DIV"; break;
        case JMP:    current="   JMP"; break;
        case JMPN:   current="  JMPN"; break;
        case JMPP:   current="  JMPP"; break;
        case JMPZ:   current="  JMPZ"; break;
        case COPY:   current="  COPY"; break;
        case LOAD:   current="  LOAD"; break;
        case STORE:  current=" STORE"; break;
        case INPUT:  current=" INPUT"; break;
        case OUTPUT: current="OUTPUT"; break;
        case STOP:   current="  STOP"; break;
        default:     current=" ERROR"; error(); return ERROR; break;
    }
    return memory[address];
}
void doInstruction(short address){   //funcao de executar a instrucao apontada pelo PC
    switch(memory[address]){
        case ADD:    ACC+=memory[memory[address+1]];                      PC+=2; break;
        case SUB:    ACC-=memory[memory[address+1]];                      PC+=2; break;
        case MUL:    ACC*=memory[memory[address+1]];                      PC+=2; break;
        case DIV:    ACC/=memory[memory[address+1]];                      PC+=2; break;
        case JMP:                                          PC=memory[address+1]; break;
        case JMPN:   if(ACC<0)  PC=memory[address+1];                else PC+=2; break;
        case JMPP:   if(ACC>0)  PC=memory[address+1];                else PC+=2; break;
        case JMPZ:   if(ACC==0) PC=memory[address+1];                else PC+=2; break;
        case COPY:   memory[memory[address+2]]=memory[memory[address+1]]; PC+=3; break;
        case LOAD:   ACC=memory[memory[address+1]];                       PC+=2; break;
        case STORE:  memory[memory[address+1]]=ACC;                       PC+=2; break;
        case INPUT:  memory[memory[address+1]]=inputVirtual();            PC+=2; break;
        case OUTPUT: outputVirtual(memory[memory[address+1]]);            PC+=2; break;
        case STOP:   stopProgram();                                       PC+=1; break;
        default:  error();                                                       break;
    }
}
int main(int argc, char *argv[]){
    FILE *fp;
    char *filename;
    char c;
    int i=0;

    memory=(short*)calloc(216, sizeof(short));
    filename=(char*)malloc(256*sizeof(char));
    current=(char*)malloc(7*sizeof(char));

    system("mode 85, 49"); // redimensiona o console para caber a memoria toda na janela

    if(argc<2){
        printf("\n\tEnter object code file: ");
        scanf("%s", filename);
    }
    else
        filename=argv[1];
    fp=fopen(filename,"r");
    if(fp==NULL){
        printf("\n\n\tFile invalid!");
        return 666;
    }
    while((c=getc(fp))!=EOF){
        memory[i/2]=memory[i/2]|(c<<(8*((i+1)%2)));
        i+=1;
    }
 getchar(); //gambiarra
    while(PC<216){
        if((opcode=readInstruction(PC))==ERROR){
            showInterface();
            return 245;
        }
        showInterface();
        getchar();
        doInstruction(PC);
    }
return 0;
}
/*
Lista de Erros propositais:

Erro 666:   nao pode abrir arquivo

Erro 245:   programa leu memoria nao reservada por ele
            provavelmente nao tem STOP no programa
*/

#include <stdio.h>
#include <stdlib.h>

void filesc(int n, int m, char s[2], int x, int y){//funcao para criar pesos aleatorios no file determminado
    //vamos criar pesos para redes de n ate m,s[0] - numero de nivel, s[1] - determine se eh (b) - bias ou (p) - peso, x e y - tamanhos de vetor
    float arr[x][y];
    FILE *file[100];//vetor para trabalhar com 100 files
    for(int k=n;k<m;k++){
        char filename[5];
        sprintf(filename, "%c%c%d.txt", s[0], s[1], k);//criamos nome de file
        file[k] = fopen(filename, "w");//abrimos file
        for(int i=0;i<x;i++){
            for(int j=0;j<y;j++){
                arr[i][j]=((((double)rand()) / RAND_MAX)*2)-1;//criamos numero aleatorio entre -1 e 1
                fprintf(file[k], "%f ", arr[i][j]);//escrevemos o numero no file
                //printf("%f ",arr[i][j]);
            }
            fprintf(file[k], "\n");//trocamos a linha
            //printf("\n");
        }
    fclose(file[k]);//fechamos o file atual
    }
    return;
}

void filcopy(char s[2], int n, int m){//funcao para copiar pesos se s[1] = (p) ou bias se s[1] = (b) de file n para file n
    //s[1] - numero de nivel
    FILE *fptr1, *fptr2;//variaveis para files
    char filename[100], c;
    sprintf(filename, "%c%c%d.txt", s[0], s[1], m);//criamos nomes de files
    fptr1 = fopen(filename, "r");//e abrimos deles
    sprintf(filename, "%c%c%d.txt", s[0], s[1], n);
    fptr2 = fopen(filename, "w");
    c = fgetc(fptr1);//copiamos o caracter de file 1 (fptr1) com numero de rede = m
    while (c!= EOF){
        fputc(c, fptr2);//para o faile 2 (fptr2) com numero de rede = n
        c = fgetc(fptr1);
    }
    fclose(fptr1);//fechamos files
    fclose(fptr2);
    return ;
}

void copy_pesos(int n, int m){//funcao inicial para copiar pesos de rede (m) para  rede (n)
    filcopy("1p", n, m);//copiamos cada nivel
    filcopy("2p", n, m);//p - peso, b - bias
    filcopy("3p", n, m);
    filcopy("4p", n, m);
    filcopy("1b", n, m);
    filcopy("2b", n, m);
    filcopy("3b", n, m);
    filcopy("4b", n, m);
    return;
}

void filmut(char s[2], int n, int m, int tom1, int tom2, int r){//funcao de mutacao (parte de pesos se tornam aleatorias)
    //mutamos os pesos de rede de numero (m) e escrevemos o resultado para rede numero (n), tom1, tom2 - tamanhos de vetor
    //s[0] - numero de nivel, s[1] - tipo de file
    //r - define que cara r'simo peso esta trocado
    FILE *fptr1, *fptr2;
    char filename[100], c[8];
    sprintf(filename, "%c%c%d.txt", s[0], s[1], m);//abrimos files
    fptr1 = fopen(filename, "r");
    sprintf(filename, "%c%c%d.txt", s[0], s[1], n);
    fptr2 = fopen(filename, "w");
    for(int i=0;i<tom1;i++){
        for(int j=0;j<tom2;j++){
            if((i+j)/r==0){//se eh r'simo numero - trocamos para aleatorio
                fprintf(fptr2, "%f ", ((((double)rand()) / RAND_MAX))*2)-1;
            }else{
                fscanf(fptr1, "%s", c);//se nao - so copiamos o valor
                fprintf(fptr2, "%s ", c);
            }
        }
        fprintf(fptr2, "\n");
    }
    fclose(fptr1);//fechamos files
    fclose(fptr2);
    return ;
}

void mutate(int n, int m){//funcao inicial para mutacao de pesos de rede (m) e salvar pesos no rede (n)
    float ran;
    int r;
    ran =(((double)rand())/RAND_MAX)*9;
    r = (int)round(ran)+1;//r'simo peso vai ser trocado para aleatorio
    //printf("%d - > %d : r = %d\n", n, m, r);
    filmut("1p", n, m, 64, 48, r);//chamamos funcao para cada nivel e cada tipo (bias e peso)
    filmut("2p", n, m, 48, 32, r);
    filmut("3p", n, m, 32, 16, r);
    filmut("4p", n, m, 16, 1, r);
    filmut("1b", n, m, 48, 1, r);
    filmut("2b", n, m, 32, 1, r);
    filmut("3b", n, m, 16, 1, r);
    filmut("4b", n, m, 1, 1, r);
    return ;
}

void filcross(char s[2], int n, int m, int k, int tom1, int tom2, int r){//funcao de "crossingover"
    //bota dois files (m) e (k) e merge deles no file (n)
    //s[0]-numero de nivel, s[1] - tipo de file (p -peso, b -bias)
    //tom1, tom2 - tamanho de vetor, r - cada r numeros trocamos o file (dentre m e k) de onde copiamos o peso
    FILE *fptr1, *fptr2, *fptr3;//criamos o file juntando partes de outros dois
    char filename[10], c1[8];
    int troc = 1;//variavel de tpo {1} ou {-1} define de qual file copiamos o peso
    sprintf(filename, "%c%c%d.txt", s[0], s[1], m);//abrimos files
    fptr1 = fopen(filename, "r");
    sprintf(filename, "%c%c%d.txt", s[0], s[1], k);
    fptr2 = fopen(filename, "r");
    sprintf(filename, "%c%c%d.txt", s[0], s[1], n);
    fptr3 = fopen(filename, "w");
    for(int i=0;i<tom1;i++){
        if(i%r==0){//cada r'simo numero mudamos o file de onde copiar
            troc = troc * (-1);
        }
        for(int j=0;j<tom2;j++){//scaneamos e escrevemos os pesos
            fscanf(fptr1, "%s", &c1);
            if(troc==1){
                fprintf(fptr3, "%s ", c1);
            }
            fscanf(fptr2, "%s", &c1);
            if(troc==-1){
                fprintf(fptr3, "%s ", c1);
            }
        }
        fprintf(fptr3, "\n");
    }
    fclose(fptr1);//fechamos files
    fclose(fptr2);
    fclose(fptr3);
    return ;
}

void cross(int bom[50][2], int i){//funcao inicial para "crossingover"
    //toma a lista dos numeros da rede e indece de rede que presisa sera mudada (i)
    float ran;
    int r;
    ran =(((double)rand())/RAND_MAX)*13;
    r = (int)round(ran)+1;//variavel para escolher files aleatorios
    int n = bom[i][0];//file atual
    int m = bom[r][0];//rede aleatoria com boa nota (primeira metade da lista)
    int k = bom[50-r][0];//rede aleatoria com nota mal (segunda metade de lista)
    //printf("Cross: %d(%d) + %d(%d) -> %d(%d)\n", m, r, k, 99-r, n, i);
    filcross("1p", n, m, k, 64, 48, 24);//chamamos funcao para cada nivel e cada tipo de file
    filcross("2p", n, m, k, 48, 32, 24);
    filcross("3p", n, m, k, 32, 16, 24);
    filcross("3p", n, m, k, 16, 1, 24);
    filcross("1b", n, m, k, 48, 1, 12);
    filcross("2b", n, m, k, 32, 1, 12);
    filcross("3b", n, m, k, 16, 1, 12);
    return ;
}

void swap(int* xp, int* yp){//funcao adicaional para sortear, troca entre se os valores de (xp) e (ep)
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
    return ;
}

void bubble_Sort(int arr[50][2]){//funcao para sortear os redes neurais de accordo com pontos recebidos
    //no vetor (bom) a columna [0]-numero de rede, columna [1] - nota dela
    int i, j;
    for(i=0;i<50-1; i++){
        for(j=0;j<50-i-1; j++){
            if(arr[j][1]<arr[j+1][1]){
                swap(&arr[j][1], &arr[j + 1][1]);
                swap(&arr[j][0], &arr[j + 1][0]);
            }
        }
    }
    return ;
}

void evolv(int bom[50][2]){//funcao geral de evolucao
    //no vetor (bom) a columna [0]-numero de rede, columna [1] - nota dela
    printf("~Evoluindo~\n");
    int n=6;//coeficiente de seguranca (quantas melhores sao salvadas)
    for(int i=0;i<50; i++){//primeiros n sao salvadas
        if(i>=n && i<n*2 && bom[i-n]>0){//segundas n sao copias de primeiras n
            copy_pesos(bom[i][0],bom[i-n][0]);
            //printf("Copi: %d -> %d\n", bom[i-n][0], bom[i][0]);
        }else if(i>=n*2 && i<n*4){//de 2n ate 4n sao copias de 0 ate 2n e mutadas
            copy_pesos(bom[i][0],bom[i-12][0]);
            mutate(bom[i][0], bom[i-2*n][0]);
            //printf("Mut: %d -> %d\n", bom[i-2*n][0], bom[i][0]);
            //printf("~Mutei~\n");
        }else if(i>=n*4 && i<n*8){//de 4n ate 8n sao combinacoes aleatorias de outras
            cross(bom, i);
            //printf("~Crossei~\n");
        }else if(i>=n*8 && i<50){//o resto sera zerado e trocado para pesos aleatorios
            rand_pesos(bom[i][0], bom[i][0]+1);
        }
    }
    return ;
}

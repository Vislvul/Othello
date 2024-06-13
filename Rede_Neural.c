#include <stdio.h>
#include <stdlib.h>
#include <math.h>
float f(float n){//funcao de ativacao de rede neural
    //return ((1 / (1 + exp(-n))));
    //if(n>0)return n;
    //if(n<=0)return 0;
    //return (exp(n)+exp(-n)/(exp(n)-exp(-n)));
    return (((2)/(1+exp(-n)))-1);//sigmid baichada de 1
}

void rand_pesos(int n, int m){ //funcao de criacao de rede,
    //shama funcao que cria pesos aleatorias para redes com numeros entre (n) e (m), funcao (filesc) esta no [Evolucao]
    filesc(n, m, "1p", 64, 48);//para cada nivel o quantidade de elementos eh differente
    filesc(n, m, "2p", 48, 32);//p - pesos   b - bias
    filesc(n, m, "3p", 32, 16);//primeio numero - numero de nivel
    filesc(n, m, "4p", 16, 1);
    filesc(n, m, "1b", 48, 1);
    filesc(n, m, "2b", 32, 1);
    filesc(n, m, "3b", 16, 1);
    filesc(n, m, "4b", 1, 1);
    return;
}

int inipesos (){ //funcao para criar a rede "de zero"
    int m;
    printf("(ATENCAO!) se os pesos ja existem eles vao ser (ELIMINADOS), Criar pesos? [1] - SIM \n");
    scanf("%d",&m);
    if(m==1){
        printf("\nQuantas listas? ");
        scanf("%d",&m);
        rand_pesos(0,m);//cria m redes
    }
    printf("Pesos aleatorios sao criados\n");
    return m;
}

float filler(int layer, char s[2], int n, int x, int y, float **arr){//funcao para ler pesos de file para vetor (arr)
    //layer - numero de nivel nececario, s- para ver se eh (p)-peso ou (b)-bias, n - numero de rede, x e y - tamanho de vetor no file
    FILE *file;
    char filename[8];
    sprintf(filename, "%d%c%d.txt", layer, s[0], n);//criamos nome de file
    //printf("%d%c%d\n",l,s[0],n);
    //printf("%c%c%c%c%c%c%c%c\n", filename[0], filename[1], filename[2], filename[3], filename[4], filename[5], filename[6], filename[7]);
    file = fopen(filename, "r");//abrimos file
    for(int i=0;i<x;i++){
        for(int j=0;j<y;j++){
            fscanf(file, "%f", &arr[i][j]);//lemos os valores
            //if(y==2){
                //printf("%d%d: %f\n",i,j, arr[i][j]);
            //}
        }
    }
    fclose(file);//fechamos o file
    return 0;
}

void neuron(float layer_antrior[64], int x, float layer_atual[64], int y, int layer, int n){//funcao de troca entre niveis de rede
    //toma dois niveis anterior de tamanho (x) e atual de tamanho (y), layer - numero de nivel atual, n - numero de rede
    int b=1;//multiplicador para bias (define a forca de efeito de rede(quando maior o efeito eh menor))
    float** peso=(float **)malloc(sizeof(float*)*x);//alocamos vetores para peso
    for (int i=0; i<x; i++){
        peso[i]=(float*)malloc(sizeof(float)*y);
    }
    float** bias=(float **)malloc(sizeof(float*)*y);// e bias
    for (int i=0; i<y; i++){
        bias[i]=(float*)malloc(sizeof(float)*1);
    }
    filler(layer, "pr", n, x, y, peso);//lemos peso e bias de file
    filler(layer, "br", n, y, 1, bias);
    for(int i=0;i<y;i++){//multiplicamos vetor anterior por pesos e bias
        layer_atual[i]=0;
        //printf("%.3f\n",layer_anterior[i]);
        for(int j=0;j<x;j++){
            layer_atual[i]=layer_atual[i]+layer_antrior[j]*peso[j][i];
            //printf("%.3f=%.3f+%.3f*%.3f\n", layer_atual[i],layer_atual[i],layer_anterior[j],peso[j][i]);
        }
        //printf("%.3f\n",layer_anterior[i]);
        layer_atual[i]=layer_atual[i]+b*bias[i][0];
        //printf("%.3f ",layer_2[i]/18);
        layer_antrior[i]=f(layer_atual[i]/sqrt(y));//usamos funcao de ativacao para eliminar linearidade e salvamos o resultado no nivel anterior
        //if(l==4)printf("Leyer: %d (%d): %.3f\n", l, i+1, layer_antrior[i]);
        //printf("%.3f\n",layer_anterior[i]);
    }
    free(peso);//liberamos a memoria de vetores alocadas
    free(bias);
    return ;
}

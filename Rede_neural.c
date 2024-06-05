#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int inicializar(int arr[8][8]){//funcao para inicializar o quadro primeiro
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            arr[i][j]=0;
        }
    }
    arr[3][3]=1;
    arr[4][4]=1;
    arr[3][4]=-1;
    arr[4][3]=-1;
    return 0;
}

void iniquadro(int arr[8][8]){//criar 50 quadros para jogos
    FILE *file;
    char filename[7];
    for(int k=0;k<100;k++){
        sprintf(filename, "Q%d.txt", k);
        file = fopen(filename, "w");
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                fprintf(file, "%d ", arr[i][j]);
                //if(k==0)printf("%d ",arr[i][j]);
            }
            //if(k==0)printf("\n");
        }
        fclose(file);
    }
    return;
}

void lerquadro(int arr[8][8], int n){
    FILE *file;
    char filename[7];
    sprintf(filename, "Q%d.txt", n);
    file = fopen(filename, "r");
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            fscanf(file, "%d", &arr[i][j]);
            //printf("%d ",arr[i][j]);
        }
        //printf("\n");
    }
    fclose(file);
    return;
}

void escquadro(int arr[8][8], int n){
    FILE *file;
    char filename[7];
    sprintf(filename, "Q%d.txt", n);
    file = fopen(filename, "w");
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            fprintf(file, "%d ", arr[i][j]);
        }
    }
    fclose(file);
    return;
}

void desenhar(int arr[8][8]){
    printf("     ");
    for(int i=0;i<8;i++){
        printf("(%d) ",i+1);
    }
    printf("\n");
    for(int i=0;i<8;i++){
        printf("    ---------------------------------\n");
        printf("(%d) | ",i+1);
        for(int j=0;j<8;j++){
            if(arr[i][j]==1)printf("X");
            if(arr[i][j]==-1)printf("O");
            if(arr[i][j]==0)printf("#");
            if(j<7)printf(" | ");
        }
        printf(" |\n");
    }
    return;

}

int line(int player, int x1, int y1, int x2, int y2,int arr[8][8]){
    //printf("Line: %d%d : %d%d\n",x1+1,y1+1,x2+1,y2+1);
    if(x2>x1 && y2<y1){
        for(int i = x1;i<=x2;i++){
            for(int j = y2;j<=y1;j++){
                if(i-x1==y1-j){
                    arr[i][j]=player;
                }
            }
        }
    }
    for(int i = x1;i<=x2;i++){
        for(int j = y1;j<=y2;j++){
            if(x2>x1 && y2>y1 && i-x1==j-y1){
                arr[i][j]=player;
            }else if(x2==x1 || y2==y1){
                arr[i][j]=player;
            }
        }
    }
    return 0;
}

int movimento(int player, int x, int y, int arr[8][8], int jogar){//funcao para ver se o movimeto eh possivel
    int d=0,e=0,s=0,b=0,ds=0,db=0,es=0,eb=0,n=0;//variaveis para cada dorecao no quadro (cima, baiho, esquerda, direita, e diagonais dentre deles
    if(x>1){ //vemos se a borda eh perta para nao passar dentro dela e tomamos valores de pontos no cada direcao
        s=arr[x-1][y];
        if(y<6){
            ds=arr[x-1][y+1];
        }
        if(y>1){
            es=arr[x-1][y-1];
        }
    }
    if(x<6){//vemos se a borda eh perta para nao passar dentro dela
        b=arr[x+1][y];
        if(y<6){
            db=arr[x+1][y+1];
        }
        if(y>1){
            eb=arr[x+1][y-1];
        }
    }
    if(y>1){//vemos se a borda eh perta para nao passar dentro dela
        e=arr[x][y-1];
    }
    if(y<6){//vemos se a borda eh perta para nao passar dentro dela
        d=arr[x][y+1];
    }
    if(arr[x][y]==0){//se o lugar de movimeno eh vazil continuamos
        for(int i=2;i<8;i++){ //vemos para cada dos direcoes
            if(d*(-1)*player==i-1){ //se o ponto atual eh ocupado com outro jogador - adicionamos para cunta nesse derecao
                if(arr[x][y+i]==player){ // se segou no ponto de jogador, desenhaamos a linha fechando a direcao
                    if(jogar ==1)line(player,x,y,x,y+d*(-1)*player,arr);
                    n++;
                }
                d=d+arr[x][y+i];
            }
            if(e*(-1)*player==i-1){ //repetimos para cada direcao
                if(arr[x][y-i]==player){
                    if(jogar ==1)line(player,x,y-e*(-1)*player,x,y,arr);
                    n++;
                }
                e=e+arr[x][y-i];
            }
            if(s*(-1)*player==i-1){
                if(arr[x-i][y]==player){
                    if(jogar ==1)line(player,x-s*(-1)*player,y,x,y,arr);
                    n++;
                }
                s=s+arr[x-i][y];
            }
            if(b*(-1)*player==i-1){
                if(arr[x+i][y]==player){
                    if(jogar ==1)line(player,x,y,x+b*(-1)*player,y,arr);
                    n++;
                }
                b=b+arr[x+i][y];
            }
            if(ds*(-1)*player==i-1){
                if(arr[x-i][y+i]==player){
                    if(jogar ==1)line(player,x-ds*(-1)*player,y+ds*(-1)*player,x,y,arr);
                    n++;
                }
                ds=ds+arr[x-i][y+i];
            }
            if(db*(-1)*player==i-1){
                if(arr[x+i][y+i]==player){
                    if(jogar ==1)line(player,x,y,x+db*(-1)*player,y+db*(-1)*player,arr);
                    n++;
                }
                db=db+arr[x+i][y+i];
            }
            if(es*(-1)*player==i-1){
                if(arr[x-i][y-i]==player){
                    if(jogar ==1)line(player,x-es*(-1)*player,y-es*(-1)*player,x,y,arr);
                    n++;
                }
                es=es+arr[x-i][y-i];
            }
            if(eb*(-1)*player==i-1){
                if(arr[x+i][y-i]==player){
                    if(jogar ==1)line(player,x,y,x+eb*(-1)*player,y-eb*(-1)*player,arr);
                    n++;
                }
                eb=eb+arr[x+i][y-i];
            }
        }
    }
    return n; //voltamos a longitude das linhas desenhadas, se nao desenhou nada - voltamos zero significa que jogo eh impossivel
}

int conta(int arr[8][8], int player){
    int p1,p2;
    p1=0;
    p2=0;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(arr[i][j]==1){
                p1++;
            }
            if(arr[i][j]==-1){
                p2++;
            }
        }
    }
    //if(p1+p2-4>7)printf("Player X: (%d);  Player O: (%d) ~%d~\n",p1,p2,p1+p2-4);
    if(player==1){
        return p1;
    }else{
        return p2;
    }
}

float f(float n){//
    //return ((1 / (1 + exp(-n))));
    //if(n>0)return n;
    //if(n<=0)return 0;
    //return (exp(n)+exp(-n)/(exp(n)-exp(-n)));
    return (((2)/(1+exp(-n)))-1);
}

float gauss(float n){
    return (1/(sqrt(2*3.14)))*exp((-n)*n/2);
}

void filesc(int n, int m, char s[2], int x, int y){
    float arr[x][y];
    FILE *file[100];
    for(int k=n;k<m;k++){
        char filename[5];
        sprintf(filename, "%c%c%d.txt", s[0], s[1], k);
        file[k] = fopen(filename, "w");
        for(int i=0;i<x;i++){
            for(int j=0;j<y;j++){
                arr[i][j]=(((double)rand()) / RAND_MAX);
                fprintf(file[k], "%f ", arr[i][j]);
                //printf("%f ",arr[i][j]);
            }
            fprintf(file[k], "\n");
            //printf("\n");
        }
    fclose(file[k]);
    }
    return;
}

void rand_pesos(int n, int m){
    filesc(n, m, "1p", 64, 48);
    filesc(n, m, "2p", 48, 24);
    filesc(n, m, "3p", 24, 2);
    filesc(n, m, "1b", 48, 1);
    filesc(n, m, "2b", 24, 1);
    filesc(n, m, "3b", 2, 1);
    return;
}

int inipesos (){
    int m;
    printf("Criar pesos? 1 - sim: ");
    scanf("%d",&m);
    if(m==1){
        printf("\nQuantas listas? ");
        scanf("%d",&m);
        rand_pesos(0,m);
    }
    return m;
}

float filler(char s[2], int n, int x, int y, int tom1, int tom2, float arr[tom1][tom2]){
    FILE *file;
    char filename[8];
    sprintf(filename, "%c%c%d.txt", s[0], s[1], n);
    file = fopen(filename, "r");
    for(int i=0;i<x;i++){
        for(int j=0;j<y;j++){
            fscanf(file, "%f", &arr[i][j]);
            //if(y==2){
                //printf("%d%d: %f\n",i,j, arr[i][j]);
            //}
        }
    }
    fclose(file);
    return 0;
}

void filcopy(char s[2], int n, int m){
    FILE *fptr1, *fptr2;
    char filename[100], c;
    sprintf(filename, "%c%c%d.txt", s[0], s[1], m);
    fptr1 = fopen(filename, "r");
    sprintf(filename, "%c%c%d.txt", s[0], s[1], n);
    fptr2 = fopen(filename, "w");
    c = fgetc(fptr1);
    while (c!= EOF){
        fputc(c, fptr2);
        c = fgetc(fptr1);
    }
    fclose(fptr1);
    fclose(fptr2);
    return ;
}

void copy_pesos(int n, int m){
    filcopy("1p", n, m);
    filcopy("2p", n, m);
    filcopy("3p", n, m);
    filcopy("1b", n, m);
    filcopy("2b", n, m);
    filcopy("3b", n, m);
    return;
}

void filmut(char s[2], int n, int m, int tom1, int tom2, int r){
    FILE *fptr1, *fptr2;
    char filename[100], c[8];
    sprintf(filename, "%c%c%d.txt", s[0], s[1], m);
    fptr1 = fopen(filename, "r");
    sprintf(filename, "%c%c%d.txt", s[0], s[1], n);
    fptr2 = fopen(filename, "w");
    for(int i=0;i<tom1;i++){
        for(int j=0;j<tom2;j++){
            if((i+j)/r==0){
                fprintf(fptr2, "%f ", (((double)rand()) / RAND_MAX));
            }else{
                fscanf(fptr1, "%s", c);
                fprintf(fptr2, "%s ", c);
            }
        }
        fprintf(fptr2, "\n");
    }
    fclose(fptr1);
    fclose(fptr2);
    return ;
}

void mutate(int n, int m){
    float ran;
    int r;
    ran =(((double)rand())/RAND_MAX)*9;
    r = (int)round(ran)+1;
    //printf("%d - > %d : r = %d\n", n, m, r);
    filmut("1p", n, m, 64, 48, r);
    filmut("2p", n, m, 48, 24, r);
    filmut("3p", n, m, 24, 2, r);
    filmut("1b", n, m, 48, 1, r);
    filmut("2b", n, m, 24, 1, r);
    filmut("3b", n, m, 2, 1, r);
    return ;
}

void filcross(char s[2], int n, int m, int k, int tom1, int tom2, int r){
    FILE *fptr1, *fptr2, *fptr3;
    char filename[10], c1[8];
    int troc = 1;
    sprintf(filename, "%c%c%d.txt", s[0], s[1], m);
    fptr1 = fopen(filename, "r");
    sprintf(filename, "%c%c%d.txt", s[0], s[1], k);
    fptr2 = fopen(filename, "r");
    sprintf(filename, "%c%c%d.txt", s[0], s[1], n);
    fptr3 = fopen(filename, "w");
    for(int i=0;i<tom1;i++){
        if(i%r==0){
            troc = troc * (-1);
        }
        for(int j=0;j<tom2;j++){
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
    fclose(fptr1);
    fclose(fptr2);
    fclose(fptr3);
    return ;
}

void cross(int bom[100][2], int i){
    float ran;
    int r;
    ran =(((double)rand())/RAND_MAX)*23;
    r = (int)round(ran)+1;
    int n = bom[i][0];
    int m = bom[r][0];
    int k = bom[99-r][0];
    //printf("Cross: %d(%d) + %d(%d) -> %d(%d)\n", m, r, k, 99-r, n, i);
    filcross("1p", n, m, k, 64, 48, 24);
    filcross("2p", n, m, k, 48, 24, 12);
    filcross("3p", n, m, k, 24, 2, 6);
    filcross("1b", n, m, k, 48, 1, 12);
    filcross("2b", n, m, k, 24, 1, 6);
    filcross("3b", n, m, k, 2, 1, 1);
    return ;
}

void nn(int arr[8][8],int hod[2],int numero){
    float layer_1[64];
    int b1=1;
    float layer_2[48];
    int b2=1;
    float layer_3[24];
    int b3=1;
    float peso[64][48];
    float bias[48][1];
    float saida[2];
    int n=0;
    //--------------Ler nivel 1-------------------
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(arr[i][j]==-1)layer_1[n] = 0.5;
            layer_1[n] = arr[i][j];
            //printf("%d: %d = %d \n", n, layer_1[n], arr[i][j]);
            n++;
        }

    }
    //--------------Criar nivel 2-------------------
    filler("1p", numero, 64, 48, 64, 48, peso);
    filler("1b", numero, 48, 1, 48, 1, bias);
    /*for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            printf("%f ",peso[i][j]);
        }
        printf("\n");
    }*/
    for(int i=0;i<48;i++){
        layer_2[i]=0;
        for(int j=0;j<64;j++){
            layer_2[i]=layer_2[i]+layer_1[j]*peso[j][i];
        }
        layer_2[i]=layer_2[i]+b1*bias[i][0];
        //printf("%.3f ",layer_2[i]/18);
        layer_2[i]=f(layer_2[i]/8);
        //printf("%.3f\n",layer_2[i]);
    }
    //--------------Criar nivel 3-------------------
    filler("2p", numero, 48, 24, 64, 48, peso);
    filler("2b", numero, 24, 1, 48, 1, bias);
    /*for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            printf("%f ",peso[i][j]);
        }
        printf("\n");
    }*/
    //printf("\n");
    for(int i=0;i<24;i++){
        layer_3[i]=0;
        for(int j=0;j<48;j++){
            layer_3[i]=layer_3[i]+layer_2[j]*peso[j][i];
        }
        layer_3[i]=layer_3[i]+b2*bias[i][0];
        //printf("%.3f ",layer_3[i]/9);
        layer_3[i]=f(layer_3[i]/7);
        //printf("%.3f\n",layer_3[i]);
    }

    //-------------Criar nivel saida---------------
    filler("3p", numero, 24, 2, 64, 48, peso);
    filler("3b", numero, 2, 1, 48, 1, bias);
    /*for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            printf("%f ",peso[i][j]);
        }
        printf("\n");
    }*/
    for(int i=0;i<2;i++){
        saida[i]=0;
        for(int j=0;j<32;j++){
            saida[i]=saida[i]+layer_3[j]*peso[j][i];
        }
        saida[i]=saida[i]+b3*bias[i][0];
        //printf("%d = %.3f ",i,saida[i]);
        hod[i]=(int)round((f(saida[i]/5))*7);
        //printf("%.3f %d\n",saida[i],hod[i]);
    }
    return ;
}

void copy(int arr[8][8], int arr2[8][8]){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
                arr2[i][j]=arr[i][j];
            }
        }
    return ;
}

void algoritmo(int arr[8][8], int player){
    int i,j;
    i=(int)round(rand() % (8 + 1 - 0));
    j=(int)round(rand() % (8 + 1 - 0));
    if(conta(arr,player*(-1))!=0){
        while(movimento(player,i,j,arr,1)==0){
            i=(int)round(rand() % (8 + 1 - 0));
            j=(int)round(rand() % (8 + 1 - 0));
        }
    }
    //printf("Movimento: %d %d\n", i+1, j+1);
    //conta(arr,player);
    return;
}

void algoritmo_2(int arr[8][8], int player){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(movimento(player, i ,j ,arr,1)!=0){
                return;
            }
        }
    }
    return ;
}

void algoritmo_3(int arr[8][8], int player){
    int r=0, move[20][3], ma=0;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(movimento(player,i,j,arr,0)!=0){
                move[r][0]=i;
                move[r][1]=j;
                move[r][2]=movimento(player,i,j,arr,0);
                move[r][2]=move[r][2]+16-abs(j-4)-abs(i-4);
                r++;
            }
        }
    }
    for(int i=1;i<r;i++){
        if(move[r][2]>move[ma][2]){
            ma=r;
        }
    }
    movimento(player,move[ma][0],move[ma][1],arr,1);
    return ;
}

void tentativa(int arr[8][8], int n, int player, int bom[n][2], int turn){
    int hod[2],i;
    nn(arr,hod,n);
    //if(turn>1)printf("Hod:(%d) %d %d\n",n,hod[0]+1,hod[1]+1);
    if(hod[0]>7 || hod[0]<0 || hod[1]>7 || hod[1]<0){
        printf("~Problema~  ");
        printf("Hod:(%d) %d %d\n",n,hod[0]+1,hod[1]+1);
        rand_pesos(bom[n][0], bom[n][0]+1);
        hod[0]=0;
        hod[1]=0;
    }
    if(arr[hod[0]][hod[1]]==0){
        if(movimento(player,hod[0],hod[1],arr,1)==0){
            //printf("The movement is unavalble, choose other point\n");
            //bom[n][1]=bom[n][1]-1;
            //tentativa(arr, n, player, bom);
            i++;
        }else{
            //printf("Great move!\n");
            //system("cls");
            //desenhar(arr);
            bom[n][1]=bom[n][1]+conta(arr, player);
            if(turn>1){
                printf("(%d) Hod: %d %d Conta = %d ~%d~\n",n,hod[0]+1,hod[1]+1,bom[n][1],turn);
                //desenhar(arr);
            }
            //printf("%d: %d %d\n",i, move[i][0], move[i][1]);
        }
    //}else{
        //printf("The movement is unavalble, choose other point\n");
        //bom[n][1]=bom[n][1]-1;
        //tentativa(arr, n, player, bom);
    }
    //printf("%d: %d %d Got:%d\n",i, move[i][0], move[i][1], bom[i][1]);
    return ;
}

void swap(int* xp, int* yp){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
    return ;
}

void bubble_Sort(int arr[100][2]){
    int i, j;
    for(i=0;i<100-1; i++){
        for(j=0;j<100-i-1; j++){
            if(arr[j][1]<arr[j+1][1]){
                swap(&arr[j][1], &arr[j + 1][1]);
                swap(&arr[j][0], &arr[j + 1][0]);
            }
        }
    }
    return ;
}

void evolv(int bom[100][2]){
    printf("~Evoluindo~\n");
    int n=10;
    for(int i=0;i<100; i++){
        if(i>=n && i<n*2 && bom[i-n]>0){
            copy_pesos(bom[i][0],bom[i-n][0]);
            //printf("Copi: %d -> %d\n", bom[i-n][0], bom[i][0]);
        }else if(i>=n*2 && i<n*4){
            //copy_pesos(bom[i][0],bom[i-12][0]);
            mutate(bom[i][0], bom[i-2*n][0]);
            //printf("Mut: %d -> %d\n", bom[i-2*n][0], bom[i][0]);
            //printf("~Mutei~\n");
        }else if(i>=n*4 && i<n*8){
            cross(bom, i);
            //printf("~Crossei~\n");
        }else if(i>=n*8 && i<100){
            rand_pesos(bom[i][0], bom[i][0]+1);
        }
    }
    return ;
}

void vez()
{
    int arr[8][8];
    inicializar(arr);
    iniquadro(arr);
    //desenhar(arr);
    int turn=1;
    int x,y,player=1;
    int quant_nn=100, bom[quant_nn][2];
    for(int i=0;i<quant_nn;i++){
       bom[i][1] = 0;
       bom[i][0] = i;
    }
    //inipesos();
    while(turn<10){
        if(player==0){
            printf("Player: X\n");
            printf("Escreve sua movimento:");
            scanf("%d%d",&x,&y);
            if(arr[x-1][y-1]==0){
                if(movimento(player,x-1,y-1,arr,1)==0){
                    printf("The movement is unavalble, choose other point\n");
                }else{
                    printf("Great move!\n");
                    //system("cls");
                    //desenhar(arr);
                    //conta(arr, player);
                    player=player*(-1);
                    turn=turn+1;
                }
            }else{
                printf("The movement is unavalble, choose other point\n");
            }
        }else{
            for(int i=1;i<100;i=i+1){
                //printf("Player: X\n");
                lerquadro(arr,i);
                //desenhar(arr);
                //tentativa(arr, 50+i, player*(-1), bom, turn);
                algoritmo(arr, 1);
                //algoritmo_2(arr, 1);
                //desenhar(arr);
                //printf("Player: O\n");
                tentativa(arr, i, -1, bom, turn);
                escquadro(arr,i);
            }
            /*if(turn >= 7){
                for(int i=0;i<10; i++){
                    if(bom[i][1]>0)printf("Turn: %d, (%d) = %d\n",turn,bom[i][0],bom[i][1]);
                }
            }*/
            //player=player*(-1);
            turn=turn+1;
            //printf("-----------------%d---------------\n",turn);
        }
    }
    bubble_Sort(bom);
    printf("~Sortei~\n");
    /*for(int i=0;i<100; i++){
        if(bom[i][1]>3)printf("%d = %d\n",bom[i][0],bom[i][1]);
    }*/
    evolv(bom);
    printf("~FINAL~");
    return ;
}

int main()
{
    srand(time(0));
    for(int i=0;i<1500;i++){
        printf("%d)\n",i);
        vez();
    }
    return 0;
}

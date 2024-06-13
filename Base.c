//Raphael Mendes Batista
//Daniil Sveshnikov
//Katarina Torres Bento

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void regras(){ //a funcao para escrever regras
    int x;
    printf("Bem vindo ao jogo Othello,\n\t"
        "Aqui voce tera que travar um duelo tatico tentando conseguir o maximo de calulas possivel.\t\n"
        "O primeiro jogador joga por (X), o segundo por (O), \t\n"
        "os espacos vazios sao designados (_), os espacos disponiveis para uma jogada sao designados (*).\t\n"
        "Para fazer um movimento, escreva dois valores separados por um espaco:\t\n"
        "PRIMEIRA - numero da LINHA, SEGUNDA - numero da COLUNA.\t\n"
        "Se um determinado movimento for impossivel, voce sera solicitado a repeti-lo.\t"
        "Usar valores FRACIONAIS quebrara o jogo :(\t\n"
        "Um movimento eh possivel se voce criar uma linha (ou varias) em qualquer direcao (horizontal, vertical ou diagonal)\t\n"
        "para que as fichas do inimigo fiquem localizadas ao longo da linha entre suas pecas.\t\n"
        "Neste caso, as fichas do adversario entre as suas se transformarao nas suas \t\n"
        "e voce tambem colocara a sua ficha no local de onde jogou.\t\n"
        "Se um jogador nao tiver espaco disponivel para fazer uma jogada, a vez passa para o outro jogador.\t\n"
        "Se nenhum dos jogadores conseguir mais fazer nenhum movimento, o jogo termina. \t\n"
        "O jogador que tiver mais fichas na mesa neste momento vence.\t\n"
        "Aproveite o jogo!\t\n"
        "Para continuar digite qualquer numero inteiro:\n");
        scanf("%d",&x);
        //system("clear");
        return ;
}

int inicializar(int arr[8][8]){ //funcao para inicaializar o vetor
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            arr[i][j]=0;  //escrevemos os zeros no vetor
        }
    }
    arr[3][3]=-1;  //e fazemos posicao inicail
    arr[4][4]=-1;
    arr[3][4]=1;
    arr[4][3]=1;
    return 0;
}

void desenhar(int arr[8][8]){  //funcao para dezenhar o quadro
    printf("     ");
    for(int i=0;i<8;i++){  //a numeracao de colunas
        printf("(%d) ",i+1);
    }
    printf("\n");
    for(int i=0;i<8;i++){
        printf("    ---------------------------------\n");  //divisao de limhas
        printf("(%d) | ",i+1);  //divisao de colunas
        for(int j=0;j<8;j++){
            if(arr[i][j]==1)printf("X");  // X - para primeiro jogador
            if(arr[i][j]==-1)printf("O"); // O - para segundo jogador
            if(arr[i][j]==0)printf("_");  // _ - para vazil
            if(arr[i][j]==2)printf("*");  // * - para lugar de posicao available para jogar
            if(j<7)printf(" | "); //divasao de colunas
        }
        printf(" |\n");
    }
    return;

}

int line(int player, int x1, int y1, int x2, int y2,int arr[8][8]){ //funcao para dezenhar a linha no quadro
    //de ponto 1 (x1,y1) ate ponto 2 (x2,y2), player - qual jogador desenha a linha
    if(y1>y2){//se eh possivel a linha tem que ser de cima para bicho
        swap(&x1, &x2);
        swap(&y1, &y2);
    }
    if(x1>x2){//linha eh sempre de cima para baiho
        swap(&x1, &x2);
        swap(&y1, &y2);
    }
    //printf("Line: %d%d : %d%d\n",x1+1,y1+1,x2+1,y2+1);
    if(x2>x1 && y2<y1){ //se eh paralelo ao diagonal secendaria usamos esta parte
        for(int i = x1;i<=x2;i++){
            for(int j = y2;j<=y1;j++){
                if(i-x1==y1-j){ //desenhamos o parte de linha (trocamos o ponto) se a distansia no x e igual a -dictancia de y
                    arr[i][j]=player;                                       //(ou seja pertence a diagonal secundaria)
                }
            }
        }
    }
    for(int i = x1;i<=x2;i++){
        for(int j = y1;j<=y2;j++){
            if(x2>x1 && y2>y1 && i-x1==j-y1){ //caso simolar para diagonal principal
                arr[i][j]=player;
            }else if(x2==x1 || y2==y1){ //caso de retas verticais ou horizontais (sao mais simples) - so passar de comeco ao final
                arr[i][j]=player;
            }
        }
    }
    return 0;
}

bool in(x){//funcao adicional para a funcao (in table), esta testando se numero esta dentro 0 e 7
    bool b = (x >= 0) && (x < 8);
    return b;
}

bool inTable(x, y){//funcao para ver se o ponto (x,y) esta no quadro
    return in(x) && in(y);
}

int movimento(int player, int x, int y, int arr[8][8], int jogar){//funcao para ver se o movimeto (x,y) eh possivel
    //e, se jogar = 1 desenhar esta linha
    int direcao[8], n=0, d=-1;//vetor para cada direcao no quadro (cima, baiho, esquerda, direita, e diagonais dentre deles)
    //n - variavel para ver se o movimento eh possivel, d - iteracao de direcao
    if(arr[x][y]!=0)return 0;//se o lugar eh ocupado retornamos 0
    if(!inTable(x,y))return 0;//se o ponto esta fora de tabuleiro retornamos 0
    //if (jogar==2)printf("Ponto: %d %d\n", x+1, y+1);
    for (int x_shift = -1; x_shift <= 1; x_shift++){//testamos cada direcao
        for (int y_shift = -1; y_shift <= 1; y_shift++){
            d++;//trocamos para proximo direcao
            direcao[d]=0;//zeramos o valor de
            //if (jogar==2)printf("direcao %d:\n", d);
            if (inTable(x+x_shift, y+y_shift) && arr[x+x_shift][y+y_shift]==player*(-1)){//se nesse direcao eh o enemigo - continuamos
                //if (jogar==1)printf("Testamos\n");
                for(int k=1; k<8; k++){//avancamos nesse diresao
                    //if (jogar==2)printf("K = %d:\n", k);
                    if (!inTable(x+x_shift*k, y+y_shift*k)) {//se vemos a borda a linha eh impossivel
                        //if (jogar==2)printf("x: %d y: %d is outside the table\n", x+x_shift*k+1, y+y_shift*k+1);
                        break;
                    }else if(arr[x+x_shift*k][y+y_shift*k]==0){ //se encomtramos o vazil a linha eh impossivel
                        //if (jogar==2)printf("x: %d y: %d is null. Attempting %d %d\n", x+x_shift*k+1, y+y_shift*k+1, x+1, y+1);
                        break;
                    }else if(arr[x+x_shift*k][y+y_shift*k]==player && direcao[d]==k-1){//se encontramos a peca de jogador e jogar = 1 desenhamos a linha
                        //if (jogar==2)printf("Allay (%d %d)\n", x+x_shift*k+1, y+y_shift*k+1);
                        //if (jogar==2)printf("Linha (%d %d) -> (%d %d)\n", x+1, y+1, x+x_shift*(k-1)+1, y+y_shift*(k-1)+1);
                        if(jogar==1)line(player, x,y, x+x_shift*(k-1), y+y_shift*(k-1), arr);
                        n++;//salvamos que esiste a linha possivel
                        //if (jogar==2)printf("Desenhou linha, n = %d\n", n);
                        break;
                    }else if(arr[x+x_shift*k][y+y_shift*k]==-player){//se encontramos o enemigo avancamos mais
                        direcao[d]++;
                        //if (jogar==2)printf("Enemigo (%d %d)\n", x+x_shift*k+1, y+y_shift*k+1);
                    }
                }
            }
            //if (jogar==2)printf("direcao %d == %d  n = %d\n", d, direcao[d], n);
        }
    }
    //if (jogar==2)printf("Ponto (%d %d), n = %d\n", x+1, y+1, n);
    return n;//retornamos o quantidade das linhas possiveis
}

void copy(int arr[8][8], int arr2[8][8]){ //funcao para copiar os valores de vetor para outro
                                                    //para desenhas os lugares possiveis para jogar
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
                arr2[i][j]=arr[i][j];
            }
        }
    return ;
}

int available(int arr[8][8], int player, int show){ //funcao para ver lugares possiveis para jogar e mostrar o quadro com jogadas possiveis se show = 1
    int r=0, arr2[8][8]; //variaveis para numero de movimentos possiveis e o vetor adicaional
    copy(arr,arr2); //coppiamos para vetor temporal
    for(int i=0;i<8;i++){ //testamos cada lugar de quadro
        for(int j=0;j<8;j++){
            if(movimento(player,i,j,arr,0)!=0){ //vemos se o movimeno eh possivel
                if(show==1){//se queremos mostrar o quadro marcamos movimentos possiveis
                    arr2[i][j]=2;
                }
                r++;//aumenamos o numero de movimentos possiveis
            }
        }
    }
    if(show==1){//se queremos mostrar desenhamos o vetor temporal
        desenhar(arr2);
    }
    return r;//voltamos o numero de movimentos possiveis
}

int conta(int arr[8][8], int player, int show){//funcao para calcular e mostrar a conta (se show = 1) de jogador (player)
    int p1=0, p2=0; //variaveis para conta de cada jogador
    for(int i=0;i<8;i++){//vemos se o ponto pertence para um dos jogadores e aumentamos a conta dele
        for(int j=0;j<8;j++){
            if(arr[i][j]==1){
                p1++;
            }
            if(arr[i][j]==-1){
                p2++;
            }
        }
    }
    if(show==1){ //se eh nececario mostramos a conta
        printf("Jogador X: (%d)     Jogador O: (%d)\n", p1, p2);
    }
    if(player==1){//retornamos a conta de jogador
        return p1;

    }else if(player==-1){
        return p2;
    }else if(player==0){//ou soma das contas dos jogadores (para ver se o quadro tem mais lugares)
        return p1+p2;
    }
}

void vencedor(int arr[8][8]){//funcao para escrever o vencedor
    if(conta(arr, 1, 0)>conta(arr, -1, 0)){//escrevemos quem tem mais pontos
        printf("O jogador X venceu, Parabens!\n");
    }else if(conta(arr, 1, 0)<conta(arr, -1, 0)){
        printf("O jogador O venceu, Parabens!\n");
    } else if(conta(arr, 1, 0)==conta(arr, -1, 0)){ //ou que ninguem vevnceu
        printf("Ninguem venceu, boa joga)\n");
    }
}


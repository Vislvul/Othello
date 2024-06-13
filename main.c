#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float nn(int arr[8][8], int n, float res){//funcao central de rede neural
    //recebo o tabuleiro (arr), (n) - numero de rede, e (res) - valor para dela voltar
    float r[1][1];//vetor para o resultado
    float layer_atual[64];//vetores de nivel atual e anerior
    float layer_antrior[64];
    float temp;//variavel temporal
    int k=0;// variavel para trocar a mitriz para o vetor
    //--------------Ler nivel 1-------------------
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            //layer_antrior[k]=0;
            //printf("%d: %.3f = %d \n", k, layer_antrior[k], arr[i][j]);
            layer_antrior[k] = (float)arr[i][j];//trocamos a mtriz para o vetor
            //printf("%d: %.3f = %d \n", k, layer_antrior[k], arr[i][j]);
            k++;
        }
    }
    //--------passar por rede neural 3 veses------------
    for(int i=1; i<4; i++){
        //3 veses usamos neron para trocar entre nivel anterior do tamnho (64-(16*i)) para nivel atual de tamnho (48-(16*i))
        //i - numero de nivel atual, n - numero de rede
        neuron(layer_antrior, 64-(16*i), layer_atual, 48-(16*i), i, n);
    }
    neuron(layer_antrior, 16, r, 1, 4, n);//ultima troca para o nivel (r) - resultado
    res = r[0][0];
    //printf("Res  = %.4f \n", res);
    return res;//retornamos o valor - resultado de avalicao de cituacao na tabela pelo rede neural
}

struct move{//struct para salvar o movimento a nota desse movimento pelo funcao miniMAX
    int x;
    int y;
    float pont;
};

int analize(int arr[8][8], int player, struct move m[20]){ //funcao para ver lugares possiveis para jogar pelo jogado (player)
    //e salvar movimentos possiveis no vetor (m)
    int r=0; //variaveis para numero de movimentos possiveis e o vetor adicaional
    for(int i=0;i<8;i++){ //testamos cada lugar de tabuleiro
        for(int j=0;j<8;j++){
            if(movimento(player,i,j,arr,0)!=0){ //vemos se o movimeno eh possivel
                //printf("Test: (%d %d) %d\n", i, j, movimento(player,i,j,arr,0));
                m[r].x=i;
                m[r].y=j;
                r++;//aumenamos o numero de movimentos possiveis
            }
        }
    }
    return r;//voltamos o numero de movimentos possiveis
}

struct move mini(struct move m1, struct move m2){//funcao que escolhe qual dos dois movimentos (m1) ou (m1) temos a menor pontuacao
    if(m1.pont<m2.pont){
        return m1;//e retornar o movimento com o menor pontuacao
    }else{
        return m2;
    }
}

struct move maxi(struct move m1, struct move m2){//funcao que escolhe qual dos dois movimentos (m1) ou (m1) temos a maior pontuacao
    if(m1.pont>m2.pont){
        return m1;//e retornar o movimento com o maior pontuacao
    }else{
        return m2;
    }
}

float avaliar(int arr[8][8], int player, float res){//funcao para avaliar a cituacao no tabuleiro de accordo com pesos fixos de pontos
    res=0;//res - resultado de avalicao player - jogador atual
    int pesos[8][8] =//tabela de pesos de lugares de tabuleiro
    {{100,-30,10,5,5,10,-30,100},
    {-30,-50,-2,-2,-2,-2,-50,-30},
    {10,-2,-1,-1,-1,-2,-2,10},
    {5,-2,-1,0,0,-1,-2,5},
    {5,-2,-1,0,0,-1,-2,5},
    {10,-2,-1,-1,-1,-2,-2,10},
    {-30,-50,-2,-2,-2,-2,-50,-30},
    {100,-30,10,5,5,10,-30,100}};
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            res=res+arr[i][j]*pesos[i][j];// o resultado eh maior quando jogodor 1 ocupa lugares melhores
            //e menor quando o jogador -1 ocupa lugares melhores
        }
    }
    return res*0.1;//retornamos o valor dividido por 10 (para ser comparavel com saida de rede neural)
}

struct move miniMAX(int arr[8][8], int player, int iteracao, int turn, int n, float alpha, float beta){//funcao principal de anlize de movimentos
    //recebe o tabuleiro, o jogador atual, iteracao(qual jogada na ferente esta analizada agora), turn - quantes veses foram feitas
    //alpha e beta - variaves para comparaacao recurciva entre niveis de analize de jogadas
    int depth=5;//determinamos quantas jogadas a frente queremos analizar
    if(turn>(61-depth)){//se somos perto da final do jogo diminuima a profundidade de analize
        depth--;
    }
    if(iteracao==depth || available(arr,player, 0)==0){//se somos no ultima nivel de movimentos que queremos anlizarr  ou se o jojo acaba com esse movimento
            //retornamos a avalicao de situacao no tabuleiro
        struct move m;//variavel para retornar o valor de avalicao de situacao atual (struct, como funcao retorna simente struct)
        if(turn<48){//se nao eh final do jogo avaliamos com rede neural e tabela de pesos
            m.pont=nn(arr, n, m.pont);
            m.pont=m.pont+avaliar(arr, player, m.pont);
        }else if(turn>=48){//se eh finaal queremos vencer(ganhar mais pontos)
            m.pont=conta(arr, player, 0);
        }
        //printf("Pontos: %.4f\n",m.pont);
        //m.pont = (float)conta(arr, player, 0);
        return m;
    }else{//no outro caso definimos movimentos possiveis e fazemos deles
        int r, arr2[8][8];// r - numero de movimentos possiveis, arr2 - vetor para fazer estes jogadas
        struct move m[20];//vetor para salvar os movimentos possiveis
        struct move res;//res - resultado de analisa (o melhor jogada possivel na situacao)
        //available(arr, player, 1);
        r = analize(arr, player, m);//definimos o numero de jogadas possiveis
        //printf("Analisou\n");
        if((player==-1 && n<50) || (player==1 && n>=50)){//se o jogador eh -1 vamos escolher o movimento com o menor valor (melhor para jogador -1)
            res.pont=100000;//pontuacao para comparacao
            for(int i=0; i<r; i++){//fazemos cada jogada possivel no tabuleiro temporal
                copy(arr, arr2);
                movimento(player, m[i].x, m[i].y, arr2, 1);
                //printf("Begin: Nivel: %d, Ineracao: %d/(%d)\n", iteracao ,i+1, r);
                m[i].pont=miniMAX(arr2, player*(-1), iteracao+1, turn, n, alpha, beta).pont;//shamamos recursivamente a avalicao desse movimento
                /*
                for(int j=1;j<iteracao;j++){
                        printf("   ");
                }
                printf("(%d) Movimento: %d %d pontos: %.3f\n", player, m[i].x+1, m[i].y+1, m[i].pont);
*/
                res = mini(m[i], res);//escolhemos o movimento com o menor pontuacao possivel (melhor para jogador -1)
                if(res.pont<beta){//vemos se essa jogada vai dar o maior vantajem para jogador atual do que outras jogadas no nivel anterior
                    beta=res.pont;
                }
                if(beta<=alpha){//se eh verdade acabamos analizar este movimento como outro jogador, quem joga optimal nao vai fazer o movimento que dar a vantajem para jogador atul
                    break;
                    //printf("Pruning: %.3f<%.3f\n", beta,alpha);
                }
            }
        }else if((player==1 && n<50) || (player==-1 && n>=50)){//se o jogador eh 1 vamos escolher o movimento com o maior valor (melhor para jogador 1)
            res.pont=-100000;//pontuacao para comparacao
            for(int i=0; i<r; i++){//fazemos cada jogada possivel no tabuleiro temporal
                copy(arr, arr2);
                movimento(player, m[i].x, m[i].y, arr2, 1);
                //printf("Begin: Nivel: %d, Ineracao: %d/(%d)\n", iteracao ,i+1, r);
                m[i].pont=miniMAX(arr2, player*(-1), iteracao+1, turn, n, alpha, beta).pont;//shamamos recursivamente a avalicao desse movimento
/*
                for(int j=1;j<iteracao;j++){
                        printf("   ");
                }
                printf("(%d) Movimento: %d %d pontos: %.3f\n", player, m[i].x+1, m[i].y+1, m[i].pont);
*/
                res = maxi(m[i], res);//escolhemos o movimento com o maior pontuacao possivel (melhor para jogador 1)
                if(res.pont>alpha){//vemos se essa jogada vai dar o maior vantajem para jogador atual do que outras jogadas no nivel anterior
                    alpha=res.pont;
                }
                if(beta<=alpha){//se eh verdade acabamos analizar este movimento como outro jogador, quem joga optimal nao vai fazer o movimento que dar a vantajem para jogador atul
                    break;
                    //printf("Pruning: %.3f<%.3f\n", beta,alpha);
                }
            }
        }
        //printf("Res: Nivel: %d, (%d %d) %d\n", iteracao, res.x, res.y, res.pont);
        return res;//retornamos o melhor movimento possivel nessa situacao
    }
}

void vez(int tipo, int veses)//funcao geral de joga contra computador
{
    int arr[8][8];//vetor geral de tabuleiro
    int turn=1, n=0;//n -numero de rede primeiro
    int x,y,player=1, r=0;//x,y - para movimento do jogador
    int bom1[50][2];//vetores de notas das redes
    int bom2[50][2];
    inicializar(arr);
    struct move s;//struct para movimento de computador
    if(tipo!=1){//se a pessoa vai jogar escrevemos sobre isso
        regras();
    }
    if(tipo==2){//escolhemos a melhor rede que joga primeira
        FILE *fptr1;
        char filename[10], c;
        sprintf(filename, "Notas2.txt");
        fptr1 = fopen(filename, "r");
        fscanf(fptr1, "%f", &n);
        fclose(fptr1);
    }
    if(tipo==3){//escolhemos a melhor rede que joga segunda
        FILE *fptr1;
        char filename[10], c;
        sprintf(filename, "Notas1.txt");
        fptr1 = fopen(filename, "r");
        fscanf(fptr1, "%f", &n);
        fclose(fptr1);
    }
    for(int i=n;i<n+veses;i++){//ciclo para treinar cada rede neural (ou jogar contra uma)
        bom1[i][0]=i;//numeramos as redes
        bom2[i][0]=i+50;
        bom1[i][1]=0;//zeramos as notas
        bom2[i][1]=0;
        inicializar(arr);//iniciamos o tabuleiro
        player=1;
        turn=1;
        while(turn<61){//ciclo geral de jogo
            //if(turn==59)printf("Turn: %d = %d\n", turn, (conta(arr, 1,0)+conta(arr,-1,0)-4)+1);
            if(available(arr, player, 0)==0 && tipo!=1){ //se o jogador atual nao tem lugar para jogar trocamos o jogador
                //system("cls");
                //available(arr, player, 1);
                printf("Jogador %d Nao tem lugar para jogar passa por outro jogador\n", player);
                player=player*(-1);
            }
//-----------------------parte para jogar como pessoa------------------------------------
            if((tipo==2 && player==1)||(tipo==3 && player==-1)||(tipo==4)){
                available(arr, player, 1);
                if(player==1){ //escrevemos o jogador atual
                    printf("Jogador (X):\n");
                }else{
                    printf("Jogador (O):\n");
                }
                scanf("%d%d",&x,&y);// e lemos o movimento
                if(arr[x-1][y-1]==0){//se o ponto eh vazil continuamos
                    if(movimento(player,x-1,y-1,arr,1)==0){ //se nao temos linha possivel nesse posicao escrevemos sobre isso
                        //system("cls");
                        available(arr, player, 1);
                        printf("Este movimeno eh impossivel, tente mais uma vez\n");//e pedimos repetir a tentetiva
                    }else{
                        //system("cls"); //em outro caso fazemos o movimento
                        available(arr, player*(-1), 1);
                        conta(arr,player, 1);
                        player=player*(-1);//trocmos o jogado
                        turn=turn+1; // e aumentamos o vez
                    }
                }else{ // se o ponto eh ocupado escrevemos sobre isso
                    //system("cls");
                    available(arr, player, 1);
                    printf("Este movimeno eh impossivel, tente mais uma vez\n");//e pedimos repetir a tentetiva
                }
//------------------parte de algoritmo com rede neural para jogador -1 (O)-----------------------------
            }else if((tipo==2 && player==-1) || (tipo==1 && player==-1)){
                //chamamos algoritmo, com tabuleiro, jogador atual, nivel atual de analize = 1
                //turn - ves atual, i - numero de rede (entre 0 e 49) e pesos para comporacao
                s = miniMAX(arr, player, 1, turn, i, -10000000.0, 10000000.0);
                if(tipo!=1){
                    available(arr, player, 1);
                    printf("Jogador (O): %d %d\n", s.x+1, s.y+1);
                }
                movimento(player, s.x, s.y, arr, 1);//fazemos movimento escolido
                player=player*(-1);//trocamos jogador e vez atual
                turn++;
//------------------parte de algoritmo com rede neural para jogador 1 (X)-----------------------------
            }else if((tipo==3 && player==1) || (tipo==1 && player==1)){
                //chamamos algoritmo, com tabuleiro, jogador atual, nivel atual de analize = 1
                //turn - ves atual, i - numero de rede (entre 50 e 99) e pesos para comporacao
                s = miniMAX(arr, player, 1, turn, 99-i,  -10000000.0, 10000000.0);
                if(tipo!=1){
                    available(arr, player, 1);
                    printf("Jogador (X):  %d %d\n", s.x+1, s.y+1);
                }
                movimento(player, s.x, s.y, arr, 1);//fazemos movimento escolido
                player=player*(-1);//trocamos jogador e vez atual
                turn++;
            }
//------------Parte para controlar qual movimento esta no jogo dos redes neurais------------------------
            if(tipo==1){
                printf("%d~",turn);//escrevemos que o jogo terminou
            }else{
                conta(arr, player, 1);
            }
//----------------------------Parte para ver se tem lugares para jogar----------------------------------
            if((turn!=61) && (available(arr, -1, 0)==0) && (available(arr, 1, 0)==0)){//se nao temos lugar para jogar para ninguem - terminamos
                printf("Nao tem mais lugar para jogar\n");
                available(arr, 1, 1);
                turn = 62;
            }
        }
//--------------------------------Depos de jogo--------------------------------
        conta(arr, 1, 1);//escrevemos a conta final e o vencedor
        printf("(%d) ",i);
        vencedor(arr);
        bom1[i][1]=conta(arr,1, 0);//a nota de rede = conta final
        bom2[i][1]=conta(arr,-1, 0);
        if(conta(arr,-1, 0)<conta(arr,1, 0)){//se a rede vence a conta eh +10
            bom1[i][1]=bom1[i][1]+10;
        }else if(conta(arr,-1, 0)>conta(arr,1, 0)){
            bom2[i][1]=bom2[i][1]+10;
        }
    }
//-------------------------Parte de evolucao de rede----------------------------
    if(tipo==1){
        bubble_Sort(bom1);//sorteamos as redes de accordo com notas deles
        bubble_Sort(bom2);
        printf("~Sortei~\n");//escrevemos sobre isso
        FILE *fptr1, *fptr2;//salvamos as notas e numeras de redes para files
        char filename[10], c;
        sprintf(filename, "Notas1.txt");
        fptr1 = fopen(filename, "w");
        sprintf(filename, "Notas2.txt");
        fptr2 = fopen(filename, "w");
        for(int i=0;i<50; i++){//parte para escrever notas de cada rede
            printf("%d = %d\n",bom1[i][0],bom1[i][1]);
            fprintf(fptr1,"%d %d\n",bom1[i][0],bom1[i][1]);
        }
        printf("\n");
        for(int i=0;i<50; i++){//parte para escrever notas de cada rede
            printf("%d = %d\n",bom2[i][0],bom2[i][1]);
            fprintf(fptr2,"%d %d\n",bom2[i][0],bom2[i][1]);
        }
        fclose(fptr1);
        fclose(fptr2);
        evolv(bom1);//shamamos a evolucao
        evolv(bom2);
    }
    return ;
}

int main()//funcao inciador de jogo, shama outros funcoes dependendo de condicoes de joga escolhidos
{
    srand(time(0));//para fazer numeros aleatorios
    int tipo=-1, criar, veses=-1, treinar=-1;//tipo de jogo, criar a rede?, veses - quantidade de iteracoes de educacao de rede
    do{
        printf("Voce quer jogar contra pessoa? - [1], ou contra computador(treinamento do rede esta aqui)? - [2]\n");
        scanf("%d",&tipo);
    }while(!(tipo==1 || tipo==2));
    if(tipo==1){
        vez(4,1);
    }else if(tipo==2){
        do{
            printf("Voce quer criar pesos para rede? SIM - [1], NAO - [2]\n");
            scanf("%d",&criar);
        }while(!(criar==1 || criar==2));
        if(criar==1){
            inipesos();
        }
        do{
            printf("Voce quer treinar a rede - [1], ou  jogar contra dela - [2]\n");
            scanf("%d",&treinar);
        }while(!(treinar==1 || treinar==2));
        if(treinar==1){
            do{
                printf("Quantas veses voce quer treinar a rede (quantas veses ela vai evoluir)?:\n");
                scanf("%d",&veses);
            }while(veses<1);
            for(int i=0;i<veses;i++){//ciclo de repeticao de jogo
                printf("[%d]\n",i);//escrevemos quando o ciclo terminou
                vez(1,50);
            }
        }else if(treinar==2){
            tipo=-1;
            do{
                printf("Voce quer jogar primeiro - [1], ou  segundo - [2]\n");
                scanf("%d",&tipo);
            }while(!(tipo==1 || tipo==2));
            if(tipo==1){
                vez(2,1);
            }else{
                vez(3,1);
            }
        }
    }
    return 0;
}

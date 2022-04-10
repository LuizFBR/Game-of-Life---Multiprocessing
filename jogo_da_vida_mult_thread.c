#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


int m;
int n;

int **matriz;

typedef struct{
    int y;
    int ** matriz;
} Args;


void print_matriz(int **matriz, int lin, int col){
    for (int i = 0; i < lin; i++){
        for (int j = 0; j < col; j++){
            printf("%d ",matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void free_matriz(int **matriz, int lin, int col){
    for (int i = 0; i < lin; i++)
        free(matriz[i]);
    free(matriz);
}


int **aloca_matriz(int lin, int col){
    int **matrix = (int **) malloc(lin * sizeof(int *)); 
    for(int i = 0 ; i < lin ; i++){
        matrix[i] = (int*) malloc(col * sizeof(int));
    }
    return matrix;
}

int **ler_matriz(){
    scanf("%d %d\n", &m, &n);
    matriz = aloca_matriz(m, n);
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            if( i == m - 1)
                scanf("%d\n", &matriz[i][j]);
            else
                scanf("%d ", &matriz[i][j]);
        }
    }
    return matriz;
}

int parse_vizinhanca(int x, int y){
    int vivas = 0; // contagem de células vizinhas vivas;
    for(int i = -1 ; i < 2 ; i++){
        for(int j = -1 ; j < 2 ; j++){
            if( x + i < 0 || y + j < 0 || x + i >= m || y + j >= n || (i == 0 && j == 0)) // verifica apenas as células vizinhas
                vivas = vivas; // células que se encontram fora dos limites da matriz são consideradas mortas
            else
                vivas += matriz[x+i][y+j];
        }
    }
    return vivas;
}

void *processa_linha(void *p){
    Args *args = (Args*) p;
    int y = args->y;
    int ** novo = args->matriz;
    for( int x = 0 ; x < m ; x++ ){
        int vivas = parse_vizinhanca(x,y);
        if( vivas < 2 || vivas > 3) // morre por superpolulação ou por sobpopulação
            novo[x][y] = 0;
        else if(vivas == 3) // se tem 3 vizinhas vivas, então a célula começa a viver
            novo[x][y] = 1;
        else // se há duas células então 
            novo[x][y] = matriz[x][y];
    }
}


void jogo_da_vida(int geracoes){

    pthread_t t_arr[n];
    Args args[n];

    printf("Começando...\n");
    print_matriz(matriz, m, n);
    sleep(1);

    for(int geracao = 0 ; geracao < geracoes ; geracao++){
        int **novo = aloca_matriz(m, n);
        sleep(1);
        for( int y = 0 ; y < n ; y++){
            args[y].y = y; args[y].matriz = novo;
            pthread_create(&(t_arr[y]), NULL, processa_linha, &args[y]);
        }
        for( int i = 0 ; i < n ; i++ )
            pthread_join(t_arr[i], NULL);
        print_matriz(novo, m, n);
        free_matriz(matriz, m, n);
        matriz = novo;
    }
}

int main(){

    ler_matriz(matriz);

    int numero_de_geracoes = 15;
    jogo_da_vida(numero_de_geracoes);

    free_matriz(matriz, m, n);

    return 0;
}
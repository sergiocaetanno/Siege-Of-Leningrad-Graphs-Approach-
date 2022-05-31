#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>

#define vertex int

static int separa (int v[], int p, int r) {
   int c = v[r]; // pivô
   int t, j = p;
   for (int k = p; k < r; ++k)
      if (v[k] <= c) {
         t = v[j], v[j] = v[k], v[k] = t;
         ++j;
      }
   t = v[j], v[j] = v[r], v[r] = t;
   return j;
}


void quicksort (int v[], int p, int r) {
   if (p < r) {                   // 1
      int j = separa (v, p, r);   // 2
      quicksort (v, p, j-1);      // 3
      quicksort (v, j+1, r);      // 4
   }
}

static int **MATRIXint(int r, int c, int val) {
   int **m = malloc( r * sizeof (int *));
   for (vertex i = 0; i < r; ++i)
      m[i] = malloc( c * sizeof (int));
   for (vertex i = 0; i < r; ++i)
      for (vertex j = 0; j < c; ++j)
         m[i][j] = val;
    return m;
}

struct graph {
    int V;
    int A;
    int **adj;
};

typedef struct graph *Graph;

Graph GRAPHinit(int V) {
   Graph G = malloc(sizeof *G);
   G->V = V;
   G->A = 0;
   G->adj = MATRIXint( V, V, -1);
   return G;
}

void GRAPHinsertArc(Graph G, vertex v, vertex w, int p) {
    v = v - 1;
    w = w - 1;
   if (G->adj[v][w] == -1) {
      G->adj[v][w] = p;
      G->A++;
   }
}

void GRAPHshow(Graph G) {
   for (vertex v = 0; v < G->V; ++v) {
      printf( "%2d:", (v+1));
      for (vertex w = 0; w < G->V; ++w)
         if (G->adj[v][w] != -1)
            printf( " %d(%d)", (w+1), G->adj[v][w]);
      printf( "\n");
   }
}

int existeAberto (Graph G, int aberto[]){ // 0 represents false while 1 represents true
    for (int i = 0; i < G->V; i++){
        if(aberto[i] == 1){
            return 1;
        }
    }
    return 0;
}

int menorDist(Graph G, int aberto[], int distanciaVertices[]) {

    int i;
    for (i = 0; i < G->V; i++){
        if(aberto[i]==1){
            break;
        }
    }

    if (i==G->V){
        return -1;
    }

    int menor = i;

    for(i=menor+1; i< G->V; i++){
        if(aberto[i]==1 && distanciaVertices[menor]>distanciaVertices[i]){
            menor = i;
        }
    }
    return(menor);
}

void relaxa(Graph G, int predecessores[], int dV[], vertex u, vertex w) {

    if(G->adj[u][w] != -1 && dV[w] > dV[u] + G->adj[u][w]){

        dV[w] = dV[u] + G->adj[u][w];
        predecessores[w] = u;
    }

}

int *dijkstra (Graph G, vertex raiz) {

    int *predecessores, *distanciaVertices;
    int aberto[G->V]; // 0 represents false while 1 represents true

    //Inicializa predecessores, lista de distâncias e vetor aberto

    predecessores = malloc((G->V) * sizeof (int));
    distanciaVertices = malloc((G->V) * sizeof (int));

    for (int i = 0; i < G->V; i++){

        predecessores[i] = -1;
        distanciaVertices[i] = INT_MAX/2;
        aberto[i] = 1;
    }

    distanciaVertices[raiz-1] = 0;

    while(existeAberto(G, aberto) == 1) {

        int u = menorDist(G, aberto, distanciaVertices);

        aberto[u] = 0;

        for(int i = 0; i < (G->V); i++){

            if(G->adj[u][i]!=-1){

                relaxa(G, predecessores, distanciaVertices, u, i);
            }
        }
    }
    return distanciaVertices;
}

double siegeLeningrad(int n, int m, int k, float p, int tropa, int inicio, int fim, int la[m][3], int tPos[]) {

    int tLa = 2 * m;

    quicksort(tPos, 0, (tropa-1));

    int i=0, j= 0;

    int peso[n];

    for(int i=0; i<n; i++){
        peso[i] = 0;
    }

    while(j<(n) && i<tropa){
        if(tPos[i]!= (j+1)){
            j++;
        } else {
            peso[j]++;
            i++;
        }
    }

    for(int i = 0; i < tLa; i++){
        la[i][2] = peso[(la[i][1] - 1)];
    }

    Graph G = GRAPHinit(n);

    for (int i = 0; i < tLa; i++) {
        GRAPHinsertArc(G, la[i][0], la[i][1], la[i][2]);
    }

    int *distancia = dijkstra (G, inicio);

    for (int i = 0; i < n; i++) {
        distancia[i] = distancia[i] + peso[inicio-1];
    }

    double probabilidade;

    if(distancia[(fim - 1)] > k) {
        probabilidade = 0;
        return probabilidade;
    } else {
        probabilidade = pow(p,distancia[(fim - 1)]);
        return probabilidade;
    }
}

int main(void) {

    int n = 0, m = 0, k = 0; //n = número de pontos estratégicos; m = ruas que conectam os pontos; k = quantidade de munição por soldado.
    float p; // probabilidade de acerto do soldado soviético.
    int tropa; //número total de soldados
    int tempPos;
    int tempini, tempfim;
    int inicio, fim;
    float *probabilidades, *probabilidadesTemp;
    int tempProb = 1;

    probabilidades = malloc(tempProb * sizeof (float));
    probabilidadesTemp = malloc(tempProb * sizeof (float));

    while(scanf("%i %i %i %f", &n, &m, &k, &p) != EOF){

        if(tempProb != 1) {

            for(int i = 0; i < tempProb-1; i++) {

                *(probabilidadesTemp + i) = *(probabilidades + i);
            }

            probabilidades = malloc(tempProb * sizeof (float));

            for(int i =0; i < tempProb-1; i++) {

                *(probabilidades + i)= *(probabilidadesTemp + i);
            }
        }

        if(n > 1000 || n < 2 || m < 0 || k > 1000 || k < 0 || p > 1 || p < 0) {
            return 0;
        }

        int tLa = 2 * m; // Tamanho da lista de adjacências

        int la[tLa][3]; //Lista de adjacências.

        for(int i = 0;i < tLa; i+=2) {
            scanf("%i %i", &tempini, &tempfim);
            la[i][0] = tempini;
            la[i][1] = tempfim;
            la[i+1][0] = tempfim;
            la[i+1][1] = tempini;
            la[i][2] = 0;
            la[i+1][2] = 0;
        }

        scanf("%i", &tropa);

        int tPos[(tropa)]; //Lista de posicionamento das tropas


        for (int i = 0; i < tropa; i++) {
            scanf("%i",&tPos[i]);
        }

        scanf("%i %i", &inicio, &fim);

        float resultado = siegeLeningrad(n, m, k, p, tropa, inicio, fim, la, tPos);

        *(probabilidades + tempProb-1) = resultado;
        tempProb += 1;

    }

    for(int i=0; i< tempProb-1; i++){
        printf("%.3f\n",*(probabilidades + i));
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <time.h>

#include <assert.h>

#define SIZE_ALLOC 10
#define SIZE_POPULATION 100
#define PRECISION 10000
#define SIZE_BIT 26
#define N 15

#define NOM_FICHIER "Doc/A_Genetique_Profil.txt"

/************************************************
Valeurs approchées :

  G[i].L.Y=rand()%(3000 - 1500) + 1500;              0.15 < Y < 0.30   ==>   1500 < Y < 3000   ==>   12 bits
  G[i].L.B=rand()%60000;                             0 < B < 6         ==>   0 < B < 60000     ==>   16 bits
  G[i].L.l=rand()%(65640000 - 65600000) + 65600000;  6560 < l < 6564   ==>     l < 65640000    ==>   26 bits
  G[i].L.d=rand()%(300000 - 100000) + 100000;        10 < d < 30       ==>  10000 < L < 30000  ==>   15 bits

************************************************/


//https://stackoverflow.com/questions/6418807/how-to-work-with-complex-numbers-in-c

typedef
struct {
  unsigned int* bit_Y;
  unsigned int* bit_B;
  unsigned int* bit_l;
  unsigned int* bit_d;
  double c;
} Bit;


//Definition For the 4 variables
typedef
struct {
    unsigned int Y;
    unsigned int B;
    unsigned int l;
    unsigned int d;
} Lorentz;


//Definition of a Gene, with the 4 parameters we need to discover and the coefficient
typedef
struct{
  Lorentz L;
  double c;
} Gene;


//Definition of 3 float for the profil we have
typedef
struct {
    float A;
    float B;
    float C;
} Val;


Val* lecture(char* nomFichier, int* i){

  FILE *f = NULL;
  unsigned int test;

  if ((f = fopen(nomFichier, "r")) == NULL) {
    printf("Impossible d'ouvrir le fichier !\n");
    exit(-1);
  }

  int bloc_alloc = SIZE_ALLOC; // On réalloc par bloc pour gagner du temps

  Val *t = malloc(sizeof(Val) * SIZE_ALLOC);
  if (t == NULL) {
    printf("Erreur lors de l'allocation de l'esapce mémoire\n");
    exit(-1);
  }
  printf("\n\nDébut du lecture du fichier... \n\n");
  while(!feof(f)){
    //printf("## Lecture ligne numéro: i=%d  ## Taille alloué au tableau Val: bloc_alloc=%d\n", *i, bloc_alloc);
    if (*i >= bloc_alloc) { // Compteur pour savoir quand realloc
      bloc_alloc += SIZE_ALLOC;
      //printf("\n## REALLOCATION : Nouvelle taille: %d\n\n", bloc_alloc);
      t = realloc(t, sizeof(t)*(bloc_alloc) * sizeof(Val));
    }

    fflush(stdin);
    test = fscanf(f,"%f %f %f",&t[*i].A,&t[*i].B,&t[*i].C);
    if(test != 3){
      printf("\n\nErreur de lecture dans les lignes du fichier !\n\n");
      exit(-1);
    }
    *i = *i + 1;

    //printf("%d : %f\t%f\t%f\n",*i,X[*i],Y[*i],Z[*i]);
  }

  //Close the file (free memory)
  printf("\n\nAllocation de %d blocs mémoire pour la lecture.\n\n", bloc_alloc);
  printf("\n\nLecture du fichier terminé ! \n\n");
  fclose(f);
  return t;
}


//Make binary with integer
void int_To_Bit(Gene G, Bit *B){
  unsigned int tmp_Gene = G.L.Y,i = 0;
  while(tmp_Gene != 0){
    //printf("%d %d\n",tmp_Gene,tmp_Gene%2);
    B->bit_Y[i] = tmp_Gene%2;
    tmp_Gene = tmp_Gene/2;
    i++;
  }

  tmp_Gene = G.L.B;
  i = 0;
  while(tmp_Gene != 0){
    //printf("%d %d\n",tmp_Gene,tmp_Gene%2);
    B->bit_B[i] = tmp_Gene%2;
    tmp_Gene = tmp_Gene/2;
    i++;
  }

  tmp_Gene = G.L.l;
  i = 0;
  while(tmp_Gene != 0){
    //printf("%d %d\n",tmp_Gene,tmp_Gene%2);
    B->bit_l[i] = tmp_Gene%2;
    tmp_Gene = tmp_Gene/2;
    i++;
  }

  tmp_Gene = G.L.d;
  i = 0;
  while(tmp_Gene != 0){
    //printf("%ld %ld\n",tmp_Gene,tmp_Gene%2);
    B->bit_d[i] = tmp_Gene%2;
    tmp_Gene = tmp_Gene/2;
    i++;
  }
}


//Make integer with binary
void bit_To_Int(Gene *G, Bit B){
  unsigned int tmp_Y = 0,tmp_B = 0, tmp_l = 0, tmp_d = 0 , p;
  for(int i = 0;i<SIZE_BIT;i++){
    p = pow(2,i);
    //printf("Bit : %d\t%d %d\n",i,B.bit_Y[i],B.bit_B[i]);
    tmp_Y = tmp_Y + B.bit_Y[i]*p;
    tmp_B = tmp_B + B.bit_B[i]*p;
    tmp_l = tmp_l + B.bit_l[i]*p;
    tmp_d = tmp_d + B.bit_d[i]*p;
  }
  //printf("%d %d\n",tmp_Y, tmp_B);
  G->L.Y = tmp_Y;
  G->L.B = tmp_B;
  G->L.l = tmp_l;
  G->L.d = tmp_d;
  //printf("\n");
}


//Convert a Int population to a Bit population
void convert_Bit(Gene *G, Bit *B){
  for(int i = 0; i<SIZE_POPULATION;i++){
    int_To_Bit(G[i],&B[i]);
  }
}


//Convert a Bit population to a Int population
void convert_Int(Gene *G, Bit *B){
  for(int i = 0; i<SIZE_POPULATION;i++){
    bit_To_Int(&G[i],B[i]);
  }
}


//Create an initial population
void create_population(Gene *G)
{
  srand(time(NULL)); // initialisation de rand
    for(int i=0;i<SIZE_POPULATION;i++)
    {
      G[i].L.Y=rand()%(2500 - 150) + 150;
      G[i].L.B=rand()%(70000 - 50000) + 50000;
      G[i].L.l=rand()%(65640000 - 65600000) + 65600000;
      G[i].L.d=rand()%(300000 - 100000) + 100000;
    }
}


//Initialize tab Bit
Bit* init_Bit(){
  Bit* B = malloc(SIZE_POPULATION * sizeof(Bit));
  for(int i = 0;i<SIZE_POPULATION;i++){
    B[i].bit_Y = malloc((SIZE_BIT) * sizeof(unsigned int));
    B[i].bit_B = malloc((SIZE_BIT) * sizeof(unsigned int));
    B[i].bit_l = malloc((SIZE_BIT) * sizeof(unsigned int));
    B[i].bit_d = malloc((SIZE_BIT) * sizeof(unsigned int));
  }
  return B;
}


//Test the population
void test_population(Gene *G, Val *V, int size_tab){
  double complex tmp;
  double module, moy;
  for(int i = 0;i<SIZE_POPULATION;i++){
    moy = 0;
    for(int j = 0;j<size_tab;j++){
      tmp = (double)(G[i].L.Y)/PRECISION + ((double)(G[i].L.B)/PRECISION)*(I/(cpow(V[j].A-(double)(G[i].L.l)/PRECISION,2) + cpow(((double)(G[i].L.d)/PRECISION)/2,2)));
      module = sqrt(pow(creal(tmp),2) + pow(cimag(tmp),2));
      moy = moy + fabs(V[j].C - module);
    }
    moy = moy / size_tab;
    G[i].c = moy;
    /*moy = 0;
    tmp = (double)(G[i].L.Y)/PRECISION + ((double)(G[i].L.B)/PRECISION)*(I/(cpow(6500-(double)(G[i].L.l)/PRECISION,2) + cpow(((double)(G[i].L.d)/PRECISION)/2,2)));
    module = sqrt(creal(tmp)*creal(tmp) + cimag(tmp)*cimag(tmp));
    moy = moy + (V[159].C - module);
    tmp = (double)(G[i].L.Y)/PRECISION + ((double)(G[i].L.B)/PRECISION)*(I/(cpow(6550-(double)(G[i].L.l)/PRECISION,2) + cpow(((double)(G[i].L.d)/PRECISION)/2,2)));
    module = sqrt(creal(tmp)*creal(tmp) + cimag(tmp)*cimag(tmp));
    moy = moy + (V[442].C - module);
    tmp = (double)(G[i].L.Y)/PRECISION + ((double)(G[i].L.B)/PRECISION)*(I/(cpow(6562-(double)(G[i].L.l)/PRECISION,2) + cpow(((double)(G[i].L.d)/PRECISION)/2,2)));
    module = sqrt(creal(tmp)*creal(tmp) + cimag(tmp)*cimag(tmp));
    moy = moy + (V[503].C - module);
    tmp = (double)(G[i].L.Y)/PRECISION + ((double)(G[i].L.B)/PRECISION)*(I/(cpow(6573-(double)(G[i].L.l)/PRECISION,2) + cpow(((double)(G[i].L.d)/PRECISION)/2,2)));
    module = sqrt(creal(tmp)*creal(tmp) + cimag(tmp)*cimag(tmp));
    moy = moy + (V[568].C - module);
    tmp = (double)(G[i].L.Y)/PRECISION + ((double)(G[i].L.B)/PRECISION)*(I/(cpow(6600-(double)(G[i].L.l)/PRECISION,2) + cpow(((double)(G[i].L.d)/PRECISION)/2,2)));
    module = sqrt(creal(tmp)*creal(tmp) + cimag(tmp)*cimag(tmp));
    moy = moy + (V[716].C - module);
    moy = moy / 5;
    G[i].c = moy;*/
  }
}


//Sort the population https://fr.wikibooks.org/wiki/Impl%C3%A9mentation_d%27algorithmes_classiques/Algorithmes_de_tri/Tri_rapide
int partitionnerInv(Gene* T, int p, int r){
  int pivot = p;
  int i = p-1;
  int  j = r+1;
  Gene temp;
  while (1) {
      do
          j--;
      while (T[j].c > T[pivot].c);
      do
          i++;
      while (T[i].c < T[pivot].c);
      if (i < j) {
          temp = T[i];
          T[i] = T[j];
          T[j] = temp;
      }
      else
          return j;
  }
}


void tri_rapide(Gene* T, int ind_premier, int ind_dernier){
  int ind_pivot;
  if(ind_premier < ind_dernier){
    ind_pivot = partitionnerInv(T, ind_premier, ind_dernier);
    tri_rapide(T, ind_premier, ind_pivot);
    tri_rapide(T, ind_pivot +1, ind_dernier);
  }
}


//Put together gene
void mix_population(Bit *B){

  for(int i = 0;i<(SIZE_POPULATION/2);i++){

    for(int j = 0; j<(SIZE_BIT/2);j++){

      //12 bits max pour Y
      if (j < 6){
        B[i+(SIZE_POPULATION)/2].bit_Y[j] = B[i].bit_Y[j];
        B[i+(SIZE_POPULATION)/2].bit_Y[j+6] = B[(i+1)%(SIZE_POPULATION/2)].bit_Y[j+6];
      }

      //16 bits max pour B et d
      if (j < 8){
        B[i+(SIZE_POPULATION)/2].bit_B[j] = B[i].bit_B[j];
        B[i+(SIZE_POPULATION)/2].bit_B[j+8] = B[(i+1)%(SIZE_POPULATION/2)].bit_B[j+8];

        B[i+(SIZE_POPULATION)/2].bit_d[j] = B[i].bit_d[j];
        B[i+(SIZE_POPULATION)/2].bit_d[j+8] = B[(i+1)%(SIZE_POPULATION/2)].bit_d[j+8];
      }

      //26 bits max pour l
      B[i+(SIZE_POPULATION)/2].bit_l[j] = B[i].bit_l[j];
      B[i+(SIZE_POPULATION)/2].bit_l[j+13] = B[(i+1)%(SIZE_POPULATION/2)].bit_l[j+13];

    }
  }
}


//Mutate a bit
void mutation(Bit *B){
  int r;

  for(int i = 50;i<SIZE_POPULATION;i++){
    //12 bits max pour Y
    r = rand()%12;
    B[i].bit_Y[r] = 1 - B[i].bit_Y[r];

    //16 bits max pour B et d
    r = rand()%16;
    B[i].bit_B[r] = 1 - B[i].bit_B[r];

    r = rand()%16;
    B[i].bit_d[r] = 1 - B[i].bit_d[r];

    //26 bits max pour Y
    r = rand()%26;
    B[i].bit_l[r] = 1 - B[i].bit_l[r];
  }
}


int main() {

    Gene* pop = malloc((SIZE_POPULATION + 1)*sizeof(Gene));
    Bit* pop_Bit = init_Bit();

    //Read the file
    int size_Tab = 0;
    Val* tab = lecture(NOM_FICHIER,&size_Tab);


    //Create population
    create_population(pop);


    test_population(pop, tab, size_Tab);

    tri_rapide(pop, 0, SIZE_POPULATION-1);


    //Convert int in bit
    for(int tmp = 0;tmp < 100; tmp++){

      printf("Itération: %d\n",tmp);

      convert_Bit(pop,pop_Bit);

      mix_population(pop_Bit);

      mutation(pop_Bit);

      convert_Int(pop,pop_Bit);
      test_population(pop, tab, size_Tab);
      tri_rapide(pop, 0, SIZE_POPULATION-1);

      //printf("tmp : %d\n",tmp);

    }

    printf("\n\n%f %f %f %f\n",(double)pop[0].L.Y/PRECISION,(double)pop[0].L.B/PRECISION,(double)pop[0].L.l/PRECISION,(double)pop[0].L.d/PRECISION);

    //Free all the table
    //free(tab);
    return 0;
}

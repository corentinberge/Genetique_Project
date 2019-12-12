#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <time.h>

#include <assert.h>

#define SIZE 10
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
  unsigned int bit_Y[SIZE_BIT];
  unsigned int bit_B[SIZE_BIT];
  unsigned int bit_l[SIZE_BIT];
  unsigned int bit_d[SIZE_BIT];
  double c;
} Bit;


//Definition For the 4 variables
typedef
struct {
    int Y;
    int B;
    int l;
    int d;
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


// //Read a file and store data
// void lecture(FILE* f, Val* t, int* i){
//
//     while(!feof(f)){
//         *i = *i + 1;
//         if ((*i)%SIZE == 0){
//             printf("\nERREUR INSTANT DE MEMOIRE!\n\n");
//             t = realloc(t, SIZE * sizeof(Val));
//         }
//         fflush(stdin);
//         fscanf(f,"%f %f %f",&t[*i].A,&t[*i].B,&t[*i].C);
//         //printf("%d : %f\t%f\t%f\n",*i,X[*i],Y[*i],Z[*i]);
//     }
//     *i = *i + 1;
// }

Val* lecture(char* nomFichier, int* i){

  FILE *f = NULL;
  if ((f = fopen(nomFichier, "r")) == NULL) {
    printf("Impossible d'ouvrir le fichier !\n");
    exit(-1);
  }

  int bloc_alloc = SIZE; // On réalloc par bloc pour gagner du temps

  Val *t = malloc(sizeof(Val) * 10);
  if (t == NULL) {
    printf("Erreur lors de l'allocation de l'esapce mémoire\n");
    exit(-1);
  }
  printf("\n\nDébut du lecture du fichier... \n\n");
  while(!feof(f)){
    printf("## Lecture ligne numéro: i=%d  ## Taille alloué au tableau Val: bloc_alloc=%d\n", *i, bloc_alloc);
    if (*i >= bloc_alloc) { // Compteur pour savoir quand realloc
      bloc_alloc += 10;
      printf("\n## REALLOCATION : Nouvelle taille: %d\n\n", bloc_alloc);
      t = realloc(t, sizeof(t)*(bloc_alloc) * sizeof(Val));
    }

    fflush(stdin);
    fscanf(f,"%f %f %f",&t[*i].A,&t[*i].B,&t[*i].C);
    *i = *i + 1;

    //printf("%d : %f\t%f\t%f\n",*i,X[*i],Y[*i],Z[*i]);
  }

  //Close the file (free memory)
  printf("\n\nLecture du fichier terminé ! \n\n");
  fclose(f);
  return t;
}


//Make binary with integer
void int_To_Bit(Gene G, Bit *B){
  int tmp_Gene = G.L.Y,i = 0;
  while(tmp_Gene != 0){
    //printf("%d %d\n",tmp_Gene,tmp_Gene%2);
    B->bit_Y[i] = tmp_Gene%2;
    tmp_Gene = tmp_Gene/2;
    i++;
  }
}


//Make integer with binary
void bit_To_Int(Gene *G, Bit B){
  int tmp_G = 0;
  for(int i = 0;i<SIZE_BIT;i++){
    tmp_G = tmp_G + B.bit_Y[i]*pow(2,i);
  }
  G->L.Y = tmp_G;
}


//Create an initial population
void create_population(Gene *G)
{
  srand(time(NULL)); // initialisation de rand
    for(int i=0;i<SIZE_POPULATION;i++)
    {
      G[i].L.Y=rand()%(3000 - 150) + 150;
      G[i].L.B=rand()%60000;
      G[i].L.l=rand()%(65640000 - 65600000) + 65600000;
      G[i].L.d=rand()%(300000 - 100000) + 100000;
    }
}


//Test the population
void test_population(Gene *G){
  double complex tmp_1;//,tmp_2,tmp_3;
  tmp_1 = (double)(G[0].L.Y)/PRECISION + ((double)(G[0].L.B)/PRECISION)*(I/(cpow(6500-(double)(G[0].L.l)/PRECISION,2) + cpow(((double)(G[0].L.d)/PRECISION)/2,2)));
  printf("%.12f\t%.12f\n", creal(tmp_1), cimag(tmp_1));
  /*for(int i = 0;i<SIZE_POPULATION;i++){
    tmp_1 = G[i].L.Y + G[i].L.B*(I/(pow(6500-G[i].L.l,2) + pow((G[i].L.d)/2,2)));
    printf("")
  }*/
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

      //26 bits max pour Y
      B[i+(SIZE_POPULATION)/2].bit_l[j] = B[i].bit_l[j];
      B[i+(SIZE_POPULATION)/2].bit_l[j+13] = B[(i+1)%(SIZE_POPULATION/2)].bit_l[j+13];

    }

  }
}


int main() {

    Gene pop[SIZE_POPULATION];
    Bit pop_Bit[SIZE_POPULATION];

    //Read the file
    int size_Tab = 0;
    Val* tab = lecture(NOM_FICHIER,&size_Tab);

    //Print initial
    /*for(int j=0;j<size;j++){
        printf("%f %f %f\n",tab[j].A,tab[j].B,tab[j].C);
    }*/


    //Create population
    create_population(pop);
    //Print initial population
    /*for(int j=0;j<SIZE_POPULATION;j++){
      printf("%d %d %d %d\n",pop[j].L.Y,pop[j].L.B,pop[j].L.l,pop[j].L.d);
    }*/

    //Convert int in bit
    for(int i = 0;i<SIZE_POPULATION;i++){
      int_To_Bit(pop[i],&pop_Bit[i]);
    }

    // Gene tableau[N];
    // tableau[0].c = 14;
    // tableau[1].c = 13;
    // tableau[2].c = 12;
    // tableau[3].c = 11;
    // tableau[4].c = 10;
    // tableau[5].c = 9;
    // tableau[6].c = 8;
    // tableau[7].c = 7;
    // tableau[8].c = 5;
    // tableau[9].c = 4;
    // tableau[10].c = 3;
    // tableau[11].c = 98;
    // tableau[12].c = 80;
    // tableau[13].c = 70;
    // tableau[14].c = 60;
    //
    // printf("\nTableau avant le tri rapide...\n");
    // for (int i =0; i < N; ++i){
    //   printf("tableau[%d].c = %f\n", i, tableau[i].c);
    // }
    //
    // tri_rapide(tableau, 0, N-1);
    //
    // printf("\nTableau après le tri rapide...\n");
    //
    // for (int i =0; i < N; ++i){
    //   printf("tableau[%d].c = %f\n", i, tableau[i].c);
    // }
    // TEST for the conversion
    int_To_Bit(pop[0],&pop_Bit[0]);
    int_To_Bit(pop[1],&pop_Bit[1]);
    printf("Chiffre : %d\n",pop[0].L.Y);
    for(int j=0;j<12;j++){
      printf("%d\n",pop_Bit[0].bit_Y[j]);
    }

    printf("Chiffre : %d\n",pop[1].L.Y);
    for(int j=0;j<12;j++){
      printf("%d\n",pop_Bit[1].bit_Y[j]);
    }

    printf("\n");
    mix_population(pop_Bit);
    for(int j=0;j<12;j++){
      printf("%d\n",pop_Bit[50].bit_Y[j]);
    }
    bit_To_Int(&pop[2],pop_Bit[50]);
    printf("Chiffre : %d\n",pop[2].L.Y);


    mix_population(pop_Bit);

    //Print mix population
    /*for(int j=0;j<SIZE_POPULATION;j++){
      printf("%d %d %d %d\n",pop[j].L.Y,pop[j].L.B,pop[j].L.l,pop[j].L.d);
    }*/

  test_population(pop);




    /*Gene test_G,test_G2;
    Bit test_B;
    for(int j = SIZE_BIT-1;j>=0;j--){
      test_B.bit_Y[j] = 0;
    }
    test_G.L.Y = 511;
    int_To_Bit(test_G, &test_B);
    for(int j = SIZE_BIT-1;j>=0;j--){
      printf("%d : %d\n",j,test_B.bit_Y[j]);
    }
    printf("\n");
    bit_To_Int(&test_G2, test_B);
    printf("G : %d\n", test_G2.L.Y);*/

    //Free all the table
    free(tab);
    return 0;
}

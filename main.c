#include <stdio.h>
#include <malloc.h>
#include <math.h>

#define SIZE 5000

//Definition of a Gene, with 4 parameters
struct {
    int Y;
    int B;
    int l;
    int d;
} Lorentz;

//Definition of 3 float for the profil we have
typedef
struct {
    float A;
    float B;
    float C;
} Val;


//Read a file and store data
void lecture(FILE* f, Val* t, int* i){
    while(!feof(f)){
        *i = *i + 1;
        if ((*i)%SIZE == 0){
            printf("\nERREUR INSTANT DE MEMOIRE!\n\n");
            t = realloc(t, SIZE * sizeof(Val));
        }
        fflush(stdin);
        fscanf(f,"%f %f %f",&t[*i].A,&t[*i].B,&t[*i].C);
        //printf("%d : %f\t%f\t%f\n",*i,X[*i],Y[*i],Z[*i]);
    }
    *i = *i + 1;
}


int main() {
    Val *tab = malloc(sizeof(Val) * SIZE);
    if (tab == NULL) {
        printf("Erreur lors de l'allocation de l'esapce mémoire\n");
        return 1;
    }
    int size = -1;
    FILE *fichier = NULL;
    if ((fichier = fopen("/home/corentinberge/Documents/L3_EEA/Tech_Sci/Genetique_Project/Doc/A_Genetique_Profil.txt",
                         "r")) == NULL) {
        printf("Impossible d'ouvrir le fichier !\n");
        return 2;
    }
    lecture(fichier, tab, &size);
    /*for(int j=0;j<size;j++){
        printf("%f %f %f\n",tab[j].A,tab[j].B,tab[j].C);
    }*/


    fclose(fichier);
    free(tab);
    return 0;
}
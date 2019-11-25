#include <stdio.h>
#include <malloc.h>

#define SIZE 2000

//Define a type to have all the informations in one type
typedef union{
    float f;
    struct {
        unsigned int s; //signe
        unsigned int e; //exposant
        unsigned int m; //mantisse
    } Bit;
} Float_Bit;

//Change float on bit
void float_Bit(){

}


//Change bit on float
void bit_Float(){

}


//Read a file and store data
void lecture(FILE* f, Float_Bit* X, Float_Bit* Y, Float_Bit* Z, int* i){
    while(!feof(f)){
        *i = *i + 1;
        if (*i%SIZE == 0){
            X = realloc(X,((SIZE) + (*i)) * sizeof(Float_Bit));
            Y = realloc(Y,((SIZE) + (*i)) * sizeof(Float_Bit));
        }
        fscanf(f,"%f %f %f",&X[*i].f,&Y[*i].f,&Z[*i].f);
        printf("%d : %f\t%f\t%f\n",*i,X[*i].f,Y[*i].f,Z[*i].f);
    }
}


int main() {
    Float_Bit *X = malloc(sizeof(Float_Bit) * SIZE);
    Float_Bit *Y = malloc(sizeof(Float_Bit) * SIZE);
    Float_Bit *Z = malloc(sizeof(Float_Bit) * SIZE);
    if ((X == NULL) || (Y == NULL)){
        return 1;
    }
    int size = -1;
    FILE* fichier = NULL;
    if((fichier = fopen("/home/corentinberge/Documents/L3_EEA/Tech_Sci/Genetique_Project/Doc/A_Genetique_Profil.txt","r")) == NULL){
        printf("Impossible d'ouvrir le fichier !\n");
        return 2;
    }
    lecture(fichier,X,Y,Z,&size);
    /*for(int j=0;j<size;j++){
        printf("%f %f %f\n",X[j].f,Y[j].f,Z[j].f);
    }*/
    fclose(fichier);
    free(X);
    free(Y);
    return 0;
}
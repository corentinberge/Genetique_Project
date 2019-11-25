#include <stdio.h>
#include <malloc.h>

#define SIZE 2000


//Change float on bit
void float_Bit(){

}


//Change bit on float
void bit_Float(){

}


//Read a file and store data
void lecture(FILE* f,float* X, float* Y,int* i){
    while(!feof(f)){
        *i = *i + 1;
        if (*i%SIZE == 0){
            X = realloc(X,((SIZE) + (*i)) * sizeof(float));
            Y = realloc(Y,((SIZE) + (*i)) * sizeof(float));
        }
        fscanf(f,"%f %f %f",&X[*i],&Y[*i]);
        printf("%d : %f \t%f\n",*i,X[*i],Y[*i]);
    }
}


int main() {
    float *X = malloc(sizeof(float)*SIZE);
    float *Y = malloc(sizeof(float)*SIZE);
    if ((X == NULL) || (Y == NULL)){
        return 1;
    }
    int size = -1;
    FILE* fichier = NULL;
    if((fichier = fopen("C:\\Users\\coren\\Documents\\L3 EEA\\Tech sci\\Genetique_Project\\Doc\\A_Genetique_Profil.txt","r")) == NULL){
        printf("Impossible d'ouvrir le fichier !\n");
        return 2;
    }
    lecture(fichier,X,Y,&size);
    /*for(int j=0;j<size;j++){
        printf("%f %f\n",X[j],Y[j]);
    }*/
    fclose(fichier);
    free(X);
    free(Y);
    return 0;
}
#include <stdio.h>
#include <malloc.h>
#include <math.h>

#define SIZE 2000
#define SIZE_B 20


//Change float on bit
void float_to_Bit(float F, int* B){
    int tmp_f = F*pow(10,6);

    printf("Y : %d\n",tmp_f);

    for(int i = 0;i<SIZE_B;i++){
        B[i] = tmp_f%2;
        tmp_f = tmp_f/2;
    }
}


//Change bit on float
void bit_to_Float(float* F, int* B){
    int tmp_b = 0;
    for (int i = 0;i<SIZE_B;i++){
        tmp_b = tmp_b + B[i]*pow(2,i);
    }
    *F = (float)tmp_b*pow(10,-6);
}


//Read a file and store data
void lecture(FILE* f, float* X, float* Y, float* Z, int* i){
    while(!feof(f)){
        *i = *i + 1;
        if (*i%SIZE == 0){
            X = realloc(X,((SIZE) + (*i)) * sizeof(float));
            Y = realloc(Y,((SIZE) + (*i)) * sizeof(float));
        }
        fscanf(f,"%f %f %f",&X[*i],&Y[*i],&Z[*i]);
        //printf("%d : %f\t%f\t%f\n",*i,X[*i].f,Y[*i].f,Z[*i].f);
    }
}


int main() {
    float *X = malloc(sizeof(float) * SIZE);
    float *Y = malloc(sizeof(float) * SIZE);
    float *Z = malloc(sizeof(float) * SIZE);
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

    int test[SIZE_B];
    float tmp;
    float_to_Bit(Y[0],test);

    bit_to_Float(&tmp,test);
    printf("test : %f\n",tmp);


    fclose(fichier);
    free(X);
    free(Y);
    free(Z);
    return 0;
}
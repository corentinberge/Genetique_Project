#include <stdio.h>
#include <malloc.h>
#include <math.h>

#define SIZE 2000
#define SIZE_M 23

//Define a type to have all the informations in one type
typedef union{
    float f;
    struct {
        int s; //signe : 1 seul bit de signe
        int e[8]; //exposant : 8 bits pour coder l'exposant
        int m[23]; //mantisse : 23 bits pour coder la mantisse
    } Bit;
} Float_Bit;


//Change float on bit
void float_to_Bit(Float_Bit F,Float_Bit* B){
    float tmp_f = fabs(F.f);
    int e = (int)tmp_f,i=0;
    B->Bit.s = (tmp_f != F.f);

    printf("%f\t%d",tmp_f,e);
    tmp_f = tmp_f-(float)e;
    while((e/2 != 0) || (i<8)){
        e = e/2;
        B->Bit.e[i] = e%2;
        printf("%d",B->Bit.e[i]);
        i++;
    }

    i=0;
    while(i<23){
        tmp_f = tmp_f*2;
        if(tmp_f > 1){
            B->Bit.m[i] = 1;
            tmp_f = tmp_f - 1.;
        }
        else {
            B->Bit.m[i] = 0;
        }
        i++;
    }
}


//Change bit on float
void bit_to_Float(Float_Bit B){
    float m = 0;
    int e = 0;
    for(int i = -SIZE_M;i<0;i++){
        m = m + B.Bit.m[i+(SIZE_M)]*pow(2.,i);
    }
    for (int i = 0;i<8;i++){
        e = e + B.Bit.e[i]*pow(2.,i);
    }
    B.f = e + m;
    if (B.Bit.s == 1){
        B.f = -B.f;
    }
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
        //printf("%d : %f\t%f\t%f\n",*i,X[*i].f,Y[*i].f,Z[*i].f);
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

    Float_Bit test;
    float_to_Bit(Y[0],&test);
    /*for (int i=0;i<size;i++){
        float_to_Bit(X[i]);
    }*/
    printf("\n\n%f\n%d\t",Y[0].f,test.Bit.s);
    for (int i=0;i<8;i++){
        printf("%d",test.Bit.e[i]);
    }
    printf("\t");
    for (int i=0;i<23;i++){
        printf("%d",test.Bit.m[i]);
    }
    bit_to_Float(test);
    printf("\n%f\n",test.f);
    fclose(fichier);
    free(X);
    free(Y);
    free(Z);
    return 0;
}
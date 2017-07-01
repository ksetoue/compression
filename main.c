#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int doesFileExist(const char *filename) {
    struct stat st;
    int result = stat(filename, &st);
    return result == 0;
}

void compress(int **matrix, int row, int col){

    int i,j, cont;
    
    FILE *comp = NULL;
    comp = fopen("compressed.pgm", "w+");
    if (comp == NULL)
    {
        printf("Error creating compressed.txt file!\n");
        exit(1);
    }

    printf("\nIniciando compressao do arquivo...");

    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){
            if(j != 0){
                fprintf(comp, " ");
            }
            if (((j+3) >= col) || ((j+2) >= col) || ((j+1) >= col)) {
                    fprintf(comp, "%d ",matrix[i][j]);
            }
            else {
                if (matrix[i][j] != matrix[i][j+1]) {
                    fprintf(comp, "%d ",matrix[i][j]);
                }
                else {
                    if (matrix[i][j] != matrix[i][j+2]) {
                        fprintf(comp, "%d ",matrix[i][j]);
                    }
                    else {
                        if (matrix[i][j] != matrix[i][j+3]) {
                            fprintf(comp, "%d ",matrix[i][j]);
                        }
                        else {
                            fprintf(comp, "@ ");
                            fprintf(comp, "%d ",matrix[i][j]);
                            cont = 1;
                            while ((cont + j) < col) {
                                if (matrix[i][j] == matrix[i][j + cont]) {
                                    cont++;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            fprintf(comp, "%d ", cont);
                            j += cont - 1;
                        }
                    }
                }
            }
        }
        fprintf(comp, "\n");  
    }
    printf("Ok.");

    for(int i=0; i < row; i++) {
        free(matrix[i]); 
    }
    free(matrix);

    fclose(comp);
}

void decompress(int gray, int height, int length){

    FILE *com = NULL;
    FILE *decomp = NULL;

    int c, times;
    int charact;
    int **m, i, j;
    char aux[3]; 

    printf("\nCriando arquivo para a descompressao...");
    decomp = fopen("decompressed.pgm", "w+");
    if (decomp == NULL)
    {
        printf("Error creating decompressed.pgm file!\n");
        exit(1);
    }
    printf("Ok.\n");

    c = doesFileExist("compressed.pgm");
    printf("\nAbrindo arquivo comprimido...");
    if(c == 0){
        fprintf(stdout, "Falha ao ler o arquivo."); fflush(stdout);
        exit(1);
    }
    com = fopen("compressed.pgm", "r");
    fprintf(stdout, "Ok.\n"); 

    printf("\nIniciando processo de descompressao...");
    fprintf(decomp, "%s\n%d %d\n%d\n", "P2", length, height, gray);
    for (int i = 0; i < height; i++){
        for (int j=0; j < length; j++) {
            fscanf(com,"%s", &aux);
            
            if (aux[0] == '@') {
                fscanf(com, "%d", &charact);
                fscanf(com, "%d", &times);
                for (int k = j; k < (j + times); k++) {
                    fprintf(decomp, "%d ", charact);
                }
                j += times - 1;                
            }
            else {
                fprintf(decomp, "%s ",aux);
            }
        }
        fprintf(decomp, "\n");
    }
    printf("Ok.");

    fclose(decomp);
    fclose(com);
}

int main(int argc, char *argv[]){

    FILE *f = NULL; 

    char type;
    int length, height, gray, c;
    int **matrix, i, j; 

    // Leitura do arquivo
    fflush(stderr);
    fprintf(stdout, "\nCarregando arquivo %s ...", argv[1]); fflush(stdout);

    c = doesFileExist(argv[0]);

    if(c == 0){
        fprintf(stdout, "Falha ao ler o arquivo."); fflush(stdout);
        exit(1);
    }
    f = fopen(argv[1], "r"); 
    fprintf(stdout, "Ok.\n"); fflush(stdout);

    fscanf(f,"%s",&type);
    fscanf(f,"%d",&length);
    fscanf(f,"%d",&height);
    fscanf(f,"%d",&gray);

    printf("\nLinhas = %d",height);
    printf("\nColunas = %d",length);
    printf("\nEscala de cinza = %d",gray);
    
    matrix = malloc(height * sizeof(int *));

    for (int i=0; i < height; i++) {
        matrix[i] = malloc(length * sizeof(int));        
    }
    printf("\nAlocando matriz...");

    for(i = 0; i < height; i++){
        for(j = 0; j < length; j++){
            if (!fscanf(f, "%d", &matrix[i][j])) 
            break;
        }

    }
    printf("Ok.");
    fclose(f);

    compress(matrix, height, length);
    decompress(gray, height, length);
    
    return 0;
}
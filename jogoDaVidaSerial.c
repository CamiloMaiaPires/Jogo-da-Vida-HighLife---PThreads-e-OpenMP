#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void aloca_matriz(int l, int c, int*** grid, int*** newgrid){
  
  int i, j;
  
  *grid = malloc(l * sizeof(int*));
  *newgrid = malloc(l * sizeof(int*));
  for(i=0; i<l; i++){
    (*grid)[i] = malloc(c * sizeof(int));
    (*newgrid)[i] = malloc(c * sizeof(int));
  }

  //zerando os valores
  for(i=0;i<l;i++){
    for(j=0;j<c;j++){
      (*grid)[i][j] = 0;
      (*newgrid)[i][j] = 0;
    }
  }
}

int getNeighbors(int** grid, int i, int j, int linhas, int colunas) {

  int vivos = 0;
  
  for(int y=i-1; y<=i+1; y++){
    for(int x=j-1; x<=j+1; x++){
      
      if(y == -1 && x == -1){
        //printf("%d", grid[linhas-1][colunas-1]);
        if (grid[linhas-1][colunas-1] == 1) { vivos++;}
      } else if(y == -1 && x >= 0 && x <= colunas-1){
        if (grid[linhas-1][x] == 1) { vivos++;}
        //printf("%d", grid[linhas-1][x]);
      } else if(y == -1 && x > colunas-1){
        if (grid[linhas-1][0] == 1) { vivos++;}
        //printf("%d", grid[linhas-1][0]);
      } else if(x == -1 && y>=0 && y<=linhas-1){
        if (grid[y][colunas-1] == 1) { vivos++;}
        //printf("%d", grid[y][colunas-1]);
      } else if(x == -1 && y > linhas-1){
        if (grid[0][colunas-1] == 1) { vivos++;}
        //printf("%d", grid[0][colunas-1]);
      } else if(y > linhas-1 && x >=0 && x <= colunas-1){
        if (grid[0][x] == 1) { vivos++;}
        //printf("%d", grid[0][x]);
      } else if(y > linhas-1 && x > colunas-1){
        if (grid[0][0] == 1) { vivos++;}
        //printf("%d", grid[0][0]);
      } else if(x > colunas-1 && y >=0 && y <= linhas-1){
        if (grid[y][0] == 1) { vivos++;}
        //printf("%d", grid[y][0]);
      } else{
        if (grid[y][x] == 1) { vivos++;}
        //printf("%d", grid[y][x]);
      }
      
    }
    //printf("\n");
  }
  if(grid[i][j] == 1){ vivos--;}
  return vivos;
}

int main(void) {

  int i, j;
  int linhas = 5;
  int colunas = 10;
  int geracoes = 10;

  int **grid, **newgrid;
  aloca_matriz(linhas, colunas, &grid, &newgrid);

  //GLIDER
  int lin = 1, col = 1;
  grid[lin  ][col+1] = 1;
  grid[lin+1][col+2] = 1;
  grid[lin+2][col  ] = 1;
  grid[lin+2][col+1] = 1;
  grid[lin+2][col+2] = 1;

  
  
  for(i=0; i<linhas; i++){
    for(j=0; j<colunas; j++){
      printf("%d", grid[i][j]);
    }
    printf("\n");
  }
  
}

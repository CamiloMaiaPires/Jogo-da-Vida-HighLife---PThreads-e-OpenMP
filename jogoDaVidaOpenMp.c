#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <omp.h>

void aloca_matriz(int l, int c, int*** grid, int*** newgrid){
  
  int i, j;
  
  *grid = malloc(l * sizeof(int*));
  *newgrid = malloc(l * sizeof(int*));
  #pragma omp parallel private(i,j) shared(grid,newgrid, c, l)
  {
  #pragma omp for
  for(i=0; i<l; i++){
    (*grid)[i] = malloc(c * sizeof(int));
    (*newgrid)[i] = malloc(c * sizeof(int));
  }

  //zerando os valores
  #pragma omp for collapse(2)
  for(i=0;i<l;i++){
    for(j=0;j<c;j++){
      (*grid)[i][j] = 0;
      (*newgrid)[i][j] = 0;
    }
  }
  }
}

int getNeighbors(int** grid, int i, int j, int linhas, int colunas) {

  int vivos = 0,x,y;
  #pragma omp parallel private(x,y) shared(grid, linhas,colunas)
  {
  #pragma omp for collapse(2)
  for(y=i-1; y<=i+1; y++){
    for(x=j-1; x<=j+1; x++){
      
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
  }
  }
  if(grid[i][j] == 1){ vivos--;}
  return vivos;
}

void nova_geracao(int*** grid, int*** newgrid, int linhas, int colunas){
  int vivos,i,j;
  #pragma omp parallel private(i,j) shared(grid,newgrid, linhas,colunas)
  {
  #pragma omp for collapse(2)
  for(i=0;i<linhas;i++){
    for(j=0;j<colunas;j++){
      vivos = getNeighbors((*grid), i, j, linhas, colunas);
      if((*grid)[i][j] == 1 && vivos == 2 || vivos == 3){
        (*newgrid)[i][j] = 1;
      } else if((*grid)[i][j] == 0 && vivos == 3){
        (*newgrid)[i][j] = 1;
      } else{
        (*newgrid)[i][j] = 0;
      }
    }
  }
  #pragma omp for
  for(i=0;i<linhas;i++){
    for(int j=0;j<colunas;j++){
      (*grid)[i][j] = (*newgrid)[i][j];
    }
  }
  }
}

int soma_celulas(int*** grid, int linhas, int colunas){
  int soma = 0,i,j;
  #pragma omp parallel private(i,j) shared(grid,linhas,colunas) reduction(+:soma)
  {
  #pragma omp for collapse(2)
  for(i=0;i<linhas;i++){
    for(j=0;j<colunas;j++){
      soma += (*grid)[i][j];
    }
  }
  }
  return soma;
}

int main(void) {

  //int i, j;
  double start, end;
  start = omp_get_wtime();
  int linhas = 2048;
  int colunas = 2048;
  int geracoes = 2000;

  int **grid, **newgrid;
  aloca_matriz(linhas, colunas, &grid, &newgrid);

  //GLIDER
  int lin = 1, col = 1;
  grid[lin  ][col+1] = 1;
  grid[lin+1][col+2] = 1;
  grid[lin+2][col  ] = 1;
  grid[lin+2][col+1] = 1;
  grid[lin+2][col+2] = 1;

  //R-pentomino
  lin =10; col = 30;
  grid[lin  ][col+1] = 1;
  grid[lin  ][col+2] = 1;
  grid[lin+1][col  ] = 1;
  grid[lin+1][col+1] = 1;
  grid[lin+2][col+1] = 1;



  /* for(i=0; i<linhas; i++){
      for(j=0; j<colunas; j++){
        printf("%d", grid[i][j]);
      }
      printf("\n");
    }
    printf("\n"); */
  for(int k=0; k<geracoes; k++){
    nova_geracao(&grid, &newgrid, linhas, colunas);
  }
  
  int total = soma_celulas(&grid, linhas, colunas);
  printf("%d", total);
  end = omp_get_wtime();
  printf(" took %f seconds.\n", end-start);
  
  
}

#include<stdio.h>
#include<stdlib.h>

//helper methods for the algorithm
double** tpose(double** train,int col, int row);
double** multiply(double**mat,int row,int col,double**mat1,int row1,int col1);
double** gauss(double**mat,int col);


int main(int argc, char** argv){

  int col;
  int row;

  FILE *fp;
  fp =fopen(argv[1],"r");

  if(fp == NULL)return 0;

  fscanf(fp,"%d \n",&col);
  fscanf(fp,"%d \n",&row);

  col += 1;

  /*creating 2d matrix skeleton for trining data set(N x (K+1)) and prize((k+1) x 1)
   *and allocats space for the rows of both matrix
   */
  double **train;
  train = (double**)malloc(row*sizeof(double*));
  double **prize;
  prize = (double**)malloc(row*sizeof(double*));
  double read;

  //allocats space for colums of the trainind data set and prize
  for(int i=0;i<row;i++){

      train[i] = (double*)malloc(col*sizeof(double));
      prize[i] = (double*)malloc(1*sizeof(double));

  }

  //reads in data from file and populates the skeletons of matrix
  for(int i=0;i<row;i++){

      train[i][0]=1;
      for(int j = 1; j<col+1; j++){
          if(j == col){

              fscanf(fp,"%lf,",&prize[i][0]);

          }else{

               fscanf(fp,"%lf,",&train[i][j]);

          }

      }
      fscanf(fp,"\n");

  }

  fclose(fp);

  FILE *fg;
  fg = fopen(argv[2],"r");

  if(fg ==NULL)return 0;

  int rowt;
  fscanf(fg,"%d\n",&rowt);

  /*creating 2d matrix skeleton for test data set
   *and allocats space for the rows & columns of the matrix
   */
  double **test;
  test = (double**)malloc(rowt*sizeof(double*));


  for(int i=0;i<rowt;i++){

      test[i] = (double*)malloc((col)*sizeof(double));

  }


  for(int i=0;i<rowt;i++){

      test[i][0]=1;
      for(int j =1;j<(col);j++){

          fscanf(fg,"%lf,",&read);
          test[i][j]=read;

      }
      fscanf(fg,"\n");

  }
  fclose(fg);

  //find transpose of training data set
  double** transpose = tpose(train,col,row);

  //multiplies transpose and training data set
  double** mul = multiply(transpose, col, row, train,row,col);

  //inverse of training data set and transpose of it multiples together
  double** inv = gauss(mul,col);

  //multiplies inverse and transpose
  double** mulx = multiply(inv,col,col,transpose,col,row);

  //multiplies result of previous and prize to find weigths of each attributes
  double** w = multiply(mulx,col,row,prize,row,1);

  //multiples test data set with the weights to generate prizes
  double** final = multiply(test,rowt,col,w,col,1);

  for(int i =0;i<rowt;i++){

      for(int j=0;j<1;j++){

          printf("%0.0lf",final[i][j]);

      }
      printf("\n");

  }

  return 0;
}

//find transpose of a given matrix
double** tpose(double** train,int col, int row){

  double** transpose;
  transpose = (double**)malloc(col*sizeof(double*));

  for(int i=0;i<col;i++){

      transpose[i]=(double*)malloc(row*sizeof(double));

  }

  for(int i =0;i<row;i++){

    for(int j=0;j<col;j++){

        transpose[j][i] = train[i][j];

    }

  }

  return transpose;
}

//multiplies 2 matrices together
double** multiply(double**mat,int row,int col,double**mat1,int row1,int col1){

	if(col != row1){

      return NULL;

  }else {

    	double** result;
  		result = (double**)malloc(row*sizeof(double*));

  		for(int i=0;i<row;i++){

           result[i] = (double*)malloc(col1*sizeof(double));

      }

  		for(int i=0;i<row;i++){

  			   for(int j=0;j<col1;j++){

                 result[i][j]=0;
  				       for(int k=0;k<col;k++){

                        result[i][j]+= (mat[i][k]*mat1[k][j]);

                 }

  			   }

      }

      return result;
  }

}

//finds gaussian of a matrix
double** gauss(double**mat,int col){
  double** id;
	id = (double**)malloc(col*sizeof(double*));

  for(int i=0;i<col;i++){

    id[i]= (double*)malloc(col*sizeof(double));

  }

  for(int i=0;i<col;i++){

    for(int j=0;j<col;j++){

        id[i][j]=0;
        if(j==i){

            id[i][j]=1;

        }

    }
  }


  //lower traingular reduced roechlon
  double pivot,constant;
  //each column i = column
  for(int i=0;i<col;i++){
     //j = row
     for(int j=i;j<col;j++){
       //checking diagonal entries
       if(mat[j][i]!=1 && j==i){

         if(mat[j][i]!=0){

           constant= mat[j][i];
           //making the diagonal entries 1

           for(int k=0;k<col;k++){

               mat[j][k] = mat[j][k]/constant;
               id[j][k] = id[j][k]/constant;

           }

         }else{
           //printf("error"); becasue diagonal equals to zero
         }
       }else if(mat[j][i]!=0 && j!=i){//non diagonal entries

         pivot = mat[j][i]; //pivot to help change non diagonal entries to zero

         for(int l=0;l<col;l++){

             mat[j][l] = -(pivot*mat[i][l])+mat[j][l];
             id[j][l] = -(pivot*id[i][l])+id[j][l];

         }

       }

     }

   }

  //upper triangular reduced roechlon
   double pivot1,scalar;
   //represents the columns still and starts from the last position
   for(int i=(col-1);i>=0;i--){
     //j is rows and starts from the same place as i to avoide overlap
     for(int j=i;j>=0;j--){
       //making sure that the diagonals are still 1 if its already not 1
         if(mat[j][i]!=1 && i==j){

            scalar = mat[j][i];
            mat[j][i] = mat[j][i]/scalar;
            id[j][i] = id[j][i]/scalar;

         }else if(mat[j][i]!=0 && i!=j){//if not zero makes it 0

           pivot1 = mat[j][i];
           for(int k=0;k<col;k++){

               mat[j][k] = mat[j][k]-(pivot1*mat[i][k]);
               id[j][k] = id[j][k]-(pivot1*id[i][k]);

           }

         }

       }

   }

   return id;

}

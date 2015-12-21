#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Programming Assignment 1
   Robert Finn
   Systems 1 10:20

   Trong Le
*/


// Matrix Multiplication. L,M,K are dimensions, *A,*B,*C are matrices
void matMult(void *A, void *B, void *C, int size, int L, int M, int K, char type) {
    
    int i, j, k;
    char *a, *b, *c;

    if(type == 'I') {
    
        int sum;
        for (i=0; i<L; i++){
            for (j=0; j<K; j++){
                sum = 0;
                for (k=0; k<M; k++){
                    a = (char *)A + (i*size)*M + (k*size);
                    b = (char *)B + (k*size)*K + (j*size);
                    sum += *(int*)a * *(int*)b;
                }
                c = (char *)C + (i*size)*K + (j*size);
                memcpy(c, &sum, size);
            }
        }

    } else if(type == 'S'){
        
        short sum;
        for (i=0; i<L; i++){
            for (j=0; j<K; j++){
                sum = 0;
                for (k=0; k<M; k++){
                    a = (char *)A + (i*size)*M + (k*size);
                    b = (char *)B + (k*size)*K + (j*size);
                    sum += *(short*)a * *(short*)b;
                }
                c = (char *)C + (i*size)*K + (j*size);
                memcpy(c, &sum, size);   
            }
        }
    } else if(type == 'D'){

        double sum;
        for (i=0; i<L; i++){
            for (j=0; j<K; j++){
                sum = 0;
                for (k=0; k<M; k++){
                    a = (char *)A + (i*size)*M + (k*size);
                    b = (char *)B + (k*size)*K + (j*size);
                    sum += *(double*)a * *(double*)b;
                }
                c = (char *)C + (i*size)*K + (j*size);
                memcpy(c, &sum, size);
            }
        }
    }
}

/* Solve matrix multiplications and back substitution with A, Y, X
   with matrix dimensions dim type of size and value type */
void solveAYX(void *A, void *Y, void *X, int dim, int size, char type) {
    
    double ident[dim][dim], temp_a[dim][dim], temp_y[dim][dim], a_hat[dim][dim], y_hat[dim], x_hat[dim];
    int i,j;

    // Create identity matrix
    for (i=0; i<dim; i++) {
        for (j=0; j<dim; j++) {
            if (i==j) {
                ident[i][j] = 1;
            } else {
                ident[i][j] = 0;
            }
        }
    }

    // Cast each element of A to a double
    for (i=0; i<dim; i++) {
        for (j=0; j<dim; j++) {
            switch (type) {
                case 'D':
                    a_hat[i][j] = *((double *)A + (i*dim) + j);
                    break;
                case 'S':
                    a_hat[i][j] = (double) *((short *)A + (i*dim) + j);
                    break;
                case 'I':
                    a_hat[i][j] = (double) *((int *)A + (i*dim) + j);
                    break;
                case 'F':
                    a_hat[i][j] = (double) *((float *)A + (i*dim) + j);
                    break;
            }
        }
    }

    // Cast each Y element to double
    for (i=0; i<dim; i++) {
        switch (type) {
            case 'D':
                y_hat[i] = *((double *)Y + i);
                break;
            case 'S':
                y_hat[i] = (double) *((short *)Y + i);
                break;
            case 'I':
                y_hat[i] = (double) *((int *)Y + i);
                break;
            case 'F':
                y_hat[i] = (double) *((float *)Y + i);
                break;
        }
    }
    
    // elimination
    for (i=0; i<dim; i++) {
        ident[i][i] /= a_hat[i][i];

        matMult(ident, a_hat, temp_a, sizeof(double), dim, dim, dim, 'D');
        memcpy(a_hat, temp_a, dim*dim*sizeof(double));

        matMult(ident, y_hat, temp_y, sizeof(double), dim, dim, 1, 'D');
        memcpy(y_hat, temp_y, dim*sizeof(double));

        ident[i][i] = 1;

        for (j=i+1; j<dim; j++) {
            ident[j][i] = -a_hat[j][i];

            matMult(ident, a_hat, temp_a, sizeof(double), dim, dim, dim, 'D');
            memcpy(a_hat, temp_a, dim*dim*sizeof(double));

            matMult(ident, y_hat, temp_y, sizeof(double), dim, dim, 1, 'D');
            memcpy(y_hat, temp_y, dim*sizeof(double));

            ident[j][i] = 0;
        }
    }
    
    // Back substitution
    x_hat[dim-1] = y_hat[dim-1];

    for (i=N-2; i>=0; i--) {
        x_hat[i] = y_hat[i];
        
        for (j=i+1; j<N; j++){
            x_hat[i] -= x_hat[j]*a_hat[i][j];
        }
        x_hat[i] /= a_hat[i][i];
    }
    

    // Cast each x element to double
    for (i=0; i<dim; i++) {
        switch (type) {
            case 'D':
                *((double *)X + i) = x_hat[i];
                break;
            case 'S':
                *((short *)X + i) = (short) x_hat[i];
                break;
            case 'I':
                *((int *)X + i) = (int) x_hat[i];
                break;
            case 'F':
                *((float *)X + i) = (float) x_hat[i];
                break;
        }
    }

}


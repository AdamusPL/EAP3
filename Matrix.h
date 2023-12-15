//
// Created by adamc on 12/10/2023.
//

#ifndef PEA1_MATRIX_H
#define PEA1_MATRIX_H
#include <iostream>

class Matrix {
public:
    int** adjMatrix; //adjacency matrix

    int nrV; //number of verticals of graph

    Matrix();
    Matrix(int); //constructor
    ~Matrix(); //destructor
    void printMatrix(); //print the matrix
};


#endif //PEA1_MATRIX_H
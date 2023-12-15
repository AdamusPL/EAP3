//
// Created by adamc on 12/10/2023.
//

#include "Matrix.h"

Matrix::Matrix(){

}

Matrix::Matrix(int nrV) {

    adjMatrix = new int *[nrV]; //array of pointers on int

    for (int i = 0; i < nrV; ++i) {
        adjMatrix[i] = new int[nrV]; //reserve memory for columns
    }

    this->nrV = nrV;

}

Matrix::~Matrix(){
    for (int i = 0; i < nrV; ++i) {
        delete [] adjMatrix[i]; //free memory from rows
    }

    delete [] adjMatrix; //free memory of array with pointers
}

void Matrix::printMatrix() {
    for (int i = 0; i < nrV; ++i) {
        for (int j = 0; j < nrV; ++j) {
            std::cout << adjMatrix[i][j] << " ";
        }
        std::cout<<std::endl;
    }
}
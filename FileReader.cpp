//
// Created by adamc on 29.11.2023.
//

#include "FileReader.h"
#include <fstream>

Matrix* FileReader::read(){

    std::cout << "Give the name of file" << std::endl;
    std::cin >> filename; //Give the name of file by user

    std::fstream file;

    std::string info;
    int number;

    file.open(filename, std::ios::in);

    if (file.is_open()) { //if the file is open

        for (int i = 0; i < 8; ++i) {

            if(i==3){ //read 4. line of the file, there's a size of matrix
                file >> info;
                file >> number;
            }

            else {
                std::getline(file, info);
            }
        }

        Matrix *matrix = new Matrix(number); //create an object of Matrix

        for (int i = 0; i < number; i++) {
            for (int j = 0; j < number; ++j) {
                if (file.fail()) {
                    std::cout << "Error while reading file" << std::endl; //"exception"
                    break;
                }

                else {
                    file >> matrix->adjMatrix[i][j]; //writing data to matrix
                }
            }
        }

        file.close();
        return matrix;
    }

    else {
        std::cout << "Error while opening file" << std::endl; //"exception"
    }

    file.close();
    return nullptr;



}
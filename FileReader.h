//
// Created by adamc on 29.11.2023.
//

#ifndef PEA2_FILEREADER_H
#define PEA2_FILEREADER_H


#include "Matrix.h"

class FileReader {
public:
    Matrix* read();
    std::string filename;
};


#endif //PEA2_FILEREADER_H
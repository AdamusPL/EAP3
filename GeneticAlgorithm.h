//
// Created by adamc on 02.01.2024.
//

#ifndef PEA3_GENETICALGORITHM_H
#define PEA3_GENETICALGORITHM_H


#include "Timer.h"
#include "Matrix.h"
#include <vector>
#include <list>
#include <bits/stdc++.h>

class GeneticAlgorithm {
public:
    GeneticAlgorithm(int stopCriteria, Matrix*, Timer, double mutationRate, int mutationMethod,
                     int initialPopulationSize, double crossoverRate, int crossoverMethod,
                     bool tests);

    bool tests;
    Matrix* matrix;
    Timer timer;
    std::vector<int> bestSolution;
    int bestObjectiveFunction;
    double whenFound;

    std::vector<std::pair<int, std::vector<int>>> population; //pair: fitness function + path
    std::vector<std::pair<int, std::vector<int>>> children; //pair: fitness function + path

    int stopCriteria;
    int initialPopulationSize;

    double mutationRate;
    int mutationMethod;


    double crossoverRate;
    int crossoverMethod;

    //variables for submethods
    //inversion
    int randomTownIndex1;
    int randomTownIndex2;
    int newObjectiveFunction;
    //stack to reverse numbers in inversionMutation
    std::stack<int> substring;

    //PMX
    std::vector<int> child1;
    std::vector<int> child2;
    //inserting towns not making conflicts with parents
//    bool foundInPath1;
//    bool foundInPath2;
    //vectors with towns which are making conflicts (we use them later)
//    std::queue<int> leftTowns1;
//    std::queue<int> leftTowns2;

    int firstCrossBreadingPoint;
    int secondCrossBreadingPoint;

//    void PMX(std::vector<int>, std::vector<int>);
    std::vector<int> OX(std::vector<int>, std::vector<int>);

    void launch();
//    void insertWithoutConflicts(std::vector<int>&, std::vector<int>&, std::vector<int>&, std::queue<int>&);
//    void insertLeftTowns(std::vector<int>&, std::vector<int>&, std::queue<int>&);
    void transpositionMutation(std::vector<int>&);
    void inversionMutation(std::vector<int>&);
    void isNewBestSolution(std::vector<int>&);

    std::vector<int> generateBegSolutionRandom();
    std::list<std::pair<double,int>> save;
    int calculateRoute(std::vector<int> track);
    void printSolution();

};


#endif //PEA3_GENETICALGORITHM_H

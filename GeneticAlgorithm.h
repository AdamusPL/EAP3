//
// Created by adamc on 02.01.2024.
//

#ifndef PEA3_GENETICALGORITHM_H
#define PEA3_GENETICALGORITHM_H


#include "Timer.h"
#include "Matrix.h"
#include <vector>
#include <list>

class GeneticAlgorithm {
public:
    GeneticAlgorithm(int stopCriteria, Matrix*);

    Matrix* matrix;
    std::vector<int> track1;
    std::vector<int> track2;
    int objectiveFunction1;
    int objectiveFunction2;
    std::vector<int> bestSolution;
    int bestObjectiveFunction;
    double whenFound;

    int stopCriteria;
    int initialPopulationSize;
    double mutationRate;
    double crossBreadingRate;
    double crossBreadingCoefficient;

    void PMX();

    std::vector<int> generateBegSolutionRandom();
    int calculateRoute(std::vector<int> track);
    void launch(Timer timer);
};


#endif //PEA3_GENETICALGORITHM_H

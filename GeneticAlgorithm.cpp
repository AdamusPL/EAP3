//
// Created by adamc on 02.01.2024.
//

#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(int stopCriteria, Matrix *matrix, Timer timer, double mutationRate,
                                   int mutationMethod,
                                   int initialPopulationSize, double crossoverRate, int crossoverMethod, bool tests) {
    this->stopCriteria = stopCriteria;
    this->matrix = matrix;
    this->timer = timer;
    this->mutationRate = mutationRate;
    this->mutationMethod = mutationMethod;
    this->initialPopulationSize = initialPopulationSize;
    this->crossoverRate = crossoverRate;
    this->crossoverMethod = crossoverMethod;
    this->tests = tests;
}

std::vector<int> GeneticAlgorithm::generateBegSolutionRandom() {
    std::vector<int> track;
    std::vector<std::pair<bool, int>> visited; //vector to help to decide

    for (int i = 0; i < matrix->nrV; ++i) { //filling the vector
        visited.push_back(std::make_pair(false, i));
    }

    int x;

    for (int i = 0; i < matrix->nrV; ++i) {
        x = rand() % (matrix->nrV); //get random number

        while (visited[x].first) { //if it exists in track1, generate once again
            x = rand() % (matrix->nrV);
        }

        track.push_back(x);
        visited[x].first = true; //we visit every node once

    }

//    for (int i = 0; i < track.size(); ++i) {
//        std::cout << track[i] << "->";
//    }
//
//    std::cout << track[0];
//    std::cout<<std::endl;
    return track;
}

void checkPath(std::vector<int> track1){
    std::vector<bool> visited;

    for(int i=0; i<track1.size(); i++){
        visited.push_back(false);
    }

    for(int i=0; i<track1.size(); i++){
        if(visited[track1[i]]){
            std::cout << "Error!" << std::endl;
            return;
        }
        else{
            visited[track1[i]] = true;
        }
    }
}

std::vector<int> GeneticAlgorithm::OX(std::vector<int> track1, std::vector<int> track2){

    std::vector<int> child;

    //1. two random cross-breading points [x1,x2]
    firstCrossBreadingPoint = rand() % (track1.size() - 1);
    secondCrossBreadingPoint = firstCrossBreadingPoint + 1 + rand() % (track1.size() - firstCrossBreadingPoint -
                                                                       1);
    child.assign(track1.size(), -1);

    std::vector<bool> visited;
    visited.assign(track1.size(), false);

    for(int i=firstCrossBreadingPoint; i<=secondCrossBreadingPoint; i++) {
        child[i] = track1[i];
        visited[track1[i]] = true;
    }

    for(int i=secondCrossBreadingPoint+1; i<track1.size(); i++){
        if(!visited[track2[i]]){
            child[i] = track2[i];
        }
    }

    for(int i=0; i<secondCrossBreadingPoint; i++){
        if(!visited[track2[i]]){
            child[i] = track2[i];
        }
    }

    return child;


}

//Partially Matched Crossover
void GeneticAlgorithm::PMX(std::vector<int> track1, std::vector<int> track2) {

    //1. two random cross-breading points [x1,x2]
    firstCrossBreadingPoint = rand() % (track1.size() - 1);
    secondCrossBreadingPoint = firstCrossBreadingPoint + 1 + rand() % (track1.size() - firstCrossBreadingPoint -
                                                                1);

    //fill the children with "-1"
    child1.assign(track1.size(), -1);
    child2.assign(track1.size(), -1);

    //2. swap sections
    for (int i = firstCrossBreadingPoint; i <= secondCrossBreadingPoint; ++i) {
        child1[i] = track2[i];
        child2[i] = track1[i];
    }

    //3. insert towns from parents which don't make conflicts
    GeneticAlgorithm::insertWithoutConflicts(track1, child1, child2, leftTowns1);
    GeneticAlgorithm::insertWithoutConflicts(track2, child2, child1, leftTowns2);

    //4. left towns: insert going from left side, watching the mapping table
    while(!leftTowns1.empty()){
        for (int j = 0; j < track1.size(); ++j) {
            if (child1[j] == -1) {
                child1[j] = leftTowns1.front();
                leftTowns1.pop();
                break;
            }
        }
    }

    while(!leftTowns2.empty()){
        for (int j = 0; j < track2.size(); ++j) {
            if (child2[j] == -1) {
                child2[j] = leftTowns2.front();
                leftTowns2.pop();
                break;
            }
        }
    }


}

void GeneticAlgorithm::insertWithoutConflicts(std::vector<int> &track, std::vector<int> &child1, std::vector<int> &child2, std::queue<int> &leftTowns){

    for (int i = 0; i < track.size(); ++i) {

        foundInPath1 = false;
        foundInPath2 = false;

        for (int j = firstCrossBreadingPoint; j <= secondCrossBreadingPoint; ++j) {

            //if town exists already in new track, don't insert it
            if (track[i] == child1[j]) {
                foundInPath1 = true;
            }

            if(track[i] == child2[j]){
                foundInPath2 = true;
            }

        }

        //insert town if it doesn't exist
        if(foundInPath2 && !foundInPath1){
            leftTowns.push(track[i]);
        }

        else if(!foundInPath1 && !foundInPath2){
            child1[i] = track[i];
        }

    }
}

void GeneticAlgorithm::transpositionMutation(std::vector<int> &track) {

    //mutation
    randomTownIndex1 = rand() % (track.size()); //1. random town
    randomTownIndex2 = rand() % (track.size()); //2. random town

    while (randomTownIndex1 == randomTownIndex2) { //it has to be 2 different towns
        randomTownIndex2 = rand() % (track.size()); //2. random town
    }

    std::swap(track[randomTownIndex1], track[randomTownIndex2]); //swap

}

void GeneticAlgorithm::inversionMutation(std::vector<int> &track) {

    //mutation
    randomTownIndex1 = rand() % (track.size() - 1); //we have to have at least 2 elements to invert
    randomTownIndex2 = randomTownIndex1 + 1 + rand() % (track.size() - randomTownIndex1 -
                                                        1); //2. random town from range (randomTownIndex1; track.size())

    for (int j = randomTownIndex1; j <= randomTownIndex2; j++) {
        substring.emplace(track[j]);
    }

    for (int j = randomTownIndex1; j <= randomTownIndex2; j++) {
        track[j] = substring.top();
        substring.pop();
    }

}

void GeneticAlgorithm::launch() {
    timer.startTimer();
    srand(time(NULL)); //initialize the seed

    int n, randomParent1, randomParent2;

    //generate random population
    while(population.size() < initialPopulationSize){
        std::vector<int> solution = GeneticAlgorithm::generateBegSolutionRandom();
        bestSolution = solution;
        newObjectiveFunction = calculateRoute(solution);
        bestObjectiveFunction = newObjectiveFunction;
        population.emplace_back(newObjectiveFunction, solution);
    }


    while(timer.stopTimer() / 1000000.0 < stopCriteria) {

        //clear table for new children
        children.clear();

        //rank selection
        //1. sort
        std::sort(population.begin(), population.end());

        //number of surviving persons
        n = 0.25*initialPopulationSize;

        //2. kill not used solutions
        for (int i = 0; i < initialPopulationSize-n; i++) {
            population.pop_back();
        }

        while(population.size() + children.size() < initialPopulationSize){
            randomParent1 = rand()%population.size();
            randomParent2 = rand()%population.size();

            while(randomParent2 == randomParent1){
                randomParent2 = rand()%population.size();
            }

            //crossover
            if(static_cast<double>(rand()%100+1) / 100.0 <= crossoverRate) {
                GeneticAlgorithm::PMX(population[randomParent1].second, population[randomParent2].second);
//                child1 = GeneticAlgorithm::OX(population[randomParent1].second, population[randomParent2].second);
//                child2 = GeneticAlgorithm::OX(population[randomParent2].second, population[randomParent1].second);

                //mutations
                if (static_cast<double>(rand() % 100 + 1) / 100.0 <= mutationRate) {
                    if (mutationMethod == 1) {
                        GeneticAlgorithm::transpositionMutation(child1);
                        GeneticAlgorithm::transpositionMutation(child2);

                    } else if (mutationMethod == 2) {
                        GeneticAlgorithm::inversionMutation(child1);
                        GeneticAlgorithm::inversionMutation(child2);

                    }
                }

                children.emplace_back(calculateRoute(child1), child1);
                children.emplace_back(calculateRoute(child2), child2);

                //changing best solution
                newObjectiveFunction = calculateRoute(child1);

                if (newObjectiveFunction < bestObjectiveFunction) {
                    bestObjectiveFunction = newObjectiveFunction;
                    bestSolution = child1;
                    std::cout << bestObjectiveFunction << std::endl;
                    whenFound = timer.stopTimer() / 1000000.0;
                    if (tests) {
                        save.emplace_back(whenFound, bestObjectiveFunction);
                    }
                }

                //changing best solution
                newObjectiveFunction = calculateRoute(child2);

                if (newObjectiveFunction < bestObjectiveFunction) {
                    bestObjectiveFunction = newObjectiveFunction;
                    bestSolution = child2;
                    whenFound = timer.stopTimer() / 1000000.0;
                    std::cout << bestObjectiveFunction << std::endl;
                    if (tests) {
                        save.emplace_back(whenFound, bestObjectiveFunction);
                    }
                }

                child1.clear();
                child2.clear();

            }

            else{
                children.emplace_back(calculateRoute(population[randomParent1].second), population[randomParent1].second);
                children.emplace_back(calculateRoute(population[randomParent2].second), population[randomParent2].second);
            }

        }

        for(int i=0; i<children.size(); i++){
            population.push_back(children[i]);
        }

        //new population has been made
    }

}

int GeneticAlgorithm::calculateRoute(std::vector<int> track) {
    int objectiveFunction = 0;

    for (int i = 1; i < track.size(); ++i) {
        objectiveFunction += matrix->adjMatrix[track[i - 1]][track[i]];
    }

    objectiveFunction += matrix->adjMatrix[track[track.size() - 1]][track[0]];

    return objectiveFunction;
}

void GeneticAlgorithm::printSolution() {

    checkPath(bestSolution);

    for (int i = 0; i < bestSolution.size(); ++i) {
        std::cout << bestSolution[i] << "->";
    }

    std::cout << bestSolution[0];
    std::cout << std::endl;
    std::cout << bestObjectiveFunction << std::endl;

}
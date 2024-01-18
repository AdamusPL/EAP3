//
// Created by adamc on 02.01.2024.
//

#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(int stopCriteria, Matrix* matrix, Timer timer, double mutationRate, int mutationMethod, int initialPopulationSize, double crossoverRate, int crossoverMethod){
    this->stopCriteria=stopCriteria;
    this->matrix=matrix;
    this->timer=timer;
    this->mutationRate=mutationRate;
    this->mutationMethod=mutationMethod;
    this->initialPopulationSize=initialPopulationSize;
    this->crossoverRate=crossoverRate;
    this->crossoverMethod=crossoverMethod;
}

std::vector<int> GeneticAlgorithm::generateBegSolutionRandom(){
    std::vector<int> track;
    std::vector<std::pair<bool, int>> visited; //vector to help to decide

    for (int i = 0; i < matrix->nrV; ++i) { //filling the vector
        visited.push_back(std::make_pair(false,i));
    }

    int x;

    for (int i = 0; i < matrix->nrV; ++i) {
        x = rand()%(matrix->nrV); //get random number

        while(visited[x].first){ //if it exists in track1, generate once again
            x = rand()%(matrix->nrV);
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

//Partially Matched Crossover
void GeneticAlgorithm::PMX(std::vector<int> track1, std::vector<int> track2){

    //two random cross-breading points [x1,x2]
    int firstCrossBreadingPoint = rand()%(track1.size()-1); //we can't cross only 1 last element
    int secondCrossBreadingPoint = rand()%(track1.size());

    //x2 > x1 && we can cross at least 2 element array
    while(secondCrossBreadingPoint < firstCrossBreadingPoint && secondCrossBreadingPoint-firstCrossBreadingPoint<2){
        secondCrossBreadingPoint = rand()%(track1.size());
    }

    //two children
    std::vector<int> child1;
    std::vector<int> child2;

    //fill the children with "-1"
    for(int i=0; i<track1.size(); i++){
        child1.push_back(-1);
        child2.push_back(-1);
    }
    //swap
    for (int i = firstCrossBreadingPoint; i < secondCrossBreadingPoint; ++i) {
        child1[i]=track2[i];
        child2[i]=track1[i];
    }

    //inserting towns not making conflicts with parents
    bool shouldInsert1;
    bool shouldInsert2;
    //vectors with towns which are making conflicts (we use them later)
    std::vector<int> leftTowns1;
    std::vector<int> leftTowns2;

    for (int i = 0; i < track1.size(); ++i) {

        shouldInsert1 = true;
        shouldInsert2 = true;

        for (int j = firstCrossBreadingPoint; j < secondCrossBreadingPoint; ++j) {

            //if town exists already in new track, don't insert it
            if(track1[i] == child1[j]){
                shouldInsert1 = false;
            }

            if(track2[i] == child2[j]){
                shouldInsert2 = false;
            }

            if(!shouldInsert1 && !shouldInsert2){
                break;
            }

        }

        //insert town if it doesn't exist
        if(shouldInsert1){
            child1[i] = track1[i];
        }

        else{
            leftTowns1.push_back(track1[i]);
        }

        if(shouldInsert2){
            child2[i] = track2[i];
        }

        else{
            leftTowns2.push_back(track2[i]);
        }

    }

    //left towns: insert going from left side, watching the mapping table
    for (int i = 0; i < leftTowns1.size(); ++i) {
        for (int j = 0; j < track1.size(); ++j) {
            if(child1[j] == -1){
                child1[j] = leftTowns1[i];
                break;
            }
        }
    }

    for (int i = 0; i < leftTowns2.size(); ++i) {
        for (int j = 0; j < track2.size(); ++j) {
            if(child2[j] == -1){
                child2[j] = leftTowns2[i];
                break;
            }
        }
    }

    //mutations
    if(static_cast<double>(rand()%100+1) / 100.0 <= mutationRate) {
        if (mutationMethod == 1) {
            GeneticAlgorithm::transpositionMutation(child1);
            GeneticAlgorithm::transpositionMutation(child2);

        } else if (mutationMethod == 2) {
            GeneticAlgorithm::inversionMutation(child1);
            GeneticAlgorithm::inversionMutation(child2);

        }

        children.emplace_back(calculateRoute(child1), child1);
        children.emplace_back(calculateRoute(child2), child2);
    }

    else{
        children.emplace_back(calculateRoute(child1), child1);
        children.emplace_back(calculateRoute(child2), child2);

        //changing best solution
        int newObjectiveFunction = calculateRoute(child1);

        if(newObjectiveFunction < bestObjectiveFunction) {
            bestObjectiveFunction = newObjectiveFunction;
            bestSolution = child1;
            whenFound = timer.stopTimer() / 1000000.0;
            std::cout << bestObjectiveFunction << std::endl;
            save.emplace_back(whenFound, bestObjectiveFunction);
        }

        //changing best solution
        newObjectiveFunction = calculateRoute(child2);

        if(newObjectiveFunction < bestObjectiveFunction) {
            bestObjectiveFunction = newObjectiveFunction;
            bestSolution = child2;
            whenFound = timer.stopTimer() / 1000000.0;
            std::cout << bestObjectiveFunction << std::endl;
            save.emplace_back(whenFound, bestObjectiveFunction);
        }
    }

}

void GeneticAlgorithm::transpositionMutation(std::vector<int>& track){

    int randomTownIndex1;
    int randomTownIndex2;

    //mutation
    randomTownIndex1 = rand() % (track.size()); //1. random town
    randomTownIndex2 = rand() % (track.size()); //2. random town

    while(randomTownIndex1 == randomTownIndex2){ //it has to be 2 different towns
        randomTownIndex2 = rand() % (track.size()); //2. random town
    }

    std::swap(track[randomTownIndex1], track[randomTownIndex2]); //swap

    //changing best solution
    int newObjectiveFunction = calculateRoute(track);

    if(newObjectiveFunction < bestObjectiveFunction) {
        bestObjectiveFunction = newObjectiveFunction;
        bestSolution = track;
        whenFound = timer.stopTimer() / 1000000.0;
        std::cout << bestObjectiveFunction << std::endl;
    }

}

void GeneticAlgorithm::inversionMutation(std::vector<int>& track){

    //stack to reverse the numbers
    std::stack<int> substring;
    int randomTownIndex1;
    int randomTownIndex2;

    //mutation
    randomTownIndex1 = rand() % (track.size() - 1); //we have to have at least 2 elements to invert
    randomTownIndex2 = (rand() % track.size()) + 1; //random towns

    for(int j=randomTownIndex1; j<randomTownIndex2; j++){
        substring.emplace(track[j]);
    }

    for(int j=randomTownIndex1; j<randomTownIndex2; j++){
        track[j] = substring.top();
        substring.pop();
    }

    //changing best solution
    int newObjectiveFunction = calculateRoute(track);

    if(newObjectiveFunction < bestObjectiveFunction) {
        bestObjectiveFunction = newObjectiveFunction;
        bestSolution = track;
        whenFound = timer.stopTimer() / 1000000.0;
        std::cout << bestObjectiveFunction << std::endl;
    }

}

void GeneticAlgorithm::launch(){
    timer.startTimer();
    srand(time(NULL)); //initialize the seed

    int n, randomParent1, randomParent2;
    int j;

    //generate random population
    while(population.size() < initialPopulationSize){
        std::vector<int> solution = GeneticAlgorithm::generateBegSolutionRandom();
        bestSolution = solution;
        int objectiveFunction = calculateRoute(solution);
        bestObjectiveFunction = objectiveFunction;
        population.emplace_back(objectiveFunction, solution);
    }


    while(timer.stopTimer() / 1000000.0 < stopCriteria) {

        //clear table for new children
        children.clear();

        //rank selection
        //1. sort
        std::sort(population.begin(), population.end());

        //crossover
        while(children.size() < initialPopulationSize){
            randomParent1 = rand()%population.size();
            randomParent2 = rand()%population.size();

            while(randomParent2 == randomParent1){
                randomParent2 = rand()%population.size();
            }

            if(static_cast<double>(rand()%100+1) / 100.0 <= crossoverRate) {
                GeneticAlgorithm::PMX(population[randomParent1].second, population[randomParent2].second);
            }

        }

        std::sort(children.begin(), children.end());

        j = 0;

        for(int i=0; i < initialPopulationSize; i++){
            if(children[j].first < population[i].first){
                population[i] = children[j];
                j++;
            }
        }

        //new population has been made
    }

}

int GeneticAlgorithm::calculateRoute(std::vector<int> track){
    int objectiveFunction = 0;

    for (int i = 1; i < track.size(); ++i) {
        objectiveFunction+=matrix->adjMatrix[track[i - 1]][track[i]];
    }

    objectiveFunction+=matrix->adjMatrix[track[track.size() - 1]][track[0]];

    return objectiveFunction;
}

void GeneticAlgorithm::printSolution(){
    for (int i = 0; i < bestSolution.size(); ++i) {
        std::cout << bestSolution[i] << "->";
    }

    std::cout << bestSolution[0];
    std::cout<<std::endl;
    std::cout << bestObjectiveFunction << std::endl;

}
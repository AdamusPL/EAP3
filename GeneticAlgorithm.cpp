//
// Created by adamc on 02.01.2024.
//

#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(int stopCriteria, Matrix* matrix){
    this->stopCriteria=stopCriteria;
    this->matrix=matrix;
    objectiveFunction1 = 0;
    objectiveFunction2 = 0;
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

    for (int i = 0; i < track.size(); ++i) {
        std::cout << track[i] << "->";
    }

    std::cout << track[0];
    std::cout<<std::endl;
    return track;
}

//Partially Matched Crossover
void GeneticAlgorithm::PMX(){

    std::vector<int> newTrack1;
    std::vector<int> newTrack2;

    //two random cross-breading points [x1,x2]
    int firstCrossBreadingPoint = rand()%(track1.size()-1); //we can't cross only 1 last element
    int secondCrossBreadingPoint = rand()%(track1.size());

    //x2 > x1
    while(secondCrossBreadingPoint < firstCrossBreadingPoint){
        secondCrossBreadingPoint = rand()%(track1.size());
    }

    for (int i = 0; i < track1.size(); ++i) {
        newTrack1.push_back(-1);
        newTrack2.push_back(-1);
    }

    //swap
    for (int i = firstCrossBreadingPoint; i < secondCrossBreadingPoint; ++i) {
        newTrack1[i]=track2[i];
        newTrack2[i]=track1[i];
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
            if(track1[i] == newTrack1[i]){
                shouldInsert1 = false;
            }

            if(track2[i] == newTrack2[i]){
                shouldInsert2 = false;
            }

            if(!shouldInsert1 && !shouldInsert2){
                break;
            }

        }

        //insert town if it doesn't exist
        if(shouldInsert1){
            newTrack1[i] = track1[i];
        }

        else{
            leftTowns1.push_back(track1[i]);
        }

        if(shouldInsert2){
            newTrack2[i] = track2[i];
        }

        else{
            leftTowns2.push_back(track2[i]);
        }

    }

    //left towns: insert going from left side, watching the mapping table
    for (int i = 0; i < leftTowns1.size(); ++i) {
        for (int j = 0; j < newTrack1.size(); ++j) {
            if(newTrack1[j] == -1){
                newTrack1[j] = leftTowns1[i];
                break;
            }
        }
    }

    for (int i = 0; i < leftTowns2.size(); ++i) {
        for (int j = 0; j < newTrack2.size(); ++j) {
            if(newTrack2[j] == -1){
                newTrack2[j] = leftTowns2[i];
                break;
            }
        }
    }

    track1 = newTrack1;
    track2 = newTrack2;

}

void GeneticAlgorithm::launch(Timer timer){

    srand(time(NULL)); //initialize the seed

    //generate 2 random tracks
    track1 = GeneticAlgorithm::generateBegSolutionRandom();
    objectiveFunction1 = calculateRoute(track1);

    track2 = GeneticAlgorithm::generateBegSolutionRandom();
    objectiveFunction2 = calculateRoute(track2);

    if(objectiveFunction1<objectiveFunction2){
        bestSolution = track1;
        bestObjectiveFunction = objectiveFunction1;
        whenFound = timer.stopTimer() / 1000000.0;
    }

    else{
        bestSolution = track2;
        bestObjectiveFunction = objectiveFunction2;
        whenFound = timer.stopTimer() / 1000000.0;
    }

    while(timer.stopTimer() / 1000000.0 < stopCriteria){

        //cross-breading
        GeneticAlgorithm::PMX();
        objectiveFunction1 = calculateRoute(track1);
        objectiveFunction2 = calculateRoute(track2);

        if(objectiveFunction1<bestObjectiveFunction){
            bestSolution = track1;
            bestObjectiveFunction = objectiveFunction1;
            whenFound = timer.stopTimer() / 1000000.0;
            std::cout << bestObjectiveFunction << std::endl;
        }

        if(objectiveFunction2<bestObjectiveFunction){
            bestSolution = track2;
            bestObjectiveFunction = objectiveFunction2;
            whenFound = timer.stopTimer() / 1000000.0;
            std::cout << bestObjectiveFunction << std::endl;
        }
    }
}

int GeneticAlgorithm::calculateRoute(std::vector<int> track){
    int objectiveFunction = 0;

    for (int i = 1; i < track1.size(); ++i) {
        objectiveFunction+=matrix->adjMatrix[track[i - 1]][track[i]];
    }

    objectiveFunction+=matrix->adjMatrix[track[track.size() - 1]][track[0]];

    return objectiveFunction;
}
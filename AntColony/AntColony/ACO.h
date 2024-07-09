#pragma once

#include <cstdlib>
#include <cmath>
#include "Ant.h"

#define MIN 2
#define MAX 50
#define Matrix_Size 40
#define RHO 0.5 //pheromone vapourisation
#define ALPHA 1.0
#define BETA 2.0
#define REPEATES 20

namespace ACO {

    using vector2D = std::vector<std::vector<double>>;

    void CreateDistanceMatrix(vector2D& cities) {
        cities.resize(Matrix_Size, std::vector<double>(Matrix_Size, 0));
        for (int i = 0; i < Matrix_Size; i++) {
            for (int j = i + 1; j < Matrix_Size; j++) {
                cities[i][j] = cities[j][i] = MIN + std::rand() % (MAX - MIN + 1);
            }
        }
    }
    void UpdatePheromoneMatrix(vector2D& pheromones, Ant& ant) {
        for (int i = 0; i < Matrix_Size; i++) {
            for (int j = 0; j < Matrix_Size; j++) {
                pheromones[i][j] *= (1 - RHO);
                if (ant.PathWasTravelled(std::make_tuple(i, j)))
                    pheromones[i][j] += ant.GetTourLength(i, j);
            }
        }
    }
    double CostOfUnvisitedCities(vector2D& Distance, vector2D& Pheromone, int row) {
        double cost = 0;
        for (int j = 0; j < Matrix_Size; j++) {
            if (Distance[row][j] != 0)
                cost += pow(Pheromone[row][j], ALPHA) * pow(1 / Distance[row][j], BETA);

        }
        return cost;
    }
    void NextCityProbalityMatrix(vector2D& Probality, vector2D& Pheromone, vector2D& Distance) {
        for (int i = 0; i < Matrix_Size; i++) {
            for (int j = 0; j < Matrix_Size; j++) {
                if (Distance[i][j] != 0)
                    Probality[i][j] = pow(Pheromone[i][j], ALPHA) * pow(1 / Distance[i][j], BETA) / CostOfUnvisitedCities(Distance, Pheromone, i);
                else
                    Probality[i][j] = 0.;
            }

        }
    }
    int ChooseNextCity(vector2D& Probality, Ant& ant) {
        std::vector<double> probalities(Matrix_Size, 0.0);
        int current_city = ant.GetCurrentCity();
        double sum = 0.;
        for (int j = 0; j < Matrix_Size; j++) {
            if (j != current_city && ant.IsUnvisitedCity(j)) {
                probalities[j] = Probality[current_city][j];
                sum += probalities[j];
            }
        }
        if (sum > 0) {
            for (int i = 0; i < Matrix_Size; i++)
                probalities[i] /= sum; //normalization
        }
        double r = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);//[0,1]
        double cumulative_sum = 0.0;
        for (int i = 0; i < Matrix_Size; i++) {
            if (ant.IsUnvisitedCity(i) && i != current_city) { //roullete wheele
                cumulative_sum += probalities[i];
                if (r <= cumulative_sum)
                    return i;
            }

        }
        return -1;// Should not reach here
    }
}


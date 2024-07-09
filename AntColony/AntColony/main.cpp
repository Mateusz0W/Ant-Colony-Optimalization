
#include<time.h>
#include<iostream>

#include"ACO.h"


void DrawCities(sf::RenderWindow& window, std::vector<std::tuple<float, float>>& coordinates) {
	sf::CircleShape city(10);
	city.setFillColor(sf::Color::Blue);
    for (const auto cord:coordinates) {
        city.setPosition(std::get<0>(cord), std::get<1>(cord));
        window.draw(city);
    }
}
void SetCitiesPosition(std::vector<std::tuple<float,float>> & coordinates) {
    for (int i = 0; i < Matrix_Size; i++) {
        coordinates.push_back(std::make_tuple(static_cast<float>(std::rand() % 800), static_cast<float>(std::rand() % 600)));
    }
}
void DrawLines(sf::RenderWindow& window, Ant& ant, std::vector<std::tuple<float, float>>& coordinates) {
    for (const auto path : ant.GetTravelledPaths()) {
        std::tuple<float, float> start_city = coordinates[std::get<0>(path)];
        std::tuple<float, float> end_city = coordinates[std::get<1>(path)];
        float s_x = std::get<0>(start_city);
        float s_y = std::get<1>(start_city);
        float e_x = std::get<0>(end_city);
        float e_y = std::get<1>(end_city);
       
        float dx = e_x - s_x;
        float dy = e_y - s_y;
        float length = sqrt(dx * dx + dy * dy);
        float angle = atan2(dy, dx) * 180 / 3.14159265;

        sf::RectangleShape line(sf::Vector2f(length, 5.0f));
        line.setPosition(s_x, s_y);
        line.setRotation(angle);
        line.setFillColor(sf::Color::Red);

        window.draw(line);
    }

}
void print(ACO::vector2D& vec) {
    for (auto& row : vec) {
        for (auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t) {
    return start + t * (end - start);
}
void SetAntMovement(std::vector<sf::Vector2f> &points, Ant& ant,std::vector<std::tuple<float, float>>& coordinates) {
    for (const auto path : ant.GetTravelledPaths()) {
        std::tuple<float, float> city = coordinates[std::get<1>(path)];
        float x = std::get<0>(city);
        float y = std::get<1>(city);
        points.push_back(sf::Vector2f(x, y));
    }
}

int main(void) {
    std::srand(time(0));

    const unsigned int number_of_ants = 100;
    unsigned int itterations = 0;
    unsigned int number_of_repeats = 0;

    ACO::vector2D Cities;
    ACO::vector2D Pheromones(Matrix_Size, std::vector<double>(Matrix_Size, 0.1));
    ACO::vector2D Probality(Matrix_Size, std::vector<double>(Matrix_Size));
    ACO::CreateDistanceMatrix(Cities);

    std::vector<Ant> ants_colony;
    for (int i = 0; i < number_of_ants; i++)
        ants_colony.push_back(Ant(0));
    Ant OptimalPath;

    //Init for Drawing
    std::vector<std::tuple<float, float>> cities_coordinates;
    SetCitiesPosition(cities_coordinates);


    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Ant Colony Optimization Visualization");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (itterations < Matrix_Size) {
            ACO::NextCityProbalityMatrix(Probality, Pheromones, Cities);
            for (auto& ant : ants_colony) {
                int city = ACO::ChooseNextCity(Probality, ant);
                if (city != -1) {
                    ant.GoToNextCity(city);
                    ant.CalulateTourLength(Cities);
                    ant.Update();
                }
            }
        }
        if (itterations == Matrix_Size) {
            for (auto& ant : ants_colony)
                ant.SetAntMovement(cities_coordinates);
        }

        if (itterations > Matrix_Size) {
            for (auto& ant : ants_colony) {
                float elapsedTime = ant._clock.getElapsedTime().asSeconds();
                float t = elapsedTime / ant._duration;
                if (t < 1.0f) {
                    sf::Vector2f newPosition = lerp(ant.ReturnPosition(ant._currentSegment), ant.ReturnPosition(ant._currentSegment + 1), t);
                    ant.SetPosition(newPosition);
                }
                else {
                    ant._currentSegment++;
                    if (ant._currentSegment < ant.ReturnPointsSize() - 1) {
                        ant._clock.restart();
                    }
                    else {
                        int last = ant.ReturnPointsSize() - 1;
                        ant.SetPosition(ant.ReturnPosition(last));
                        ant._isFinishedAnimation = true;
                    }
                }
            }
            if (number_of_repeats != REPEATES) {
                window.clear(sf::Color::White);
                DrawCities(window, cities_coordinates);
                for (const auto& ant : ants_colony)
                    window.draw(ant._agent);
                window.display();
            }
            else {
                window.clear(sf::Color::White);
                DrawCities(window, cities_coordinates);
                DrawLines(window, OptimalPath, cities_coordinates);
                window.display();
            }
           

        }
        itterations++;
        if (ants_colony.back()._isFinishedAnimation && number_of_repeats < REPEATES) {
            itterations = 0;
            for (auto& ant : ants_colony) {
                OptimalPath = ant;
                ACO::UpdatePheromoneMatrix(Pheromones, ant);
                ant = Ant(0);
            }
            number_of_repeats++;
        }
        
        


    }
    return 0;
}
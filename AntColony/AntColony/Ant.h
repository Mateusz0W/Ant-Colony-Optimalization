#pragma once
#include<vector>
#include <tuple>
#include <SFML/Graphics.hpp>

class Ant {
public:
    using nodes = std::vector<std::tuple<int, int>>;
    using vector2D = std::vector<std::vector<double>>;
    Ant() = default;
    Ant(int start_city) :_tourLength(0.), _current_city(start_city), _next_city(start_city),_isFinishedAnimation(false) {_paths_travelled.push_back(std::make_tuple(start_city,start_city)); _agent.setRadius(5.); _agent.setFillColor(sf::Color::Black); _currentSegment = 0; _duration = 0.5f; }
    void CalulateTourLength(vector2D& matrix);
    double GetTourLength(int i, int j);
    int GetCurrentCity();
    void GoToNextCity(int& city);
    void Update();
    bool IsUnvisitedCity(int city);
    bool PathWasTravelled(std::tuple<int, int> path);
    nodes GetTravelledPaths();
    void Reset();

    //graphic part
    void SetAntMovement(std::vector<std::tuple<float, float>>& coordinates);
    void SetPosition(sf::Vector2f position);
    sf::Vector2f ReturnPosition(int index);
    int ReturnPointsSize();
    sf::CircleShape _agent;
    int _currentSegment;
    sf::Clock _clock;
    float _duration;
    bool _isFinishedAnimation;

private:
    double _tourLength;
    int _current_city;
    int _next_city;
    nodes _paths_travelled;
    
    //grapgic part
    std::vector<sf::Vector2f> _points; //movement points
   

};
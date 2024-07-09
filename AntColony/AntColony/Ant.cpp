#include "Ant.h"

void Ant::CalulateTourLength(vector2D& matrix) {
    _tourLength += matrix[_current_city][_next_city];
}
double Ant::GetTourLength(int i, int j) {
    if (_paths_travelled.size() == 0)
        return 0.;
    for (const auto el : _paths_travelled) {
        if (std::get<0>(el) == i && std::get<1>(el) == j)
            return 1 / _tourLength;
    }
    return 0.;
}
int Ant::GetCurrentCity() {
    return _current_city;
}
void Ant::GoToNextCity(int& city) {
    _next_city = city;
}
void Ant::Update() {
    _paths_travelled.push_back(std::make_tuple(_current_city, _next_city));
    _current_city = _next_city;
}
bool Ant::IsUnvisitedCity(int city) {
    if (_paths_travelled.size() == 0)
        return true;
    for (const auto visited_cities : _paths_travelled) {
        if (std::get<1>(visited_cities) == city)
            return false;
    }
    return true;
}
bool Ant::PathWasTravelled(std::tuple<int, int> path) {
    if (_paths_travelled.size() == 0)
        return false;
    for (const auto _path : _paths_travelled) {
        if (_path == path)
            return true;
    }
    return false;
}
Ant::nodes Ant::GetTravelledPaths() {
    return _paths_travelled;
}

//Graphic part

void Ant::SetAntMovement(std::vector<std::tuple<float, float>>& coordinates) {
    for (const auto path : this->GetTravelledPaths()) {
        std::tuple<float, float> city = coordinates[std::get<1>(path)];
        float x = std::get<0>(city);
        float y = std::get<1>(city);
        _points.push_back(sf::Vector2f(x, y));
    }
    _agent.setPosition(_points[0]);
}
void Ant::SetPosition(sf::Vector2f position) {
    _agent.setPosition(position);
}
sf::Vector2f Ant::ReturnPosition(int index) {
        return _points[index];
}
int Ant::ReturnPointsSize() {
    return _points.size();
}
void Ant::Reset() {
    _currentSegment = 0;
    _clock.restart();
    _isFinishedAnimation = false;
    _tourLength = 0;
    _current_city = _next_city = std::get<0>(_paths_travelled[0]);
    _paths_travelled.clear();
}

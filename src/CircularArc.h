#pragma once

class CircularArc {
private:
    float _radius;
    float _length;
    float _angle;
    size_t _cntPoints;
    sf::VertexArray lines;

public:

    explicit CircularArc(float radius, float length) :
            _radius(radius), _length(length) {
        _angle = _length / _radius;
        _cntPoints = 250;
        lines = sf::VertexArray(sf::LineStrip, _cntPoints);
        createLine();
    }

    void setRadius(float radius){
        _radius = radius;
    }

    void setLength(float length){
        _length = length;
    }

    void setAngle(){
        _angle = _length / _radius;
    }

    std::size_t getPointCount() const {
        return _cntPoints;
    }

    sf::Vector2f getPoint(std::size_t index) const {
        float angle = _angle * (float)index / (float)_cntPoints;
        float x = std::cos(angle - std::numbers::pi / 2);
        float y = std::sin(angle + std::numbers::pi / 2);
        // std::cout << x * 50 + 642 << " " << y * 50 + 102 << "\n";
        return {x * 211 + 647, y * 215 + 310};
    }

    void createLine(){
        for (int i = 0; i < _cntPoints; ++i){
            lines[i].position = getPoint(i);
            lines[i].color = sf::Color::White;
        }
    }

    sf::VertexArray getArray() const{
        return lines;
    }

    bool Collision(sf::Vector2f coordinates){
        double x = (coordinates.x - 647) / 211;
        double y = (coordinates.y - 310) / 215;
        double alpha = std::acos(x) + std::numbers::pi / 2;
        double beta = std::asin(y) - std::numbers::pi / 2;

        std::cout << (abs(1 - std::pow(std::cos(alpha), 2) + std::pow(std::sin(beta), 2)) < 1e3 && alpha > 0 && alpha < _angle) << "\n";
        return abs(1 - std::pow(std::cos(alpha), 2) + std::pow(std::sin(beta), 2)) < 1e3 && alpha > 0 && alpha < _angle;
    }
};
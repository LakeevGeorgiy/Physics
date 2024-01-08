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

    std::size_t getPointCount() const {
        return _cntPoints;
    }

    sf::Vector2f getPoint(std::size_t index) const {
        float angle = _angle * (float)index / (float)_cntPoints;
        float x = std::cos(angle - std::numbers::pi / 2);
        float y = std::sin(angle + std::numbers::pi / 2);
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

	sf::Vector2f getCenterCoordinates() {
		float angle = std::numbers::pi / 2;
		float x = 0;
		float y = 1;
		return {x, y};
	}

	double distance(sf::Vector2f lhs, sf::Vector2f rhs){
		return std::sqrt(std::pow((lhs.x - rhs.x), 2) + std::pow((lhs.y - rhs.y), 2));
	}

    bool Collision(sf::Vector2f coordinates, double r){
        double x = (coordinates.x) / r;
        double y = (coordinates.y) / r;
		sf::Vector2f newCoordinates(x, y);
		if (x < 0 || y < 0)
			return false;

		double l = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
		double gamma = std::acos((2 - std::pow(l, 2)) / 2);

        return gamma >= 0 &&  gamma < std::numbers::pi / 2 && distance(getCenterCoordinates(), newCoordinates) >= 1.0f - pow(10, -3);
    }
};
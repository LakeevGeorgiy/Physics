#pragma once

#include <iostream>


class CircularMotion{
private:
	StartSettings settings;
	double l;
	double speed;
	double dt = std::pow(10, -2);
	double xOffset = -std::numbers::pi / 2;
	double yOffset = -std::numbers::pi / 2;
	double xBeginPos = 640;
	double yBeginPos = 100;
	double xSpeed = 0;
	double ySpeed = 0;
	double xAcceleration = 0;
	double yAcceleration = 0;
	double g = 9.8;
	double x;
	double y;
	double dl;
public:

	CircularMotion(StartSettings set, double x, double y) : settings(set), speed(set.speed), x(x), y(y){
		CurrentLength();
	}

	void CurrentLength(){
		double length = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
		double alpha = 2 * std::asin(length / settings.radius / 2);
		l = settings.radius * alpha;
	}

	void ChangeCoordinates() {
		x = settings.radius * cos(l / settings.radius + xOffset);
		y = settings.radius * sin(l / settings.radius + yOffset) + settings.radius;
	}

	void ChangeAcceleration(){
		yAcceleration = pow(speed, 2) / settings.radius;
		xAcceleration = -(settings.mu * g * (settings.radius - y) + settings.mu * speed + g * (x)) / settings.radius;
	}

	void ChangeSpeed(){
		speed += xAcceleration * dt;
		xSpeed = speed * std::abs(cos(l / settings.radius));
		ySpeed = speed * sin(l / settings.radius);
	}

	void MoveForward(){
		l = l + speed * dt + xAcceleration * pow(dt, 2) / 2;
	}

	void ForwardIteration(){
		ChangeCoordinates();
		ChangeAcceleration();
		ChangeSpeed();
		MoveForward();
	}

	sf::Vector2f GetCoordinates(){
		double temp = 2 * settings.radius - y; // settings.radius * std::sin(l / settings.radius + std::numbers::pi / 2) + settings.radius;
		return sf::Vector2f(x / settings.radius * 200 + xBeginPos, (temp - settings.radius) / settings.radius * 200 + yBeginPos + 200);
	}

	sf::Vector2f GetCoordinates2(){
		return sf::Vector2f(x, y);
	}

	void ChangeBackwardAcceleration(){
		yAcceleration = pow(speed, 2) / settings.radius;
		xAcceleration = (-settings.mu * g * (settings.radius - y) - settings.mu * std::pow(speed, 2) + g * x) / settings.radius;
	}

	void ChangeSpeedBackward(){
		speed += xAcceleration * dt;
		xSpeed = speed * cos(l / settings.radius);
		ySpeed = speed * sin(l / settings.radius);
	}

	void MoveBackward(){
		dl = -speed * dt - xAcceleration * pow(dt, 2) / 2;
		l = l - speed * dt - xAcceleration * pow(dt, 2) / 2;
	}

	void BackwardIteration(){
		ChangeBackwardAcceleration();
		ChangeSpeedBackward();
		MoveBackward();
		ChangeCoordinates();
	}

	double GetDl(){
		return dl;
	}

	double CurrentSpeed(){
		return speed;
	}

	bool Check(){
		double N = settings.weight * (yAcceleration + g * (settings.radius - y) / settings.radius);
		return speed > 0 && N > 0 && l < settings.length;
	}

	void Fall(){
		ySpeed -= g * dt;
		y = y + ySpeed * dt - g * pow(dt, 2) / 2;
		x -= xSpeed * dt;
		if (x > 0)
			CurrentLength();
	}

	bool Success(){
		return l >= settings.length;
	}

	double GetLength(){
		return l;
	}

	void AbsSpeed(){
		speed = abs(speed);
	}
};
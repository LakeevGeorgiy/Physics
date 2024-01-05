#pragma once

struct StartSettings{
    float radius;
    float weight;
    float length;
    float g;
    float mu;
    float speed;
};

class DeadLoop{
private:

    StartSettings settings;

    double kG = 9.8;
    double x = 0;
    double y = 0;
    double speed = 10;
    double xSpeed = 0;
    double ySpeed = 0;

    double xAcceleration = 0;
    double yAcceleration = 0;
    double acceleration = 0;

    long double dt = pow(10, -2);
    double xOffset = -std::numbers::pi / 2;
    double yOffset = std::numbers::pi / 2;
    double xBeginPos = 640;
    double yBeginPos = 100;
    double l = 0;
    bool win = false;

public:

    DeadLoop(StartSettings set) : settings(set) {
        speed = settings.speed;
    }

    void ChangeCoordinates(){
        x = settings.radius * cos(l / settings.radius + xOffset);
        y = settings.radius * sin(l / settings.radius + yOffset) + settings.radius;
    }

    void ChangeAcceleration(){
        yAcceleration = pow(speed, 2) / settings.radius;
        xAcceleration = -(settings.mu * kG * (settings.radius - y) + settings.mu * speed + kG * (x)) / settings.radius;
    }

    void ChangeSpeed(){
        speed += xAcceleration * dt;
        xSpeed = speed * cos(l / settings.radius);
        ySpeed = speed * sin(l / settings.radius);
    }

    void MoveForward(){
        l = l + speed * dt + xAcceleration * pow(dt, 2) / 2;
    }

    bool Check(){
        return speed > 0 && settings.weight * (yAcceleration + kG * (settings.radius - y) / settings.radius) > 0 && l < settings.length;
    }

    bool Success(){
        return l >= settings.length;
    }

    void ForwardIteration() {
        ChangeCoordinates();
        ChangeAcceleration();
        ChangeSpeed();
        MoveForward();
    }

    void MoveBackward(){
        ySpeed -= kG * dt;
        y = y - ySpeed * dt + kG * pow(dt, 2) / 2;
        x += xSpeed * dt;
    }

    void BackwardIteration() {
        MoveBackward();
        //std::cout << y << " " << ySpeed << " " << x << " " << xSpeed << "\n";
    }

    sf::Vector2f GetCoordinates(){
        return sf::Vector2f(x / settings.radius * 200 + xBeginPos, (y - settings.radius) / settings.radius * 200 + yBeginPos + 200);
    }

    void ChangeDirectory(){
        xSpeed *= -1;
        ySpeed *= -1;
    }
};
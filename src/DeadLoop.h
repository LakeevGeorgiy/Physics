#pragma once

class DeadLoop{
private:

    const double kRadius;
    const double kWeight = 1;
    const double kLength = 5.f / 6 * std::numbers::pi * kRadius;
    const double kG = 9.8;
    const double kMu = 0.05;

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

    DeadLoop(double radius, double weight, double length, double g, double mu, double speed)
        : kRadius(radius), kWeight(weight), kLength(length), kG(g), kMu(mu), speed(speed)
    { }

    void ChangeCoordinates(){
        x = kRadius * cos(l / kRadius + xOffset);
        y = kRadius * sin(l / kRadius + yOffset) + kRadius;
    }

    void ChangeAcceleration(){
        yAcceleration = pow(speed, 2) / kRadius;
        xAcceleration = -(kMu * kG * (kRadius - y) + kMu * speed + kG * (x)) / kRadius;
    }

    void ChangeSpeed(){
        speed += xAcceleration * dt;
        xSpeed = speed * cos(l / kRadius);
        ySpeed = speed * sin(l / kRadius);
    }

    void MoveForward(){
        l = l + speed * dt + xAcceleration * pow(dt, 2) / 2;
    }

    bool Check(){
        return speed > 0 && kWeight * (yAcceleration + kG * (kRadius - y) / kRadius) > 0 && l < kLength;
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
        return sf::Vector2f(x * 50 + xBeginPos, y * 50 + yBeginPos);
    }

    void ChangeDirectory(){
        xSpeed *= -1;
        ySpeed *= -1;
    }
};
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>
#include <cmath>
#include "CircularArc.h"
#include "DeadLoop.h"

bool Simulation(StartSettings settings){
    DeadLoop deadLoop(settings);
    while (deadLoop.Check()){
        deadLoop.ForwardIteration();
    }
    return deadLoop.Success();
}

double FindSolution(StartSettings settings){
    double left = 0;
    double right = 100;
    while (left + 0.001 < right){
        double mid = (left + right) / 2;
        settings.speed = mid;
        if (Simulation(settings))
            right = mid;
        else
            left = mid;
    }
    return right;
}

int main() {

    float kRadius = 5;
    float kWeight = 3;
    float alpha = 4 * std::numbers::pi / 6;
    float kLength = 0;
    float kG = 9.8;
    float kMu = 0.03;
    float speed = 14;
    bool start = false;
    bool end = false;
    float solution = 0;
    StartSettings settings{kRadius, kWeight, kLength, kG, kMu, speed};

    CircularArc arc(kRadius, kLength);
    DeadLoop deadLoop(settings);

    const int windowWidth = 1280;
    const int windowHeight = 720;
    const int frameLimit = 60;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Simulation!");
    window.setFramerateLimit(frameLimit);
    ImGui::SFML::Init(window);

    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::Green);


    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (start && !deadLoop.Check()){
            end = true;
        }
        if (end){
            deadLoop.BackwardIteration();
            /*if (arc.Collision(deadLoop.GetCoordinates()))
                deadLoop.ChangeDirectory();*/
        }
        else if (start) deadLoop.ForwardIteration();

        if (start || end) shape.setPosition(deadLoop.GetCoordinates());
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Start");
        ImGui::SliderFloat("Weight", &settings.weight, 0, 10, "%.3f", 0);
        ImGui::SliderFloat("Radius", &settings.radius, 0, 10, "%.3f", 0);
        ImGui::SliderFloat("Alpha", &alpha, 0, 3 * std::numbers::pi / 2, "%.3f", 0);
        ImGui::SliderFloat("Mu", &settings.mu, 0, 1, "%.3f", 0);
        ImGui::SliderFloat("Speed", &settings.speed, 0, 100, "%.3f", 0);
        if (ImGui::Button("Start")){
            start = true;
            end = false;
            settings.length = alpha * settings.radius;
            arc = CircularArc(settings.radius, settings.length);
            deadLoop = DeadLoop(settings);
        }
        if (ImGui::Button("FindSpeed")){
            settings.length = alpha * settings.radius;
            solution = FindSolution(settings);
        }
        ImGui::InputFloat("Speed", &solution);
        ImGui::End();

        window.clear();
        if (start || end) {
            window.draw(shape);
            window.draw(arc.getArray());
        }
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
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

int main() {

    float kRadius = 4;
    float kWeight = 1;
    float alpha = 0;
    float kLength = 0* std::numbers::pi * kRadius;
    float kG = 9.8;
    float kMu = 0.05;
    float speed = 14;
    bool start = false;
    bool end = false;

    CircularArc arc(0, 0);
    DeadLoop deadLoop(0, 0, 0, 0, 0, 0);

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
        ImGui::SliderFloat("Weight", &kWeight, 0, 100, "%.3f", 0);
        ImGui::SliderFloat("Radius", &kRadius, 0, 100, "%.3f", 0);
        ImGui::SliderFloat("Alpha", &alpha, 0, 3 * std::numbers::pi / 2, "%.3f", 0);
        ImGui::SliderFloat("Mu", &kMu, 0, 1, "%.3f", 0);
        ImGui::SliderFloat("Speed", &speed, 0, 100, "%.3f", 0);
        if (ImGui::Button("Start")){
            start = true;
            kLength = alpha * kRadius;
            arc = CircularArc(kRadius, kLength);
            deadLoop = DeadLoop(kRadius, kWeight, kLength, kG, kMu, speed);
        }
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
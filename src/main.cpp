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

    const double kRadius = 4;
    const double kWeight = 1;
    const double kLength = 9.f / 6 * std::numbers::pi * kRadius;
    const double kG = 9.8;
    const double kMu = 0.05;
    double speed = 14;
    bool end = false;

    CircularArc arc(kRadius, kLength);
    DeadLoop deadLoop(kRadius, kWeight, kLength, kG, kMu, speed);

    const int windowWidth = 1280;
    const int windowHeight = 720;
    const int frameLimit = 60;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Simulation!");
    window.setFramerateLimit(frameLimit);
    ImGui::SFML::Init(window);

    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::Green);

    sf::CircleShape background(210.f);
    background.setFillColor(sf::Color::Black);
    background.setPosition(440, 100);
    sf::CircleShape foreground(210.f);
    foreground.setFillColor(sf::Color::White);
    foreground.setPosition(444, 100);


    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (!deadLoop.Check()){
            end = true;
        }
        if (end){
            deadLoop.BackwardIteration();
            /*if (arc.Collision(deadLoop.GetCoordinates()))
                deadLoop.ChangeDirectory();*/
        }
        else deadLoop.ForwardIteration();

        shape.setPosition(deadLoop.GetCoordinates());
        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear();
        window.draw(shape);
        window.draw(arc.getArray());
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
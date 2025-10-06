#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class GameSystem {
public:
    static void start(unsigned int width, unsigned int height,
        const std::string& name, const float& time_step);
    static void clean();
    static void reset();

private:
    GameSystem() = delete;
    static void _init();
    static void _update(const float& dt);
    static void _render(sf::RenderWindow& window);
};
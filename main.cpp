#include <SFML/Graphics.hpp>
#include <deque>
#include <ctime>
#include <iostream>
using namespace sf;

int main() {
    RenderWindow window(VideoMode({800u, 600u}), "Snake Game");
    const float blockSize = 20.f;
    const int width = 800 / (int)blockSize;
    const int height = 600 / (int)blockSize;

    std::deque<Vector2f> snake;
    snake.push_back({400.f, 300.f});
    Vector2f direction(blockSize, 0.f);
    float delay = 0.1f;
    Clock clock;

    RectangleShape food({blockSize, blockSize});
    food.setFillColor(Color::Red);
    srand((unsigned)time(nullptr));
    food.setPosition({(float)((rand() % width) * (int)blockSize),
                      (float)((rand() % height) * (int)blockSize)});

    RectangleShape block({blockSize - 1, blockSize - 1});
    block.setFillColor(Color::Green);

    Font font;
    if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) return -1;
    Text scoreText(font, "Score: 0", 22);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition({10.f, 10.f});

    int score = 0;
    bool gameOver = false;

    while (window.isOpen()) {
        while (auto event = window.pollEvent())
            if (event->is<Event::Closed>()) window.close();

        if (!gameOver) {
            if (Keyboard::isKeyPressed(Keyboard::Key::Up) && direction.y == 0)
                direction = {0.f, -blockSize};
            if (Keyboard::isKeyPressed(Keyboard::Key::Down) && direction.y == 0)
                direction = {0.f, blockSize};
            if (Keyboard::isKeyPressed(Keyboard::Key::Left) && direction.x == 0)
                direction = {-blockSize, 0.f};
            if (Keyboard::isKeyPressed(Keyboard::Key::Right) && direction.x == 0)
                direction = {blockSize, 0.f};

            if (clock.getElapsedTime().asSeconds() > delay) {
                clock.restart();
                Vector2f newHead = snake.front() + direction;
                if (newHead.x < 0) newHead.x = 800 - blockSize;
                if (newHead.x >= 800) newHead.x = 0;
                if (newHead.y < 0) newHead.y = 600 - blockSize;
                if (newHead.y >= 600) newHead.y = 0;
                for (auto &s : snake)
                    if (s == newHead)
                        gameOver = true;
                snake.push_front(newHead);
                if (snake.front() == food.getPosition()) {
                    score += 10;
                    food.setPosition({(float)((rand() % width) * (int)blockSize),
                                      (float)((rand() % height) * (int)blockSize)});
                } else snake.pop_back();
            }
        }

        window.clear(Color::Black);
        window.draw(food);
        for (auto &s : snake) {
            block.setPosition(s);
            window.draw(block);
        }
        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);

        if (gameOver) {
            Text over(font, "GAME OVER! Press ESC", 28);
            over.setFillColor(Color::Red);
            over.setPosition({200.f, 280.f});
            window.draw(over);
            if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) window.close();
        }
        window.display();
    }
    return 0;
}

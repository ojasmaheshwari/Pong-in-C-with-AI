#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include <imgui.h>
#include <imgui_funcs.h>
#include <imgui-SFML.h>
#include <bot.h>

// Globals
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
int playerWidth = 30;
int playerHeight = 150;
float movementSpeed = 8;
float ballSpeed = 8;
auto ballUnitVector = sf::Vector2f(1, -1);
int MAX_FPS = 60;
bool gameRunning = true;
bool PLYR_1_MOV_DOWN_ENABLED = true;
bool PLYR_2_MOV_DOWN_ENABLED = true;
bool gameOver = false;
bool competitive = false;
bool gamePaused = true;
bool bot = false;
int winner;
int loser;
float finalPos;
int player1_win_count = 0;
int player2_win_count = 0;
int bot_win_count = 0;

sf::RectangleShape drawPlayer(const sf::Vector2f &size, const sf::Color &color, sf::Vector2f pos)
{
    auto player = sf::RectangleShape(size);
    player.setFillColor(color);
    player.setPosition(pos);
    return player;
}

sf::CircleShape drawBall(float radius, const sf::Color &color, sf::Vector2f pos)
{
    auto ball = sf::CircleShape(radius);
    ball.setFillColor(color);
    ball.setPosition(pos);
    return ball;
}

sf::RectangleShape drawWall(const sf::Vector2f &size, sf::Vector2f pos)
{
    auto wall = sf::RectangleShape(size);
    wall.setPosition(pos);
    return wall;
}

sf::Text drawText(std::string content, sf::Vector2f pos, int charSize, sf::Font &font)
{
    auto text = sf::Text();
    text.setString(content);
    text.setPosition(pos);
    text.setCharacterSize(charSize);
    text.setFillColor(sf::Color::White);
    text.setFont(font);

    return text;
}

void update(sf::RenderWindow &window, sf::RectangleShape &player1, sf::RectangleShape &player2, sf::Text &player1_winc_label, sf::Text &player2_winc_label)
{
    player1.setSize(sf::Vector2f(playerWidth, playerHeight));
    player2.setSize(sf::Vector2f(playerWidth, playerHeight));
    window.setFramerateLimit(MAX_FPS);
    std::string player1_win_count_str = "Player 1: " + std::to_string(player1_win_count);
    player1_winc_label.setString(player1_win_count_str);
    if (!bot) {
        std::string player2_win_count_str = "Player 2: " + std::to_string(player2_win_count);
        player2_winc_label.setString(player2_win_count_str);
    }
    else {
        std::string player2_win_count_str = "Lynx: " + std::to_string(bot_win_count);
        player2_winc_label.setString(player2_win_count_str);
    }
    
}

void gameLoop(sf::RenderWindow &window, sf::RectangleShape &player1, sf::RectangleShape &player2, sf::CircleShape &ball, sf::RectangleShape &wallLeft, sf::RectangleShape &wallRight, sf::RectangleShape &wallTop, sf::RectangleShape &wallBottom, sf::Text &gameOverText, sf::Text &winDescription, sf::Clock &deltaClock, sf::Text &player1_winc_label, sf::Text &player2_winc_label)
{
    // Imgui related stuff

    // Draw a black background
    window.clear();

    // If game over then show game over screen
    // Otherwise, continue rendering the game
    if (gameOver)
    {
        window.draw(gameOverText);
        window.draw(winDescription);
        window.display();

        // Restart game on enter key
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            gameOver = false;
            ball.setPosition(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
        }
    }
    else
    {
        if (!gamePaused)
        {
            // check player movement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                player1.move(0, -movementSpeed);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && PLYR_1_MOV_DOWN_ENABLED)
                player1.move(0, movementSpeed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                player2.move(0, -movementSpeed);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && PLYR_2_MOV_DOWN_ENABLED)
            {
                player2.move(0, movementSpeed);
            }

            // check if player hits boundary, if yes then block player
            if (player1.getGlobalBounds().intersects(wallBottom.getGlobalBounds()))
            {
                PLYR_1_MOV_DOWN_ENABLED = false;
            }
            else
            {
                PLYR_1_MOV_DOWN_ENABLED = true;
            }
            if (player1.getPosition().y <= 0)
            {
                player1.setPosition(player1.getPosition().x, 0);
            }
            if (player2.getGlobalBounds().intersects(wallBottom.getGlobalBounds()))
            {
                PLYR_2_MOV_DOWN_ENABLED = false;
            }
            else
            {
                PLYR_2_MOV_DOWN_ENABLED = true;
            }
            if (player2.getPosition().y <= 0)
            {
                player2.setPosition(player2.getPosition().x, 0);
            }

            // make the ball move in direction of unit velocity vector with speed = ballSpeed
            ball.move(ballSpeed * ballUnitVector);

            // check collision between ball and player
            if (ball.getGlobalBounds().intersects(player2.getGlobalBounds()))
            {
                ballUnitVector.x *= -1;
                if (competitive)
                {
                    ballSpeed += 0.25;
                    movementSpeed += 0.25;
                }
            }
            if (ball.getGlobalBounds().intersects(player1.getGlobalBounds()))
            {
                ballUnitVector.x *= -1;
                if (competitive)
                {
                    ballSpeed += 0.25;
                    movementSpeed += 0.25;
                }
            }

            // check collision between ball and right/left walls (game over)
            if (ball.getGlobalBounds().intersects(wallLeft.getGlobalBounds()))
            {
                std::cout << "Game over, player 2 won!\n";
                winner = 1;
                loser = 2;
                
                if (bot)
                {
                    bot_win_count++;
                    winDescription.setString("Lynx won! Get better lmao u big noob");
                }
                else {
                    player2_win_count++;
                    winDescription.setString("Player 2 won! Eat shit player 1!");
                }
                gameOver = true;
            }
            if (ball.getGlobalBounds().intersects(wallRight.getGlobalBounds()))
            {
                std::cout << "Game over, player 1 won!\n";
                if (bot) {
                    player1_win_count++;
                    winDescription.setString("You win! Why u haxxing!?!?!");
                }
                else {
                    player1_win_count++;
                    winDescription.setString("Player 1 won! Eat shit player 2!");
                }
                
                winner = 2;
                loser = 1;
                
                gameOver = true;
            }

            // check collision between ball and top/bottom walls
            // if yes then reflect ball, simply invert y component of ball's unit vector
            if (ball.getGlobalBounds().intersects(wallTop.getGlobalBounds()))
            {
                ballUnitVector.y *= -1;
            }
            if (ball.getGlobalBounds().intersects(wallBottom.getGlobalBounds()))
            {
                ballUnitVector.y *= -1;
            }

            // check if esc key pressed
            // if yes then pause the game
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                gamePaused = true;

            // Update some game objects
            update(window, player1, player2, player1_winc_label, player2_winc_label);
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        if (bot)
            finalPos = predictBallFinalPosition(ball.getPosition().x, SCREEN_WIDTH - playerWidth, ballSpeed * (ballUnitVector.y), ballSpeed * (ballUnitVector.x), ball.getPosition().y);

        pong_engine(playerHeight, ballSpeed, movementSpeed, MAX_FPS, competitive, gamePaused, ball.getPosition().x, ball.getPosition().y, 0, finalPos, bot);

        if (finalPos > 0)
        {
            player2.setPosition(SCREEN_WIDTH - playerWidth, finalPos - playerHeight / 2);
        }

        // Draw game objects onto screen
        window.draw(player1);
        window.draw(player2);
        window.draw(ball);
        window.draw(player1_winc_label);
        window.draw(player2_winc_label);

        ImGui::SFML::Render(window);
        // Display window
        window.display();
    }
}

int main()
{

    // seed random with curr time
    srand(time(NULL));

    // Some solutions of x^2 + y^2 = 2
    // The values of x and y here represent the values of ballUnitVector.x and ballUnitVector.y for which the speed of the ball will remain the same (sqrt(2))
    // std::vector<std::vector<float>> solutions;
    // solutions = {{1.2247, 0.7071}, {0.7071, 1.2247}, {1.3531, 0.492}};

    // int rand_index = rand() % solutions.size();
    // float rand_speed_multiplier_x = solutions[rand_index][0];
    // float rand_speed_multiplier_y = solutions[rand_index][1];
    // ballUnitVector.x *= rand_speed_multiplier_x;
    // ballUnitVector.y *= rand_speed_multiplier_y;

    // Initialization of game objects
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Pong");
    auto player1 = drawPlayer(sf::Vector2f(playerWidth, playerHeight), sf::Color(sf::Color::White), sf::Vector2f(0, (SCREEN_HEIGHT - playerHeight) / 2));
    auto player2 = drawPlayer(sf::Vector2f(playerWidth, playerHeight), sf::Color(sf::Color::White), sf::Vector2f((SCREEN_WIDTH - playerWidth), (SCREEN_HEIGHT - playerHeight) / 2));
    auto ball = drawBall(10, sf::Color(sf::Color::White), sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
    ball.setOrigin(10, 10);
    auto wallLeft = drawWall(sf::Vector2f(1, SCREEN_HEIGHT), sf::Vector2f(0, 0));
    auto wallRight = drawWall(sf::Vector2f(1, SCREEN_HEIGHT), sf::Vector2f(SCREEN_WIDTH, 0));
    auto wallTop = drawWall(sf::Vector2f(SCREEN_WIDTH, 1), sf::Vector2f(0, 0));
    auto wallBottom = drawWall(sf::Vector2f(SCREEN_WIDTH, 1), sf::Vector2f(0, SCREEN_HEIGHT));
    sf::Font font;
    font.loadFromFile("assets/font.ttf");
    auto gameOverText = drawText("Game Over Lil Bro!", sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100), 50, font);
    gameOverText.setOrigin(gameOverText.getGlobalBounds().getSize() / 2.f + gameOverText.getLocalBounds().getPosition());
    auto winDescription = drawText("", sf::Vector2f(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 30), 20, font);
    auto player1_winc_label = drawText("Player 1: ", {50, 20}, 25, font);
    auto player2_winc_label = drawText("Player 2: ", {(SCREEN_WIDTH - 150), 20}, 25, font);

    ImGui::SFML::Init(window);

    // Set the maximum FPS allowed for smooth similar gameplay on all devices

    sf::Clock deltaClock;

    // run the program as long as the window is open
    while (window.isOpen() && gameRunning)
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            switch (event.type)
            {
            // window closed
            case sf::Event::Closed:
                window.close();
                break;

            // we don't process other types of events
            default:
                break;
            }
        }

        // Main game loop
        gameLoop(window, player1, player2, ball, wallLeft, wallRight, wallTop, wallBottom, gameOverText, winDescription, deltaClock, player1_winc_label, player2_winc_label);
    }

    ImGui::SFML::Shutdown();

    return 0;
}

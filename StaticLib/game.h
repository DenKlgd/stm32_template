#ifndef GAME_H
    
    #define GAME_H

    #include <stdint.h>
    #include <stdbool.h>
    #include "delay.h"
    #include "videobuffer.h"
    #include "images.h"

    #define SNAKE_SCALE 5
    #define SNAKE_LENGTH 255
    #define SNAKE_START_LENGTH 15

    #define FRUIT_SIZE 5

    typedef enum GameState
    {
        MainMenu,
        Alive,
        Lose,
        Win,
    } GameState;

    typedef struct Game
    {
        uint8_t score;
        GameState gameState;
    } Game;

    typedef struct Coords
    {
        uint8_t x;
        uint8_t y;
    } Coords;

    typedef struct Velocity
    {
        int8_t x;
        int8_t y;
    } Velocity;

    typedef struct Snake
    {
        Coords coords[SNAKE_LENGTH];
        Velocity headVelocity, tailVelocity;
        Velocity prevDirection;
    } Snake;

    typedef struct Fruit
    {
        Coords coords;
    } Fruit;
    
    extern Game game;
    extern Snake snake;
    extern VideoBuffer videoBuffer;
    extern Fruit fruit;
    extern bool redrawMenu;

    void moveSnake();

    void initSnake();

    void initGame();

    void stopGame(GameState gamestate);

    void placeFruit(uint8_t x, uint8_t y);

    void drawSnake();

    void drawFruit(DrawMethod draw);

    void eatFruit();

    void gameUpdate();

    void drawMenu();

    void drawDigit(uint8_t digit, uint8_t x, uint8_t y);

    void drawScore();

    void drawBorder();

#endif
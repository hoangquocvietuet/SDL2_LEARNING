#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <assert.h>
#include <queue>
#include <vector>
#include <stdio.h>
#include "LTexture.h"
#include "LRandom.h"

/*
    assume that each cell is a 61px square image
*/
const int LENGTH_CELL = 61;
const int numRow = 9;
const int numColumn = 9;
const int numMine = 10;
const int WINDOW_WIDTH = LENGTH_CELL * numColumn;
const int WINDOW_HEIGHT = LENGTH_CELL * numRow;
const std::vector<std::pair<int, int>> dir = {{1, 1}, {1, 0}, {1, -1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, 1}, {0, -1}};

enum Cell {
    defaultCell,
    mineCell,
    emptyCell,
    flagCell,
    numOneCell,
    numTwoCell,
    numThreeCell,
    numFourCell,
    numFiveCell,
    numSixCell,
    numSevenCell,
    numEighthCell,
    totalCell
};

struct LCell {
    Cell mCurrentCellType, mActualCellType;
    SDL_Point topLeftPosition;
    bool isFlag = false;
};

class LGame {
    public:
        LGame();
        LGame(SDL_Renderer* mRenderer, TTF_Font* mFont);
        ~LGame();
        /*
            load ltexture from assets
        */
        bool loadMedia();

        /*
            generate all mines 
            assume that cell (row, col) is safe cell
        */
        void generateMine(int row, int col);

        /*
            bfs if it wasnt a bomb
        */
        void bfs(int row, int col);

        /*
            initialize new map
        */
        void initializeMap(int firstRow, int firstColumn);

        
        /*
            check if gPoint is inside
        */
        bool inside(SDL_Point topLeft, int length, SDL_Point gPoint);

        /*
            given coordinate, return what row and col it was
        */
        bool getCellFromCoor(int& row, int& col, SDL_Point gPoint);

        /*
            return true if it was a mine
            return false
        */
        bool handleEvent(SDL_Event* mEvent);

        /*
            option page contains:
            - win/lose?
            - new game? return true
            - stop game? return false
        */
       bool optionPage(bool win);

        /*
            check if win this game
        */
        bool checkWinGame();

        /*
            load current state of the game
        */
        void loadGame();

        /*
            running and handle game
        */
        void playGame();
    private:
        SDL_Renderer* mRenderer = NULL;
        LTexture mLoadedCell[totalCell];
        LTexture winning, losing, continuing, stopping;
        LCell mCurrentGrid[numRow][numColumn];
        LRandom mRandom;
        SDL_Event mEvent;
        bool quit = false;
        int totalFlag = 0;
        TTF_Font* mFont;
        bool isFirstCellClicked = true;
};
#endif
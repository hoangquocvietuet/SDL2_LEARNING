#include "LGame.h"

LGame::LGame() {}

LGame::LGame(SDL_Renderer* windowRenderer, TTF_Font* globalFont) {
    mRenderer = windowRenderer;
    mFont = globalFont;
    if (!loadMedia()) {
        printf("fail to initialize game\n");
    }
    for (int i = 0; i < numRow; ++ i) {
        for (int j = 0; j < numColumn; ++ j) {
            mCurrentGrid[i][j].mCurrentCellType = defaultCell;
            mCurrentGrid[i][j].topLeftPosition = {j * LENGTH_CELL, i * LENGTH_CELL};
        }
    }
}

LGame::~LGame() {
    for (int i = 0; i < totalCell; ++ i) {
        mLoadedCell[i].free();
    }
    mRenderer = NULL;
}

bool LGame::loadMedia() {
    bool success = true;
    if (!mLoadedCell[defaultCell].loadFromFile(mRenderer, "assets/defaultCell.png")) {
        success = false;
    }
    if (!mLoadedCell[mineCell].loadFromFile(mRenderer, "assets/mineCell.png")) {
        success = false;
    }
    if (!mLoadedCell[emptyCell].loadFromFile(mRenderer, "assets/emptyCell.png")) {
        success = false;
    }
    if (!mLoadedCell[flagCell].loadFromFile(mRenderer, "assets/flagCell.png")) {
        success = false;
    }
    if (!mLoadedCell[numOneCell].loadFromFile(mRenderer, "assets/numOneCell.png")) {
        success = false;
    }
    if (!mLoadedCell[numTwoCell].loadFromFile(mRenderer, "assets/numTwoCell.png")) {
        success = false;
    }
    if (!mLoadedCell[numThreeCell].loadFromFile(mRenderer, "assets/numThreeCell.png")) {
        success = false;
    }
    if (!mLoadedCell[numFourCell].loadFromFile(mRenderer, "assets/numFourCell.png")) {
        success = false;
    }
    if (!mLoadedCell[numFiveCell].loadFromFile(mRenderer, "assets/numFiveCell.png")) {
        success = false;
    }
    if (!mLoadedCell[numSixCell].loadFromFile(mRenderer, "assets/numSixCell.png")) {
        success = false;
    }
    if (!mLoadedCell[numSevenCell].loadFromFile(mRenderer, "assets/numSevenCell.png")) {
        success = false;
    }
    if (!mLoadedCell[numEighthCell].loadFromFile(mRenderer, "assets/numEighthCell.png")) {
        success = false;
    }
    SDL_Color textColor = {0, 0, 0};
    winning.loadFromRenderedText(mFont, mRenderer, "yay, you win this game!", textColor);
    losing.loadFromRenderedText(mFont, mRenderer, "sadly that you lose...", textColor);
    continuing.loadFromRenderedText(mFont, mRenderer, "wanna try again? click here", textColor);
    stopping.loadFromRenderedText(mFont, mRenderer, "its time to stop...byeee~", textColor);
    return success;
}

void LGame::generateMine(int row, int col) {
    std::vector<std::pair<int, int>> remCell;
    for (int i = 0; i < numRow; ++ i) {
        for (int j = 0; j < numColumn; ++ j) {
            remCell.push_back({i, j});
        }
    }
    for (int i = 0; i < numMine; ++ i) {
        assert(remCell.size());
        do {
            mRandom.shuffleVector(remCell);
        } while (remCell.back() == std::make_pair(row, col));
        int u = remCell.back().first;
        int v = remCell.back().second;
        mCurrentGrid[u][v].mActualCellType = mineCell;
        remCell.pop_back();
    }
}

void LGame::bfs(int row, int col) {
    std::queue<std::pair<int, int>> qu;
    qu.push({row, col});
    while (!qu.empty()) {
        int u = qu.front().first;
        int v = qu.front().second;
        qu.pop();
        assert(mCurrentGrid[u][v].mActualCellType != mineCell);
        mCurrentGrid[u][v].mCurrentCellType = mCurrentGrid[u][v].mActualCellType;
        // skip number cell
        if (mCurrentGrid[u][v].mActualCellType != emptyCell) continue;
        for (auto p : dir) {
            int nxtu = u + p.first;
            int nxtv = v + p.second;
            if (nxtu < 0 || nxtv < 0 || nxtu >= numRow || nxtv >= numColumn) continue;
            // never hit
            // if (mCurrentGrid[nxtu][nxtv].mActualCellType == mineCell) continue; 
            if (mCurrentGrid[nxtu][nxtv].mCurrentCellType != mCurrentGrid[nxtu][nxtv].mActualCellType) 
                qu.push({nxtu, nxtv});
        }
    }
}   

void LGame::initializeMap(int firstRow, int firstCol) {
    for (int i = 0; i < numRow; ++ i) {
        for (int j = 0; j < numColumn; ++ j) {
            mCurrentGrid[i][j].mCurrentCellType = defaultCell;
            mCurrentGrid[i][j].mActualCellType = emptyCell;
        }
    }
    generateMine(firstRow, firstCol);
    for (int row = 0; row < numRow; ++ row) {
        for (int col = 0; col < numColumn; ++ col) {
            if (mCurrentGrid[row][col].mActualCellType == mineCell) continue;
            int cnt = 0;
            for (auto tmpDir : dir) {
                int newRow = row + tmpDir.first;
                int newCol = col + tmpDir.second;
                if (newRow < 0 || newCol < 0 || newRow >= numRow || newCol >= numColumn) continue;
                if (mCurrentGrid[newRow][newCol].mActualCellType == mineCell) ++ cnt;
            }
            Cell here;
            switch (cnt)
            {
            case 0:
                here = emptyCell;  
                break;
            case 1:
                here = numOneCell;
                break;
            case 2:
                here = numTwoCell;
                break;
            case 3:
                here = numThreeCell;
                break;
            case 4:
                here = numFourCell;
                break;
            case 5:
                here = numFiveCell;
                break;
            case 6:
                here = numSixCell;
                break;
            case 7:
                here = numSevenCell;
                break;
            case 8:
                here = numEighthCell;
                break;
            }
            mCurrentGrid[row][col].mActualCellType = here;
        }
    }
    bfs(firstRow, firstCol);
}

bool LGame::inside(SDL_Point topLeft, int length, SDL_Point gPoint) {
    if (topLeft.x <= gPoint.x && topLeft.y <= gPoint.y && topLeft.x + length > gPoint.x && topLeft.y + length > gPoint.y) {
        return true;
    }
    return false;
}

bool LGame::getCellFromCoor(int& row, int& col, SDL_Point gPoint) {
    row = col = -1;
    for (int i = 0; i < numRow; ++ i) {
        for (int j = 0; j < numColumn; ++ j) {
            if (inside(mCurrentGrid[i][j].topLeftPosition, LENGTH_CELL, gPoint)) {
                row = i;
                col = j;
                return 1;
            }
        }
    }
    return 0;
}

bool LGame::handleEvent(SDL_Event* mEvent) {
    bool loseGame = false;
    if (mEvent->type == SDL_MOUSEBUTTONDOWN) {
        SDL_Point clickPoint;
        SDL_GetMouseState(&clickPoint.x, &clickPoint.y);
        int row, col;
        getCellFromCoor(row, col, clickPoint);

        // check if it was the first click
        if (isFirstCellClicked == true) {
            isFirstCellClicked = false;
            initializeMap(row, col);
        }

        if (mEvent->button.button == SDL_BUTTON_LEFT && !mCurrentGrid[row][col].isFlag) {
            // process 
            switch (mCurrentGrid[row][col].mActualCellType)
            {
            case mineCell:
                mCurrentGrid[row][col].mCurrentCellType = mCurrentGrid[row][col].mActualCellType;
                loseGame = true;
                break;

            default:
                bfs(row, col);
                break;
            }
        } else if (mEvent->button.button == SDL_BUTTON_RIGHT) {
            if (mCurrentGrid[row][col].isFlag == false // isnt flag
            && mCurrentGrid[row][col].mCurrentCellType != mCurrentGrid[row][col].mActualCellType // didnt open
            && totalFlag < numMine) { // current flag < total mine
                mCurrentGrid[row][col].isFlag = true;
                ++ totalFlag;
            } else if (mCurrentGrid[row][col].isFlag == true) {
                mCurrentGrid[row][col].isFlag = false;
                -- totalFlag;
            }
        }
    }
    return loseGame;
}

bool LGame::optionPage(bool win) {
    while (true) {
        SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(mRenderer);
        if (win) {
            winning.render(mRenderer, 50, 50);
        } else {
            losing.render(mRenderer, 50, 50);
        }
        continuing.render(mRenderer, 50, 150);
        stopping.render(mRenderer, 50, 250);
        SDL_RenderPresent(mRenderer); 
        while (SDL_PollEvent(&mEvent)) {
            if (mEvent.type != SDL_MOUSEBUTTONDOWN) continue;
            SDL_Point clickPoint;
            SDL_GetMouseState(&clickPoint.x, &clickPoint.y);
            if (clickPoint.x >= 50 && clickPoint.x < 50 + continuing.getWidth()
             && clickPoint.y >= 150 && clickPoint.y < 150 + continuing.getHeight()) {
                isFirstCellClicked = true;
                for (int i = 0; i < numRow; ++ i) {
                    for (int j = 0; j < numColumn; ++ j) {
                        mCurrentGrid[i][j].mCurrentCellType = defaultCell;
                    }
                }
                return true;
            }
            if (clickPoint.x >= 50 && clickPoint.x < 50 + stopping.getWidth()
             && clickPoint.y >= 250 && clickPoint.y < 250 + stopping.getHeight()) 
                return false;
        }
    }
    // never run here
    assert(false);
}

bool LGame::checkWinGame() {
    for (int i = 0; i < numRow; ++ i) {
        for (int j = 0; j < numColumn; ++ j) {
            if (mCurrentGrid[i][j].mCurrentCellType != mCurrentGrid[i][j].mActualCellType) {
                return false;
            }
        }
    }
    return true;
}

void LGame::loadGame() {
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(mRenderer);
    for (int i = 0; i < numRow; ++ i) {
        for (int j = 0; j < numColumn; ++ j) {
            Cell tmpType = mCurrentGrid[i][j].mCurrentCellType;
            if (mCurrentGrid[i][j].isFlag) {
                tmpType = flagCell;
            }
            mLoadedCell[tmpType].render(mRenderer, 
                                    mCurrentGrid[i][j].topLeftPosition.x, 
                                    mCurrentGrid[i][j].topLeftPosition.y);
        }
    }
    SDL_RenderPresent(mRenderer); 
}

void LGame::playGame() {
    while (!quit) { 
        loadGame();
        while (SDL_PollEvent(&mEvent)) {
            if (mEvent.type == SDL_QUIT) {
                quit = true;
            } else {
                if (handleEvent(&mEvent)) {
                    loadGame();
                    SDL_Delay(500);
                    quit = !optionPage(0);   
                }
            }
        }
        if (checkWinGame()) {
            quit = !optionPage(1);
        }
    }
}
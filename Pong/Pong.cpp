#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_version.h>
#include "Pong.h"
#include <stdlib.h>
#include <iostream>

struct Vector2 {
    float x;
    float y;
};

SDL_Renderer* mRenderer;
Vector2 mPaddlePosition;
Vector2 mBallPosition;
Uint32 mTicksCount;
int mPaddleDir;
const int mPaddleH = 100;
const int mPaddleT = 15;

Game::Game()
    : mWindow(nullptr)
    , mIsRunning(true)
{
    mBallPosition = {1024.0 / 2, 768.0 / 2};
    mPaddlePosition = {0,768.0 / 2};
    mTicksCount = 0;
    mPaddleDir = 0;
}

int main(int argc, char** argv)
{
    Game game;
    bool success = game.Initialize();

    if (success)
    {
        game.RunLoop();
    }

    game.Shutdown();

    return 0;
}

bool
Game::Initialize()
{
    mWindow = nullptr;
    mIsRunning = true;

    bool sdlResult = SDL_Init(SDL_INIT_VIDEO);

    if (sdlResult == false)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        std::cerr << "SDL_Init failed: [" << SDL_GetError() << "]" << std::endl;
        return false;
    }

    mWindow = SDL_CreateWindow("Pong", 1024, 768, 0);

    if (!mWindow)
    {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, nullptr);

    if (!mRenderer)
    {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        return false;
    }

    return true;
}

void
Game::Shutdown()
{
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    SDL_Quit();
}

void
Game::RunLoop()
{
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void
Game::UpdateGame()
{
    while (!SDL_GetTicks() >= mTicksCount + 16)
        ;
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 5000.0f;
    mTicksCount = SDL_GetTicks();

    if (mPaddleDir != 0) {
        mPaddlePosition.y += mPaddleDir * 1.0f * deltaTime;

        if (mPaddlePosition.y < (mPaddleH/2.0f + mPaddleT)) {
            mPaddlePosition.y = mPaddleH/2.0f + mPaddleT;
        }
        else if (mPaddlePosition.y > (768.0f - mPaddleH/2.0f - mPaddleT)) {
            mPaddlePosition.y = 768.0f - mPaddleH/2.0f - mPaddleT;
        }
    }
}

void
Game::ProcessInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                mIsRunning = false;
                break;
        }
    }

    const bool* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }
    if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
        mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) {
        mPaddleDir += 1;
    }
}

void Game::GenerateWalls() {
    const int thickness = 15;
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    SDL_FRect topWall {
        0,
        0,
        1024,
        thickness

    };

    SDL_RenderFillRect(mRenderer, &topWall);

    SDL_FRect bottomWall {
        0,
        768 - thickness,
        1024,
        thickness
    };

    SDL_RenderFillRect(mRenderer, &bottomWall);

    SDL_FRect backWall {
        1024 - thickness,
        0,
        thickness,
        768
    };

    SDL_RenderFillRect(mRenderer, &backWall);
}

void
Game::GeneratePaddle() {
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    SDL_FRect ball {
        mPaddlePosition.x - mPaddleT / 2,
        mPaddlePosition.y - mPaddleH / 2,
        mPaddleT,
        mPaddleH
    };

    SDL_RenderFillRect(mRenderer, &ball);
}

void
Game::GenerateBall() {
    const int thickness = 10;
    const int height = 10;
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    SDL_FRect paddle {
        mBallPosition.x - thickness / 2,
        mBallPosition.y - height / 2,
        thickness,
        height
    };

    SDL_RenderFillRect(mRenderer, &paddle);
}

void
Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
    SDL_RenderClear(mRenderer);
    GenerateWalls();
    GenerateBall();
    GeneratePaddle();
    SDL_RenderPresent(mRenderer);
}


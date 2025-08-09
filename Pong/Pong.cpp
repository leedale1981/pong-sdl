#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_version.h>
#include "Pong.h"
#include <stdlib.h>
#include <iostream>

Game::Game()
    : mWindow(nullptr)
    , mIsRunning(true)
{
}

SDL_Renderer* mRenderer;

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
}

void
Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
    SDL_RenderClear(mRenderer);
    SDL_RenderPresent(mRenderer);
}


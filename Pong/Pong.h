class Game
{
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();
private:
    void ProcessInput();

    void GenerateWalls();
    void GeneratePaddle();
    void GenerateBall();

    void UpdateGame();
    void GenerateOutput();

    SDL_Window* mWindow;
    bool mIsRunning;
};

#include "SDL2/SDL.h"

#include "verlet.hpp"


class Simulation
{
private:
    physics::Verlet<float>* world;
    SDL_Window* window;
    SDL_Renderer* renderer;

    int InitializeSDL();
    void DestroySDL();
    void CreateWorld(int width, int height);
public:
    Simulation();
    ~Simulation();

    bool HandleInput();
    void Update();
    void Draw();
};


#include "SDL2/SDL.h"

#include "vector2d.hpp"
#include "verlet.hpp"


class Simulation
{
private:
    float world_width;
    float world_height;
    float world_aspect_ratio;
    
    int renderer_width;
    int renderer_height;
    
    physics::Verlet<float>* world;
    SDL_Window* window;
    SDL_Renderer* renderer;

    int InitializeSDL();
    void DestroySDL();
    void CreateWorld(int width, int height);

    math::Vector2d<float> ScaleFromWorldToRenderer(math::Vector2d<float> position) const;
public:
    Simulation();
    ~Simulation();

    bool HandleInput();
    void Update();
    void Draw();
};


#include "SDL2/SDL.h"

#include "math/vector2d.hpp"
#include "verlet/verlet.hpp"


namespace simulation
{
    class Simulation
    {
    private:
        float world_width;
        float world_height;
        float world_aspect_ratio;
        int renderer_width;
        int renderer_height;

        SDL_Window* window;
        SDL_Renderer* renderer;

        simulation::ObjectPool<float>* object_pool;
        verlet::Verlet<float>* world;

        int InitializeSDL();
        void DestroySDL();
        bool CreateWorld(int width, int height);

        inline math::Vector2d<float> ScaleFromWorldToRenderer(math::Vector2d<float> position) const;
    public:
        Simulation();
        ~Simulation();

        bool HandleInput();
        void Update();
        void Draw();
    };
}
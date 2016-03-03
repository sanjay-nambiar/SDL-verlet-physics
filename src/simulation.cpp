
#include "simulation/simulation.hpp"

#include <iostream>

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

#include "math/vector2d.hpp"
#include "verlet/particle.hpp"
#include "verlet/constraints.hpp"
#include "verlet/composite.hpp"
#include "verlet/objects.hpp"
#include "verlet/verlet.hpp"

#define WORLD_WIDTH 1000
#define WORLD_HEIGHT 700
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

#define MAX_PARTICLES 5000
#define MAX_PIN_CONSTRAINTS 100
#define MAX_DISTANCE_CONSTRAINTS 5000
#define MAX_ANGULAR_CONSTRAINTS 0
#define MAX_COMPOSITES 50

#define VERLET_PARTICLE_COLOR 0xFF00FF00
#define VERLET_PIN_COLOR 0xFF0000FF
#define VERLET_LINE_COLOR 0xFFFFFFFF

namespace simulation
{
    using namespace verlet;

    // Private methods

    int Simulation::InitializeSDL()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return 1;
        }
        
        window = SDL_CreateWindow("Verlet Sim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr)
        {
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr)
        {
            SDL_DestroyWindow(window);
            std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }

        if (SDL_GetRendererOutputSize(renderer, &renderer_width, &renderer_height) != 0)
        {
            std::cout << "SDL_GetRendererOutputSize Error: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }

        return 0;
    }

    void Simulation::DestroySDL()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    inline bool Simulation::CreateLineSegments()
    {
        std::vector<math::Vector2d<float> > segment_points = {
            math::Vector2d<float>(0,0), math::Vector2d<float>(20,0),
            math::Vector2d<float>(40,0), math::Vector2d<float>(60,0),
            math::Vector2d<float>(80,0), math::Vector2d<float>(100,0),
            math::Vector2d<float>(120,0), math::Vector2d<float>(140,0),
            math::Vector2d<float>(160,0), math::Vector2d<float>(180,0),
            math::Vector2d<float>(200,0), math::Vector2d<float>(220,0),
            math::Vector2d<float>(240,0), math::Vector2d<float>(260,0),
            math::Vector2d<float>(280,0), math::Vector2d<float>(300,0)
        };
        std::vector<int> segment_pin_particle_indexes = {0};
        math::Vector2d<float> segment_position_offset(400, 30);
        float segment_stiffness = 0.2;
        
        Composite<float>* segment = LineSegments<float>(segment_points, segment_pin_particle_indexes,
            segment_position_offset, segment_stiffness, object_pool);

        return (segment != nullptr);
    }

    inline bool Simulation::CreateBoxes()
    {
        std::vector<math::Vector2d<float> > box_points = {
            math::Vector2d<float>(40,0), math::Vector2d<float>(110,0),
            math::Vector2d<float>(150,75),
            math::Vector2d<float>(75,150), math::Vector2d<float>(0,75)
        };
        std::vector<std::pair<int, int> > constraint_particle_indexes = {
            {0,1}, {1,2}, {2,3}, {3,4}, {0,4},
            {0,2}, {0,3}, {1,3}, {1,4}, {2,4}
        };
        math::Vector2d<float> box_position_offset(100, 200);
        float box_stiffness = 1;
        Composite<float>* box = Polygon<float>(box_points, constraint_particle_indexes, box_position_offset, 
            box_stiffness, object_pool);

        return (box != nullptr);
    }

    inline bool Simulation::CreateTire()
    {
        math::Vector2d<float> center(500, 200);
        float tread_stiffness = 1, spoke_stiffness = 1, radius = 100;
        int segments = 30;
        Composite<float>* tire = Tire<float>(center, radius, segments, spoke_stiffness, tread_stiffness,
            object_pool);

        return (tire != nullptr);
    }

    inline bool Simulation::CreateCloth()
    {
        float width = 300, height = 350;
        int segments = 20;
        int pin_mod = 5;
        float stiffness = 0.9;
        math::Vector2d<float> top_left(700, 50);

        Composite<float>* cloth = Cloth<float>(top_left, width, height, segments, pin_mod, stiffness,
            object_pool);

        return (cloth != nullptr);
    }

    bool Simulation::CreateWorld(int width, int height)
    {
        object_pool = new ObjectPool<float>(MAX_PARTICLES, MAX_PIN_CONSTRAINTS, MAX_DISTANCE_CONSTRAINTS,
            MAX_ANGULAR_CONSTRAINTS, MAX_COMPOSITES);

        world_width = width;
        world_height = height;
        world_aspect_ratio = width / height;
        world = new Verlet<float>(width, height, object_pool);

        return CreateLineSegments()
            && CreateBoxes()
            && CreateTire()
            && CreateCloth();
    }


    inline math::Vector2d<float> Simulation::ScaleFromWorldToRenderer(math::Vector2d<float> position) const
    {
        return math::Vector2d<float>(position.x, position.y);
    }


    // Public methods

    Simulation::Simulation()
    {
        // InitializeSDL();
        if (!CreateWorld(WORLD_WIDTH, WORLD_HEIGHT))
        {
            exit(1);
        }
    }

    Simulation::~Simulation()
    {
        // DestroySDL();
        delete this->world;
        delete this->object_pool;
    }

    bool Simulation::HandleInput()
    {
        SDL_Event event;

        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return false;
            }
      
            if (event.type == SDL_KEYDOWN)
            {
                SDL_Keycode keyPressed = event.key.keysym.sym;
                switch (keyPressed)
                {
                    case SDLK_ESCAPE:
                        return false;
                }
            }
        }
        return true;
    }

    void Simulation::Update()
    {
        world->Update(16);
    }

    void Simulation::Draw()
    {
        // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        // SDL_RenderClear(renderer);

        const Particle<float>* particle = object_pool->particles;
        int particle_count = object_pool->particle_count;
        for (int p = 0; p < particle_count; ++p, ++particle)
        {
            math::Vector2d<float> scaled_position = particle->position;
            // filledCircleColor(renderer, scaled_position.x, scaled_position.y, 3, VERLET_PARTICLE_COLOR);
        }

        const DistanceConstraint<float>* distance_constraint = object_pool->distance_constraints;
        int constraint_count = object_pool->distance_constraints_count;
        for (int c = 0; c < constraint_count; ++c, ++distance_constraint)
        {
            math::Vector2d<float> scaled_position1 = distance_constraint->particle1->position;
            math::Vector2d<float> scaled_position2 = distance_constraint->particle2->position;

            // lineColor(renderer, scaled_position1.x, scaled_position1.y, 
            //     scaled_position2.x, scaled_position2.y, VERLET_LINE_COLOR);
        }

        const PinConstraint<float>* pin_constraint = object_pool->pin_constraints;
        constraint_count = object_pool->pin_constraints_count;
        for (int c = 0; c < constraint_count; ++c, ++pin_constraint)
        {
            math::Vector2d<float> scaled_position = pin_constraint->particle->position;
            // filledCircleColor(renderer, scaled_position.x, scaled_position.y, 5, VERLET_PIN_COLOR);
        }
        // SDL_RenderPresent(renderer);
    }
}

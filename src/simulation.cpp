
#include "simulation.hpp"

#include <iostream>

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

#include "vector2d.hpp"
#include "particle.hpp"
#include "constraints.hpp"
#include "composite.hpp"
#include "objects.hpp"
#include "verlet.hpp"


#define VERLET_PARTICLE_COLOR 0x00FF00FF
#define VERLET_PIN_COLOR 0xFF0000FF
#define VERLET_LINE_COLOR 0xFFFFFFFF
#define WORLD_WIDTH 800
#define WORLD_HEIGHT 600


using namespace physics;

// Private methods

int Simulation::InitializeSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    window = SDL_CreateWindow("Verlet Sim", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
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
    return 0;
}

void Simulation::DestroySDL()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Simulation::CreateWorld(int width, int height)
{
    world = new Verlet<float>(width, height);
    
    // entities
    std::vector<math::Vector2d<float> > segment_points = { math::Vector2d<float>(20,10), math::Vector2d<float>(40,10),
        math::Vector2d<float>(60,10), math::Vector2d<float>(80,10), math::Vector2d<float>(100,10) };
    
    Composite<float>* segment = LineSegments<float>(segment_points, 0.02);
    (*segment).Pin(0);
    (*segment).Pin(4);
    world->AddComposite(segment);
}



// Public methods

Simulation::Simulation()
{
    InitializeSDL();
    CreateWorld(WORLD_WIDTH, WORLD_HEIGHT);
}

Simulation::~Simulation()
{
    DestroySDL();
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
    SDL_RenderClear(renderer);
    
    for (auto compositeIt = world->composites.begin(); compositeIt != world->composites.end(); compositeIt++)
    {
        for (auto constraintIt = (*compositeIt)->constraints.begin();
             constraintIt != (*compositeIt)->constraints.end(); constraintIt++)
        {
            //PinConstraint
            if (dynamic_cast<PinConstraint<float>*>(*constraintIt) != nullptr)
            {
                PinConstraint<float>* pin_constraint = dynamic_cast<PinConstraint<float>*>(*constraintIt);
                math::Vector2d<float> position = pin_constraint->particle.position;
                filledCircleColor(renderer, position.x, position.y, 20, VERLET_PIN_COLOR);
            }
            //DistanceConstraint
            else if (dynamic_cast<DistanceConstraint<float>*>(*constraintIt) != nullptr)
            {
                DistanceConstraint<float>* distance_constraint = dynamic_cast<DistanceConstraint<float>*>(*constraintIt);
                math::Vector2d<float> position1 = distance_constraint->p1.position;
                math::Vector2d<float> position2 = distance_constraint->p2.position;
                lineColor(renderer, position1.x, position1.y, position2.x, position2.y, VERLET_LINE_COLOR);
                filledCircleColor(renderer, position1.x, position1.y, 10, VERLET_PARTICLE_COLOR);
                filledCircleColor(renderer, position2.x,position2.y, 10, VERLET_PARTICLE_COLOR);
            }
        }
    }
    
    SDL_RenderPresent(renderer);
}

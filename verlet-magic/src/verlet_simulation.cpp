//
//  verlet_worldulation.cpp
//  
//
//  Created by Nambiar, Sanjay M on 1/27/16.
//
//

#include <iostream>

#include "SDL2/SDL.h"
#include "SDL2_gfx/SDL2_gfxPrimitives.h"

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


class Simulation
{
    Verlet<float>* world;
    SDL_Window* window;
    SDL_Renderer* renderer;

    int InitializeSDL()
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
    
    void DestroySDL()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    
    void createWorld(int width, int height)
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
    
public:
    Simulation()
    {
        InitializeSDL();
        createWorld(WORLD_WIDTH, WORLD_HEIGHT);
    }
    
    ~Simulation()
    {
        DestroySDL();
    }

    void Update()
    {
        world->Update(16);
    }
    
    void Draw()
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
};


int main()
{
    Simulation sim;
    
    while(true)
    {
        sim.Update();
        sim.Draw();
    }
    
    return 0;
}

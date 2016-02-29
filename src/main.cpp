
#include "SDL2/SDL.h"

#include "simulation/simulation.hpp"

int main()
{
    simulation::Simulation sim;

    SDL_Delay(1000);
    while(sim.HandleInput())
    {    	
        sim.Update();
        sim.Draw();
    }
    
    return 0;
}

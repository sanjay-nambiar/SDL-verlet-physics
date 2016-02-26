
#include "simulation/simulation.hpp"

int main()
{
    simulation::Simulation sim;

    while(sim.HandleInput())
    {    	
        sim.Update();
        sim.Draw();
    }
    
    return 0;
}


#include "simulation/simulation.hpp"

int main()
{
    simulation::Simulation sim;

    for(int i=0; i<20000; i++)
    // while(sim.HandleInput())
    {    	
        sim.Update();
        sim.Draw();
    }
    
    return 0;
}

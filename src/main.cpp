
#include "simulation.hpp"

int main()
{
    Simulation sim;
    while(sim.HandleInput())
    {    	
        sim.Update();
        sim.Draw();
    }
    
    return 0;
}

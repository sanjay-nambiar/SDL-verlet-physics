
#include "simulation.hpp"

int main()
{
    Simulation sim;
    for(int i=0; i<20000; i++)
    // while(sim.HandleInput())
    {    	
        sim.Update();
        sim.Draw();
    }
    
    return 0;
}

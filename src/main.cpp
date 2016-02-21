
#include "simulation.hpp"

int main()
{
    Simulation sim;
    
    // while(std::cin.ignore())
    while(sim.HandleInput())
    {    	
        sim.Update();
        sim.Draw();
    }
    
    return 0;
}

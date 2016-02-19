//
//  particle.hpp
//  
//
//  Created by Nambiar, Sanjay M on 1/28/16.
//
//

#ifndef ____particle__
#define ____particle__


#include "vector2d.hpp"


namespace physics
{
    template <class T>
    class Particle
    {
    public:
        math::Vector2d<T> position;
        math::Vector2d<T> last_position;
        
        Particle(math::Vector2d<T>& position)
        : position(math::Vector2d<T>(position)), last_position(math::Vector2d<T>(position))
        {
        }
        
        Particle(Particle<T>& particle)
        : position(math::Vector2d<T>(particle.position)), last_position(math::Vector2d<T>(particle.position))
        {
        }
        
        void SetPosition(math::Vector2d<T>& position) {
            this->last_position = this->position;
            this->position = position;
        }
        
        void operator=(const Particle<T>& particle)
        {
            position = particle.position;
            last_position = particle.last_position;
        }
    };
}

#endif /* defined(____particle__) */

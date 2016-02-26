
#ifndef ____particle__
#define ____particle__


#include "math/vector2d.hpp"


namespace verlet
{
    template <class T>
    class Particle
    {
        static_assert(std::is_floating_point<T>::value,
                      "Particle can be of floating point data types only");
    public:
        math::Vector2d<T> position;
        math::Vector2d<T> last_position;

        Particle()
        {
            this->position = math::Vector2d<T>(0, 0);
            this->last_position = this->position;
        }
        
        Particle(math::Vector2d<T>& position)
        {
            this->position = position;
            this->last_position = position;
        }
        
        Particle(Particle<T>& particle)
        {
            this->position = particle.position;
            this->last_position = particle.last_position;
        }
    };
}

#endif /* defined(____particle__) */


#ifndef ____composite__
#define ____composite__


#include <vector>

#include "verlet/particle.hpp"
#include "verlet/constraints.hpp"


namespace verlet
{
    template <class T>
    class Composite
    {
        static_assert(std::is_floating_point<T>::value,
              "Composite can be of floating point data types only");

        std::vector<Particle<T>*> _particles;
        std::vector<Constraint<T>*> _constraints;
    public:
        const std::vector<Particle<T>*>& particles;
        const std::vector<Constraint<T>*>& constraints;

        const int particle_count()
        {
            return _particles.size();
        }

        const int constraint_count()
        {
            return _constraints.size();
        }

        Composite() : particles(_particles), constraints(_constraints)
        {
        }

        void AddParticle(Particle<T>* particle)
        {
            _particles.push_back(particle);
        }

        void AddConstraint(Constraint<T>* constraint)
        {
            _constraints.push_back(constraint);
        }

        void operator=(const Composite<T>& composite)
        {
            _particles = composite.particles;
            _constraints = composite._constraints;
        }
    };
}


#endif /* defined(____composite__) */

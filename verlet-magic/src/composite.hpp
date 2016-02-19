//
//  composite.hpp
//
//
//  Created by Nambiar, Sanjay M on 1/29/16.
//
//

#ifndef ____composite__
#define ____composite__


#include <vector>

#include "particle.hpp"
#include "constraints.hpp"


namespace physics
{
    template <class T>
    class Composite
    {
    public:
        std::vector<Particle<T>*> particles;
        std::vector<Constraint<T>*> constraints;

        const int particle_count()
        {
            return particles.size();
        }

        const int constraint_count()
        {
            return constraints.size();
        }

        Composite(std::vector<Particle<T>*>& particles, std::vector<Constraint<T>*>& constraints)
        : particles(particles), constraints(constraints)
        {
        }

        Composite(int particles_count, int constraints_count)
        {
            particles.reserve(particles_count);
            constraints.reserve(constraints_count);
        }

        void AddParticle(Particle<T>* particle)
        {
            particles.push_back(particle);
        }

        void AddConstraint(Constraint<T>* constraint)
        {
            constraints.push_back(constraint);
        }

        void operator=(const Composite<T>& composite)
        {
            particles = composite.particles;
            constraints = composite.constraints;
        }

        Constraint<T>* Pin(int index, math::Vector2d<T>& position)
        {
            Constraint<T>* constraint = new PinConstraint<T>(*particles[index], position);
            this->constraints.push_back(constraint);
            return constraint;
        }

        Constraint<T>* Pin(int index)
        {
            return Pin(index, (*particles[index]).position);
        }
    };
}


#endif /* defined(____composite__) */
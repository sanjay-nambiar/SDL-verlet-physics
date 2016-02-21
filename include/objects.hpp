
#ifndef ____basic_objects__
#define ____basic_objects__

#include <cmath>

#include "vector2d.hpp"
#include "particle.hpp"
#include "constraints.hpp"
#include "composite.hpp"


namespace physics {

    template<class T> Composite<T>* Point(math::Vector2d<T>& position)
    {
        std::vector<Particle<T>*> particles(1, new Particle<T>(position));
        return new Composite<T>(particles, std::vector<Constraint<T>*>());
    }

    template<class T> Composite<T>* LineSegments(std::vector<math::Vector2d<T> >& vertices, 
        math::Vector2d<T>& position_offset, T stiffness)
    {
        int vertex_count = (int) vertices.size();
        Composite<T>* composite = new Composite<T>(vertex_count, vertex_count - 1);

        Particle<T>* prev_particle;
        Particle<T>* particle;

        auto it = vertices.begin();
        math::Vector2d<T> actual_position = (*it) + position_offset;
        prev_particle = new Particle<T>(actual_position);
        composite->AddParticle(prev_particle);
        for (++it; it != vertices.end(); ++it)
        {
            actual_position = (*it) + position_offset;
            particle = new Particle<T>(actual_position);
            composite->AddParticle(particle);
            composite->AddConstraint(new DistanceConstraint<T> (prev_particle, particle, stiffness));
            prev_particle = particle;
        }
        return composite;
    }

    template<class T> Composite<T>* Tire(math::Vector2d<T>& origin, T radius, int segments,
                                                       T spoke_stiffness, T tread_stiffness)
    {
        T stride = (2*M_PI)/segments;
        Composite<T>* composite = new Composite<T>(segments+1, segments*3);

        // particles
        for (int i = 0; i < segments; ++i)
        {
            T theta = i * stride;
            math::Vector2d<T> particle_position((origin.x + cos(theta)*radius), (origin.y + sin(theta)*radius));
            Particle<T>* particle = new Particle<T>(particle_position);
            composite->AddParticle(particle);
        }

        Particle<T>* center = new Particle<T>(origin);
        composite->AddParticle(center);

        // constraints
        for (int i = 0; i < segments; ++i)
        {
            Constraint<T>* distance_constraint1 =
                new DistanceConstraint<T> (composite->particles[i], composite->particles[(i+1)%segments], tread_stiffness);
            composite->AddConstraint(distance_constraint1);

            Constraint<T>* distance_constraint2 =
                new DistanceConstraint<T> (composite->particles[i], center, spoke_stiffness);
            composite->AddConstraint(distance_constraint2);

            Constraint<T>* distance_constraint3 =
                new DistanceConstraint<T> (composite->particles[i], composite->particles[(i+5)%segments], tread_stiffness);
            composite->AddConstraint(distance_constraint3);
        }

        return composite;
    }
}


#endif /* defined(____basic_objects__) */

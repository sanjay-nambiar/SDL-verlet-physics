
#ifndef ____basic_objects__
#define ____basic_objects__

#include <iostream>

#include "math/vector2d.hpp"
#include "verlet/particle.hpp"
#include "verlet/constraints.hpp"
#include "verlet/composite.hpp"
#include "simulation/object_pool.hpp"


namespace simulation {

    using namespace verlet;

    template<class T> Composite<T>* Point(math::Vector2d<T>& position, ObjectPool<T>* object_pool)
    {
        static_assert(std::is_floating_point<T>::value,
              "Point can be of floating point data types only");

        if (object_pool->CanAllocate(1, 0, 0, 0, 1))
        {
            Particle<T>* particle = object_pool->AllocateParticles(1);
            Composite<T>* composite = object_pool->AllocateComposites(1);

            *particle = Particle<T>(position);
            *composite = Composite<T>();

            composite->AddParticle(particle);
            return composite;
        }
        return nullptr;
    }

    template<class T> Composite<T>* LineSegments(std::vector<math::Vector2d<T> >& vertices,
        std::vector<int> pin_particle_indexes, math::Vector2d<T>& position_offset, T stiffness, 
        ObjectPool<T>* object_pool)
    {
        static_assert(std::is_floating_point<T>::value,
              "LineSegments can be of floating point data types only");

        int vertex_count = (int) vertices.size();
        int pin_constraints_count = (int) pin_particle_indexes.size();

        if (object_pool->CanAllocate(vertex_count, pin_constraints_count, vertex_count-1, 0, 1))
        {
            Composite<T>* composite = object_pool->AllocateComposites(1);
            Particle<T>* particles = object_pool->AllocateParticles(vertex_count);
            PinConstraint<T>* pin_constraints = object_pool->AllocatePinConstraints(pin_constraints_count);
            DistanceConstraint<T>* distance_constraints = object_pool->AllocateDistanceConstraints(vertex_count-1);

            *composite = Composite<T>();
            Particle<T>* prev_particle = &particles[0];
            Particle<T>* particle = &particles[1];
            DistanceConstraint<T>* distance_constraint = &distance_constraints[0];

            auto it = vertices.begin();
            math::Vector2d<T> actual_position = (*it) + position_offset;
            *prev_particle = Particle<T>(actual_position);
            composite->AddParticle(prev_particle);

            for (++it; it != vertices.end(); ++it, ++particle, ++distance_constraint)
            {
                actual_position = (*it) + position_offset;
                *particle = Particle<T>(actual_position);
                composite->AddParticle(particle);

                *distance_constraint = DistanceConstraint<T> (prev_particle, particle, stiffness);
                composite->AddConstraint(distance_constraint);
                prev_particle = particle;
            }

            PinConstraint<T>* pin_constraint = &pin_constraints[0];
            for(auto it = pin_particle_indexes.begin(); it != pin_particle_indexes.end(); ++it, ++pin_constraint)
            {
                *pin_constraint = PinConstraint<T>(&particles[*it], particles[*it].position);
                composite->AddConstraint(pin_constraint);
            }

            return composite;
        }
        return nullptr;
    }

    template<class T> Composite<T>* Polygon(std::vector<math::Vector2d<T> >& vertices, 
        std::vector<std::pair<int, int> >& constraint_pairs, math::Vector2d<T>& position_offset,
        T stiffness, ObjectPool<T>* object_pool)
    {
        static_assert(std::is_floating_point<T>::value,
              "Polygon can be of floating point data types only");

        int vertex_count = (int) vertices.size();
        int constraints_count = (int) constraint_pairs.size();

        if (object_pool->CanAllocate(vertex_count, 0, constraints_count, 0, 1))
        {
            Composite<T>* composite = object_pool->AllocateComposites(1);
            Particle<T>* particles = object_pool->AllocateParticles(vertex_count);
            DistanceConstraint<T>* distance_constraints = object_pool->AllocateDistanceConstraints(constraints_count);

            *composite = Composite<T>();
            Particle<T>* particle = &particles[0];
            for (auto it = vertices.begin(); it != vertices.end(); ++it, ++particle)
            {
                math::Vector2d<T> actual_position = (*it) + position_offset;
                *particle = Particle<T>(actual_position);
                composite->AddParticle(particle);
            }

            DistanceConstraint<T>* distance_constraint = &distance_constraints[0];
            for(auto it = constraint_pairs.begin(); it != constraint_pairs.end(); ++it, ++distance_constraint)
            {
                *distance_constraint = DistanceConstraint<T>(&particles[it->first], &particles[it->second], stiffness);
                composite->AddConstraint(distance_constraint);
            }
            return composite;
        }
        return nullptr;
    }

    // TODO: Add Tire and Cloth
}


#endif /* defined(____basic_objects__) */

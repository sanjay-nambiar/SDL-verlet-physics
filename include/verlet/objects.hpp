
#ifndef ____basic_objects__
#define ____basic_objects__

#include <cmath>

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

    template<class T> Composite<T>* Tire(math::Vector2d<T>& origin, T radius, int segments,
        T spoke_stiffness, T tread_stiffness, ObjectPool<T>* object_pool)
    {
        if (object_pool->CanAllocate(segments + 1, 0, segments * 3, 0, 1))
        {
            T stride = (2 * M_PI)/segments;
            Composite<T>* composite = object_pool->AllocateComposites(1);
            Particle<T>* particles = object_pool->AllocateParticles(segments + 1);
            DistanceConstraint<T>* distance_constraints = object_pool->AllocateDistanceConstraints(segments * 3);

            // particles
            *composite = Composite<T>();
            Particle<T>* particle = &particles[0];
            for (int i=0; i < segments; ++i, ++particle)
            {
                T theta = i * stride;
                math::Vector2d<T> position(origin.x + cos(theta)*radius, origin.y + sin(theta)*radius);
                *particle = Particle<T>(position);
                composite->AddParticle(particle);
            }
            *particle = Particle<T>(origin);
            composite->AddParticle(particle);

            // constraints
            DistanceConstraint<T>* distance_constraint = &distance_constraints[0];
            for (int i = 0; i < segments; ++i)
            {
                *distance_constraint = DistanceConstraint<T>(&particles[i], &particles[(i+1) % segments],
                    tread_stiffness);
                composite->AddConstraint(distance_constraint);
                distance_constraint++;

                *distance_constraint = DistanceConstraint<T>(&particles[i], particle, spoke_stiffness);
                composite->AddConstraint(distance_constraint);
                distance_constraint++;

                *distance_constraint = DistanceConstraint<T>(&particles[i], &particles[(i+5) % segments],
                    tread_stiffness);
                composite->AddConstraint(distance_constraint);
                distance_constraint++;
            }
            return composite;
        }
        return nullptr;
    }

    template<class T> Composite<T>* Cloth(math::Vector2d<T> top_left, int width, int height, int segments,
        int pin_mod, T stiffness, ObjectPool<T>* object_pool)
    {
        int particle_count = segments * segments;
        int distance_constraints_count = 2 * segments * (segments - 1);
        int pin_constraints_count = (segments / pin_mod) + 1;

        if (object_pool->CanAllocate(particle_count, pin_constraints_count, distance_constraints_count, 0, 1))
        {
            Composite<T>* composite = object_pool->AllocateComposites(1);
            Particle<T>* particles = object_pool->AllocateParticles(particle_count);
            PinConstraint<T>* pin_constraints =  object_pool->AllocatePinConstraints(pin_constraints_count);
            DistanceConstraint<T>* distance_constraints =
                object_pool->AllocateDistanceConstraints(distance_constraints_count);

            T x_stride = width / segments;
            T y_stride = height / segments;

            *composite = Composite<T>();
            Particle<T>* particle = &particles[0];
            DistanceConstraint<T>* distance_constraint = &distance_constraints[0];
            PinConstraint<T>* pin_constraint = &pin_constraints[0];
            for (int y = 0; y < segments; ++y)
            {
                for (int x = 0; x < segments; ++x)
                {
                    T px = top_left.x + (x * x_stride);
                    T py = top_left.y + (y * y_stride);
                    math::Vector2d<T> position(px, py);

                    *particle = Particle<T>(position);
                    composite->AddParticle(particle);
                    // Add pin if required
                    if (y == 0 && ((x%pin_mod) == 0 || x == segments-1))
                    {
                        *pin_constraint = PinConstraint<T>(particle, particle->position);
                        composite->AddConstraint(pin_constraint);
                        pin_constraint++;
                    }
                    particle++;

                    if (x > 0)
                    {
                        int index = (y * segments) + x;
                        // (y*segments + x) and (y*segments + x-1)
                        *distance_constraint = DistanceConstraint<T>(&particles[index], &particles[index - 1],
                            stiffness);
                        composite->AddConstraint(distance_constraint);
                        distance_constraint++;
                    }

                    if (y > 0)
                    {
                        int index = (y * segments) + x;
                        // (y*segments + x) and ((y-1)*segments + x)
                        *distance_constraint = DistanceConstraint<T>(&particles[index], &particles[index - segments],
                            stiffness);
                        composite->AddConstraint(distance_constraint);
                        distance_constraint++;
                    }
                }
            }
            return composite;
        }
        return nullptr;
    }
}


#endif /* defined(____basic_objects__) */

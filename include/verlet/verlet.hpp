
#ifndef ____verlet__
#define ____verlet__

#include "math/vector2d.hpp"
#include "verlet/particle.hpp"
#include "verlet/constraints.hpp"
#include "simulation/object_pool.hpp"


namespace verlet
{
    template <class T>
    class Verlet
    {
        static_assert(std::is_floating_point<T>::value,
              "Verlet can be of floating point data types only");

        T _width;
        T _height;
        T _friction;
        T _ground_friction;
        math::Vector2d<T> _gravity;

        simulation::ObjectPool<T>* _object_pool;

        void RestrictToBounds(Particle<T>* particle)
        {
            T x = particle->position.x;
            T y = particle->position.y;

            if (particle->position.y > _height-1)
            {
                y = _height-1;
            }
            else if (particle->position.y < 0)
            {
                y = 0;
            }
            
            if (particle->position.x < 0)
            {
                x = 0;
            }
            else if (particle->position.x > _width-1)
            {
                x = width-1;
            }
            particle->position.Set(x, y);
        }

    public:
        const T& width;
        const T& height;
        const T& friction;
        const T& ground_friction;
        const math::Vector2d<T>& gravity;

        simulation::ObjectPool<T>* const & object_pool;
        
        Verlet(T width, T height, simulation::ObjectPool<T>* object_pool)
            : _object_pool(nullptr), width(_width), height(_height), friction(_friction),
            ground_friction(_ground_friction), gravity(_gravity), object_pool(_object_pool)
        {
            _width = width;
            _height = height;
            _gravity.Set(0, 0.2);
            _friction = 1;
            _ground_friction = 0.8;
            _object_pool = object_pool;
        }

        void Update(T step)
        {
            // TODO: Add ground friction and bounds checking

            Particle<T>* particle = _object_pool->particles;
            int particle_count = _object_pool->particle_count;
            for (int p = 0; p < particle_count; ++p, ++particle)
            {
                // calculate velocity
                math::Vector2d<T> velocity = (particle->position - particle->last_position) * friction;

                // apply ground_friction
                if (particle->position.y >= _height-1 && math::EuclideanLengthSquare<T>(velocity) > 0.000001)
                {
                    T m = math::EuclideanLength<T>(velocity);
                    velocity /= m;
                    velocity *= (m * ground_friction);
                }

                // save last good state
                particle->last_position = particle->position;

                // gravity
                particle->position += gravity;

                // inertia
                particle->position += velocity;
            }

            // relax
            T stepCoef = 1/step;
            for (int i = 0; i < step; ++i)
            {
                DistanceConstraint<T>* distance_constraint = _object_pool->distance_constraints;
                int constraint_count = _object_pool->distance_constraints_count;
                for (int c = 0; c < constraint_count; ++c, ++distance_constraint)
                {
                    distance_constraint->Relax(stepCoef);
                }

                AngularConstraint<T>* angular_constraint = _object_pool->angular_constraints;
                constraint_count = _object_pool->angular_constraints_count;
                for (int c = 0; c < constraint_count; ++c, ++angular_constraint)
                {
                    angular_constraint->Relax(stepCoef);
                }

                PinConstraint<T>* pin_constraint = _object_pool->pin_constraints;
                constraint_count = _object_pool->pin_constraints_count;
                for (int c = 0; c < constraint_count; ++c, ++pin_constraint)
                {
                    pin_constraint->Relax(stepCoef);
                }
            }

            // restrict to bounds
            particle = _object_pool->particles;
            for (int p = 0; p < particle_count; ++p, ++particle)
            {
                RestrictToBounds(particle);
            }
        }
    };
}

#endif /* defined(____verlet__) */

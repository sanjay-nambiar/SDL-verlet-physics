
#ifndef ____verlet__
#define ____verlet__


#include "vector2d.hpp"
#include "composite.hpp"


namespace physics
{
    template <class T>
    class Verlet
    {
        T _width;
        T _height;
        T _friction;
        T _ground_friction;
        math::Vector2d<T> _gravity;
        std::vector<Composite<T>*> _composites;

        void RestrictToBounds(Particle<T>* particle)
        {
            T x = particle->position.x;
            T y = particle->position.y;

            if (particle->position.y > height-1)
            {
                y = height-1;
            }
            
            if (particle->position.x < 0)
            {
                x = 0;
            }
            
            if (particle->position.x > width-1)
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
        const std::vector<Composite<T>*>& composites;
        
        Verlet(T width, T height)
        : width(_width), height(_height), friction(_friction), ground_friction(_ground_friction),
        gravity(_gravity), composites(_composites)
        {
            _gravity.Set(0, 0.2);
            _friction = 1;
            _ground_friction = 0.8;
        }

        void AddComposite(Composite<T>* composite)
        {
            _composites.push_back(composite);
        }
        
        void Update(T step)
        {
            for (auto c = composites.begin(); c != composites.end(); ++c)
            {
                for (auto p = (*c)->particles.begin(); p != (*c)->particles.end(); ++p)
                {
                    // calculate velocity
                    math::Vector2d<T> velocity = ((*p)->position - (*p)->last_position) * friction;

                    // apply ground_friction
                    if ((*p)->position.y >= height-1 && math::EuclideanLengthSquare<T>(velocity) > 0.000001)
                    {
                        T m = math::EuclideanLength<T>(velocity) ;
                        velocity /= m;
                        velocity *= (m * ground_friction);
                    }

                    // save last good state
                    (*p)->last_position = (*p)->position;

                    // gravity
                    (*p)->position += gravity;

                    // inertia
                    (*p)->position += velocity;
                }
            }

            // relax
            T stepCoef = 1/step;
            for (auto c = composites.begin(); c != composites.end(); ++c)
            {
                for (int i = 0; i < step; ++i)
                {
                    for (auto j = (*c)->constraints.begin(); j != (*c)->constraints.end(); ++j)
                    {
                        (*j)->Relax(stepCoef);
                    }
                }
            }

            // bounds checking
            for (auto c = composites.begin(); c != composites.end(); ++c)
            {
                for (auto p = (*c)->particles.begin(); p != (*c)->particles.end(); ++p)
                {
                    RestrictToBounds((*p));
                }
            }
        }
    };
}

#endif /* defined(____verlet__) */

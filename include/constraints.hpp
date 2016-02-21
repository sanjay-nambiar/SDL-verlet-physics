
#ifndef ____constraints__
#define ____constraints__

#include "vector2d.hpp"
#include "particle.hpp"

namespace physics
{
    template<class T>
    class Constraint
    {
    public:
        virtual void Relax(T stepCoeff) = 0;
    };


    template<class T>
    class PinConstraint: public Constraint<T>
    {
    public:
        physics::Particle<T>* particle;
        math::Vector2d<T> position;
        
        PinConstraint(physics::Particle<T>* particle, math::Vector2d<T>& position)
        : particle(particle), position(position)
        {
        }

        void Relax(T stepCoeff)
        {
            particle->position = position;
        }

        void operator=(const PinConstraint<T>& constraint) {
            particle = constraint.particle;
            position = constraint.position;
        }
        
    };


    template<class T>
    class DistanceConstraint: public Constraint<T>
    {
    public:
        physics::Particle<T>* p1;
        physics::Particle<T>* p2;
        T stiffness;
        T distance;

        DistanceConstraint(physics::Particle<T>* p1, physics::Particle<T>* p2, T stiffness)
        : p1(p1), p2(p2), stiffness(stiffness), distance(math::EuclideanLength<T>(p1->position - p2->position))
        {
        }

        void Relax(T stepCoeff)
        {
            math::Vector2d<T> normal = p1->position - p2->position;
            T normal_length_square = math::EuclideanLengthSquare(normal);
            normal *= (((distance*distance - normal_length_square)/normal_length_square) * stiffness * stepCoeff);
            p1->position += normal;
            p2->position -= normal;
        }
        
        void operator=(const DistanceConstraint<T>& constraint) {
            p1 = constraint.p1;
            p2 = constraint.p2;
            stiffness = constraint.stiffness;
            distance = constraint.distance;
        }
    };


    template<class T>
    class AngularConstraint: public Constraint<T>
    {
    public:
        physics::Particle<T>* p1;
        physics::Particle<T>* vertex;
        physics::Particle<T>* p2;
        T& stiffness;
        T& angle_in_radians;

        AngularConstraint(physics::Particle<T>* p1, physics::Particle<T>* vertex, physics::Particle<T>* p2, T stiffness)
        : p1(p1), p2(p2), vertex(vertex), stiffness(stiffness), 
            angle_in_radians(math::Angle(vertex->position, p1->position, p2->position))
        {
        }

        void Relax(T stepCoeff)
        {
            T angle = math::Angle(vertex->position, p1->position, p2->position);
            T diff = angle - angle_in_radians;

            if (diff <= -M_PI)
            {
                diff += 2*M_PI;
            }
            else if (diff >= M_PI)
            {
                diff -= 2*M_PI;
            }

            diff *= stepCoeff * stiffness;
            p1->position = math::Rotate(p1->position, vertex->position, diff);
            p2->position = math::Rotate(p2->position, vertex->position, -diff);
            vertex->position = math::Rotate(vertex->position, p1->position, diff);
            vertex->position = math::Rotate(vertex->position, p2->position, -diff);
        }
        
        void operator=(const AngularConstraint<T>& constraint) {
            p1 = constraint.p1;
            vertex = constraint.vertex;
            p2 = constraint.p2;
            stiffness = constraint.stiffness;
            angle_in_radians = constraint.angle_in_radians;
        }
    };
}


#endif /* defined(____constraints__) */

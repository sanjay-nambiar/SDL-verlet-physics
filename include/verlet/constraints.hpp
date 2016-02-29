
#ifndef ____constraints__
#define ____constraints__

#include "math/vector2d.hpp"
#include "verlet/particle.hpp"

namespace verlet
{
    template<class T>
    class Constraint
    {
        static_assert(std::is_floating_point<T>::value,
                      "Constraint can be of floating point data types only");

    public:
        virtual void Relax(T stepCoeff) = 0;
    };

    template<class T>
    class PinConstraint: public Constraint<T>
    {
        static_assert(std::is_floating_point<T>::value,
              "PinConstraint can be of floating point data types only");

        Particle<T>* _particle;
        math::Vector2d<T> _position;
    public:
        Particle<T>* const & particle;
        const math::Vector2d<T>& position;

        PinConstraint() : particle(_particle), position(_position)
        {
            _particle = nullptr;
            _position = math::Vector2d<T>(0, 0);
        }
        
        PinConstraint(Particle<T>* particle, math::Vector2d<T>& position) : particle(_particle), position(_position)
        {
            _particle = particle;
            _position = position;
        }

        void Relax(T stepCoeff)
        {
            _particle->position = _position;
        }

        void operator=(const PinConstraint<T>& constraint) {
            _particle = constraint.particle;
            _position = constraint.position;
        }
    };


    template<class T>
    class DistanceConstraint: public Constraint<T>
    {
        static_assert(std::is_floating_point<T>::value,
              "DistanceConstraint can be of floating point data types only");

        Particle<T>* _particle1;
        Particle<T>* _particle2;
        T _stiffness;
        T _distance;
    public:
        Particle<T>* const & particle1;
        Particle<T>* const & particle2;
        const T& stiffness;
        const T& distance;

        DistanceConstraint() : particle1(_particle1), particle2(_particle2), stiffness(_stiffness), distance(_distance)
        {
            _particle1 = nullptr;
            _particle2 = nullptr;
            _stiffness = 1;
            _distance = 0;
        }

        DistanceConstraint(Particle<T>* particle1, Particle<T>* particle2, T stiffness)
        : particle1(_particle1), particle2(_particle2), stiffness(_stiffness), distance(_distance)
        {
            _particle1 = particle1;
            _particle2 = particle2;
            _stiffness = stiffness;
            _distance = math::EuclideanLength<T>(particle1->position - particle2->position);
        }

        void Relax(T stepCoeff)
        {
            math::Vector2d<T> normal = particle1->position - particle2->position;
            T normal_length_square = math::EuclideanLengthSquare(normal);
            normal *= (((distance*distance - normal_length_square)/normal_length_square) * stiffness * stepCoeff);
            particle1->position += normal;
            particle2->position -= normal;
        }
        
        void operator=(const DistanceConstraint<T>& constraint) {
            _particle1 = constraint.particle1;
            _particle2 = constraint.particle2;
            _stiffness = constraint.stiffness;
            _distance = constraint.distance;
        }
    };


    template<class T>
    class AngularConstraint: public Constraint<T>
    {
        static_assert(std::is_floating_point<T>::value,
              "AngularConstraint can be of floating point data types only");

        Particle<T>* _particle1;
        Particle<T>* _vertex;
        Particle<T>* _particle2;
        T _stiffness;
        T _angle;

    public:
        Particle<T>* const & particle1;
        Particle<T>* const & vertex;
        Particle<T>* const & particle2;
        const T& stiffness;
        const T& angle_in_radians;

        AngularConstraint() : particle1(_particle1), vertex(_vertex), particle2(_particle2),
            stiffness(_stiffness), angle_in_radians(_angle)
        {
            _particle1 = nullptr;
            _particle2 = nullptr;
            _vertex = nullptr;
            _stiffness = 1;
            _angle = 0;
        }

        AngularConstraint(Particle<T>* particle1, Particle<T>* vertex, Particle<T>* particle2, T stiffness)
        : particle1(_particle1), vertex(_vertex), particle2(_particle2), stiffness(_stiffness),
            angle_in_radians(_angle)
        {
            _particle1 = particle1;
            _particle2 = particle2;
            _vertex = vertex;
            _stiffness = stiffness;
            _angle = math::Angle(vertex->position, particle1->position, particle2->position);
        }

        void Relax(T stepCoeff)
        {
            T angle = math::Angle(vertex->position, particle1->position, particle2->position);
            T diff = angle - angle_in_radians;

            if (diff <= -M_PI)
            {
                diff += (2 * M_PI);
            }
            else if (diff >= M_PI)
            {
                diff -= (2 * M_PI);
            }

            diff *= stepCoeff * stiffness;
            particle1->position = math::Rotate(particle1->position, vertex->position, diff);
            particle2->position = math::Rotate(particle2->position, vertex->position, -diff);
            vertex->position = math::Rotate(vertex->position, particle1->position, diff);
            vertex->position = math::Rotate(vertex->position, particle2->position, -diff);
        }
        
        void operator=(const AngularConstraint<T>& constraint) {
            _particle1 = constraint.particle1;
            _vertex = constraint.vertex;
            _particle2 = constraint.particle2;
            _stiffness = constraint.stiffness;
            _angle = constraint.angle_in_radians;
        }
    };
}


#endif /* defined(____constraints__) */

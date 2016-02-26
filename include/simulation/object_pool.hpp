
#ifndef ____object_pool__
#define ____object_pool__

#include "verlet/particle.hpp"
#include "verlet/constraints.hpp"
#include "verlet/composite.hpp"

namespace simulation
{
	using namespace verlet;

	template <class T>
	class ObjectPool
	{
		const int MAX_PARTICLES;
		const int MAX_PIN_CONSTRAINTS;
		const int MAX_DISTANCE_CONSTRAINTS;
		const int MAX_ANGULAR_CONSTRAINTS;
		const int MAX_COMPOSITES;

		int _particle_count;
		Particle<T>* _particles;

		int _pin_constraints_count;
		PinConstraint<T>* _pin_constraints;

		int _distance_constraints_count;
		DistanceConstraint<T>* _distance_constraints;

		int _angular_constraints_count;
		AngularConstraint<T>* _angular_constraints;

		int _composite_count;
		Composite<T>* _composites;

	public:

		Particle<T>* const & particles;
		const int& particle_count;

		PinConstraint<T>* const & pin_constraints;
		const int& pin_constraints_count;

		DistanceConstraint<T>* const & distance_constraints;
		const int& distance_constraints_count;

		AngularConstraint<T>* const & angular_constraints;
		const int& angular_constraints_count;

		Composite<T>* const & composites;
		const int& composite_count;


		ObjectPool(int max_particles, int max_pin_constraints, int max_distance_constraints, 
			int max_angular_constraints, int max_composites)
			: MAX_PARTICLES(max_particles), MAX_PIN_CONSTRAINTS(max_pin_constraints),
			MAX_DISTANCE_CONSTRAINTS(max_distance_constraints), MAX_ANGULAR_CONSTRAINTS(max_angular_constraints),
			MAX_COMPOSITES(max_composites), particles(_particles), particle_count(_particle_count),
			pin_constraints(_pin_constraints), pin_constraints_count(_pin_constraints_count),
			distance_constraints(_distance_constraints), distance_constraints_count(_distance_constraints_count),
			angular_constraints(_angular_constraints), angular_constraints_count(_angular_constraints_count),
			composites(_composites), composite_count(_composite_count)
		{
			_particle_count = 0;
			_pin_constraints_count = 0;
			_distance_constraints_count = 0;
			_angular_constraints_count = 0;
			_composite_count = 0;

			_particles = new Particle<T>[MAX_PARTICLES];
			_pin_constraints = new PinConstraint<T>[MAX_PIN_CONSTRAINTS];
			_distance_constraints = new DistanceConstraint<T>[MAX_DISTANCE_CONSTRAINTS];
			_angular_constraints = new AngularConstraint<T>[MAX_ANGULAR_CONSTRAINTS];
			_composites = new Composite<T>[MAX_COMPOSITES];
		}

		~ObjectPool() {
			delete [] _composites;
			delete [] _angular_constraints;
			delete [] _distance_constraints;
			delete [] _pin_constraints;
			delete [] _particles;
		}

		bool CanAllocate(int particles, int pin_constraints, int distance_constraints, int angular_constraints,
			int composites)
		{
			return (_particle_count + particles <= MAX_PARTICLES)
				&& (_pin_constraints_count + pin_constraints <= MAX_PIN_CONSTRAINTS)
				&& (_distance_constraints_count + distance_constraints <= MAX_DISTANCE_CONSTRAINTS)
				&& (_angular_constraints_count + angular_constraints <= MAX_ANGULAR_CONSTRAINTS)
				&& (_composite_count + composites <= MAX_COMPOSITES);
		}

		Particle<T>* AllocateParticles(int count)
		{
			if (_particle_count + count > MAX_PARTICLES)
			{
				return nullptr;
			}
			_particle_count += count;
			return _particles + (_particle_count - count);
		}

		PinConstraint<T>* AllocatePinConstraints(int count)
		{
			if (_pin_constraints_count + count > MAX_PIN_CONSTRAINTS)
			{
				return nullptr;
			}
			_pin_constraints_count += count;
			return _pin_constraints + (_pin_constraints_count - count);
		}

		DistanceConstraint<T>* AllocateDistanceConstraints(int count)
		{
			if (_distance_constraints_count + count > MAX_DISTANCE_CONSTRAINTS)
			{
				return nullptr;
			}
			_distance_constraints_count += count;
			return _distance_constraints + (_distance_constraints_count - count);
		}

		AngularConstraint<T>* AllocateAngularConstraints(int count)
		{
			if (_angular_constraints_count + count > MAX_ANGULAR_CONSTRAINTS)
			{
				return nullptr;
			}
			_angular_constraints_count += count;
			return _angular_constraints + (_angular_constraints_count - count);
		}

		Composite<T>* AllocateComposites(int count)
		{
			if (_composite_count + count > MAX_COMPOSITES)
			{
				return nullptr;
			}
			_composite_count += count;
			return _composites + (_composite_count - count);
		}
	};
}

#endif /* defined(____object_pool__) */

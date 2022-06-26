#include <assert.h>
#include <algorithm> 	// For std::swap()
#include <utility>		// For std::exchange()

#include "axial.hpp"

#include <iostream>

namespace axial {
	// Constructor
	vector::vector(int q, int r, int s) 
	: q_{q}
	, r_{r}
	, s_{s}
	{
		assert(q + r + s == 0); // REMOVE
	}

	// Paperwork momement - IE RULE OF 5
	vector::vector(vector const& vec)				// Copy Constructor
	: vector::vector(vec.q(), vec.r(), vec.s())
	{}

	vector::vector(vector&& vec)					// Move Constructor
	: q_{vec.q_}
	, r_{vec.r_}
	, s_{vec.s_}
	{}
	
	// Copy Assignment
	auto vector::operator=(vector const& vec) -> vector& {
		// Copy and Swap idiom
		auto deleted_at_the_end_of_scope = vector(vec);
		swap(*this, deleted_at_the_end_of_scope);
		return *this;
	}

	auto vector::operator=(vector&& vec) -> vector& {
		if (*this != vec) {
			swap(*this, vec);
		}
		return *this;
	} 	

	// Simple operators
	auto vector::operator+=(vector const& vec) -> vector& {
		// Do normal vector addition
		this->q_ += vec.q_;
		this->r_ += vec.r_;
		this->s_ += vec.s_;
		return *this;
	}

	auto vector::operator-=(vector const& vec) -> vector& {
		// Do normal vector subtraction
		this->q_ -= vec.q_;
		this->r_ -= vec.r_;
		this->s_ -= vec.s_;
		return *this;
	}

	auto vector::operator*=(int scalar) -> vector& {
		// Do normal vector multiple
		this->q_ *= scalar;
		this->r_ *= scalar;
		this->s_ *= scalar;
		return *this;
	}


	// Helper functions
	auto vector::q() const& -> int {
		return this->q_;
	}
	auto vector::r() const& -> int {
		return this->r_;
	}
	auto vector::s() const& -> int {
		return this->s_;
	}

	auto vector::uq() -> vector {
		return vector{ 0, 1,-1};
	}
	auto vector::ur() -> vector {
		return vector{-1, 0, 1};
	} 
	auto vector::us() -> vector {
		return vector{ 1,-1, 0};
	}
	auto vector::uqr() -> vector {
		return vector{-1, 1, 0};
	}
	auto vector::urs() -> vector {
		return vector{ 0,-1, 1};
	} 
	auto vector::usq() -> vector {
		return vector{ 1, 0,-1};
	}

	auto vector::unit_directions() -> std::vector<vector> const {
		// Unit circle, anticlockwise directions
		return std::vector<vector> {
			axial::vector::uq(),
			axial::vector::uqr(),
			axial::vector::ur(),
			axial::vector::urs(),
			axial::vector::us(),
			axial::vector::usq()
		};
	};

	// Unit circle, anticlockwise directions
	// axial::vector{ 0, 1,-1}, // q
	// axial::vector{-1, 0, 1}, // r
	// axial::vector{ 1,-1, 0}, // s
	// axial::vector{-1, 1, 0}, // qr
	// axial::vector{ 0,-1, 1}, // rs
	// axial::vector{ 1, 0,-1}, // sq
}




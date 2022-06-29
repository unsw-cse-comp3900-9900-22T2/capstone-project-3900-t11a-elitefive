#include <assert.h>
#include <algorithm> 	// For std::swap()
#include <utility>		// For std::exchange()
#include <cmath>		// For std::abs()

#include "axial.hpp"

#include <iostream>

namespace axial {
	// Constructor
	vector::vector(int q, int r, int s) 
	: q_{q}
	, r_{r}
	, s_{s}
	{
		assert(q + r + s == 0); // TODO: REMOVE
	}

	vector::vector(int flatten_index)
	:q_{5}
	,r_{5}
	,s_{5}
	{
		auto calculate_q = [](int index) {
			if (index <=  4) return -4;
			if (index <= 10) return -3;
			if (index <= 17) return -2;
			if (index <= 25) return -1;
			if (index <= 34) return  0;
			if (index <= 42) return  1;
			if (index <= 49) return  2;
			if (index <= 55) return  3;
							 return  4;
		};
		// auto calculate_q = [](int index) {
		// 	for (int q = -4; q < 5; ++q) {
		// 		if (index < (((q + 9) * (q + 10) / 2) - 10 - (q > 0 ? q * (q + 1) : 0))) {
		// 		  return q;
		// 		}
		// 	}
		// 	return -1;
		// };
		q_ = calculate_q(flatten_index);
		s_ = ((q_ + 8) * (q_ + 9)) / 2 - 6 - flatten_index - ((q_ > 0) ? q_ * q_ : 0);
		r_ = -q_-s_;
	}


	// Paperwork moment - IE RULE OF 5
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
		return q_;
	}
	auto vector::r() const& -> int {
		return r_;
	}
	auto vector::s() const& -> int {
		return s_;
	}

	auto vector::distance() const& -> int {
		return (std::abs(q_) + std::abs(r_) + std::abs(s_)) / 2;
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

	auto vector::basis_vectors() -> std::vector<vector> const {
		return std::vector<vector> { 
			axial::vector::uq(), 
			axial::vector::ur(), 
			axial::vector::us() 
		};
	}


	// Unit circle, anticlockwise directions
	// axial::vector{ 0, 1,-1}, // q
	// axial::vector{-1, 0, 1}, // r
	// axial::vector{ 1,-1, 0}, // s
	// axial::vector{-1, 1, 0}, // qr
	// axial::vector{ 0,-1, 1}, // rs
	// axial::vector{ 1, 0,-1}, // sq
}




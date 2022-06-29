#ifndef YAVALATH_AXIAL_HPP
#define YAVALATH_AXIAL_HPP

#include <iostream>
#include <vector>

namespace axial {
	class vector {
		public:
			// Gives you the flatten hex index when given a vector
			static auto index(vector const& vec) -> int {
				return (((vec.q() + 8)*(vec.q() + 9)) / 2) - (6 + vec.s()) - ((vec.q() > 0) ? vec.q() * vec.q() : 0);
			}

		private:
			int q_;
			int r_;
			int s_; // Coordinates
		public:
			// Unit vectors
			static auto uq() -> vector;
			static auto ur() -> vector; 
			static auto us() -> vector; 
			static auto uqr() -> vector;
			static auto urs() -> vector;
			static auto usq() -> vector;
			static auto basis_vectors() -> std::vector<vector> const;
			static auto unit_directions() -> std::vector<vector> const;

			// Constructors
			vector(int q, int r, int s);					// Regular Constructors
			vector(int flatten_index);						// 
			vector(vector const& vec);						// Copy Constructor
			vector(vector&& vec);							// Move construtor
			auto operator=(vector const& vec) -> vector&;	// Copy Assignment
			auto operator=(vector&& vec) -> vector&; 		// Move assignment
			
			auto q() const& -> int;
			auto r() const& -> int;
			auto s() const& -> int;
			auto distance() const& -> int;

			~vector() = default;


			auto operator+=(vector const& vec) -> vector&;
			auto operator-=(vector const& vec) -> vector&;
			auto operator*=(int scalar) -> vector&;

			// Operators
			friend auto operator<<(std::ostream& os, vector const& ax) -> std::ostream& {
				os << "qrs: " << ax.q_ << ", " << ax.r_ << ", " << ax.s_;
				return os;
			}

			friend auto operator+(vector const& v1, vector const& v2) -> vector {
				auto addition_res = vector(v1);
				addition_res += v2;
				return addition_res;
			}

			friend auto operator-(vector const& v1, vector const& v2) -> vector {
				auto addition_res = vector(v1);
				addition_res -= v2;
				return addition_res;
			}

			friend auto operator*(vector const& v1, int scalar) -> vector {
				auto addition_res = vector(v1);
				addition_res *= scalar;
				return addition_res;
			}

			friend auto operator==(vector const& v1, vector const& v2) -> bool {
				if (v1.q_ != v2.q_) return false;
				if (v1.r_ != v2.r_) return false;
				if (v1.s_ != v2.s_) return false;
				return true;
			}

			friend auto operator!=(vector const& v1, vector const& v2) -> bool {
				return !(v1 == v2);
			}

			friend auto swap(vector& v1, vector& v2) -> void {
				std::swap(v1.q_, v2.q_);
				std::swap(v1.r_, v2.r_);
				std::swap(v1.s_, v2.s_);
			}
	};
}

#endif
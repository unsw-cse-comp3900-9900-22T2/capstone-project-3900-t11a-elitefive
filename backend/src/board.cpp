#include <iostream>
#include <vector>

#include "axial.hpp"
#include "hexagon.hpp"

int main(void) {

	auto directions = std::vector<axial::vector>{
		axial::vector::uq(),
		axial::vector::uqr(),
		axial::vector::ur(),
		axial::vector::urs(),
		axial::vector::us(),
		axial::vector::usq(),
	};

	auto vec = axial::vector(0,0,0);
	// vec += axial::vector::uq();
	for (auto i : directions) {
		vec += i;
	}

	std::cout << vec.q() << vec.r() << vec.s() << '\n';

	// auto hex = Hexagon(vec);
	// std::cout << vec << '\n';
	// std::cout << hex << '\n';
	return 0;
}
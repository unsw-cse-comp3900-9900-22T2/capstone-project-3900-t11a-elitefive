#ifndef YAVALATH_HEXAGON_HPP
#define YAVALATH_HEXAGON_HPP

#include <ostream>
#include "axial.hpp"

class Hexagon {
	private:
		axial::vector position_;
		int lookup_index_;
		int used_; // What player is occupying
	public:
		Hexagon(axial::vector pos);
		Hexagon(axial::vector pos, int index);
		Hexagon(axial::vector pos, int index, int used);

		// auto hex_add(axial::vector dir) -> Hexagon {
		// 	return Hexagon(vector_)
		// }

		
		friend auto operator<<(std::ostream& os, Hexagon const& hex) -> std::ostream& {
			os << "Hex Pos: [" << hex.position_ << "]\n\t" << "Index: " << hex.lookup_index_ << " Used: " << hex.used_;
			return os;
		}
};

#endif
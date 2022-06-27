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
		static int const EMPTY = -2; // Denotes tile is out of play

		Hexagon(axial::vector pos);
		Hexagon(axial::vector pos, int index);
		Hexagon(axial::vector pos, int index, int used);

		// Determine whether this hexagon is in play
		auto valid() const -> bool;

		// Simple Geters
		auto getPosVec() const -> axial::vector {return Hexagon::position_;};
		auto getPlayer() const -> int {return used_;};

		// Setters
		auto setTile(int player) -> void;

		
		friend auto operator<<(std::ostream& os, Hexagon const& hex) -> std::ostream& {
			os << "Hex Pos: [" << hex.position_ << "]\n\t" << "Index: " << hex.lookup_index_ << " Used: " << hex.used_;
			return os;
		}
};

#endif
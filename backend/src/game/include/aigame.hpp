#ifndef YAVALATH_AIGAME_HPP
#define YAVALATH_AIGAME_HPP

#include <string.h>
#include "basegame.hpp"

class AIGame : public BaseGame {
    private:
        bool terminal_;
    public:
        AIGame(int nplayers);
        AIGame(int nplayers, bool terminal);

        ~AIGame() {}

        auto play(std::string move) -> bool override;	// Does NOT provide error checking
		auto play(int index) -> bool override;			// Does NOT provide error checking
        auto unplay(int index) -> void;
        
        auto terminal() -> bool;

    private:
        auto end_turn() -> void;
        auto undo_turn() -> void;

};

#endif
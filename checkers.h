//Adam Dubsky
//Final Project Checkers
//April 2021
#ifndef CHECKERS_H
#define CHECKERS_H
#include "game.h"
#include "space.h"
namespace main_savitch_14
{
    class Checkers:public main_savitch_14::game
    {
        public:
            //default contructor
            Checkers();
            //The next player makes a specified move
    	    void make_move(const std::string& move);
            //Restarts the game at the beginning
    	    void restart();
            //Display the status of the current game
            void display_status() const;
            //Returns true if the given move is legal for the next player
            bool is_legal(const std::string& move) const;
            //Return true if the current game is finished
            bool is_game_over() const;
            //Determines who wins the game
            who winning() const;

            //helper functions

            //checks to see if the jump move is legal or not
            bool is_jump_legal_white(const std::string& move) const;
            bool is_jump_legal_blue(const std::string& move) const;
            //checks to see if the jump move is legal or not for a king
            bool is_jump_legal_white_king(const std::string& move) const;
            bool is_jump_legal_blue_king(const std::string& move) const;
            //used in make_move function to remove pieces when jumped
            void remove_white_piece();
            void remove_blue_piece();


            //stubs of the purely virtual functions

            // Return a pointer to a copy of myself
            game* clone() const;
            // Compute all the moves that the next player can make
            void compute_moves(std::queue<std::string>& moves) const;
            // Evaluate a board position
            // NOTE: positive values are good for the computer.
            int evaluate() const;
        private:
            Space board[8][8];
            size_t bluePieces;
            size_t whitePieces;
    };
}
#endif //CHECKERS_H
//Adam Dubsky
//Final Project Checkers
//April 2021

#include "space.h"
#include "colors.h"
namespace main_savitch_14
{
    //default constructor
    Space::Space() 
    {
        color_of_space = 'e';
    }
    //check to see if the piece is empty
    bool Space::is_empty() const
    {
        return (color_of_space == 'e');
    }
    //check to see if the piece is unplayable
    bool Space::is_unplayable() const
    {
        return (color_of_space == 'u');
    }
    //check to see if the piece is blue
    bool Space::is_blue() const
    {
        return (color_of_space == 'b');
    }
    //check to see if the piece is white
    bool Space::is_white() const
    {
        return (color_of_space == 'w');
    }
    //check to see if the piece is a blue king
    bool Space::is_blue_king() const
    {
        return (color_of_space == 'B');
    }
    //check to see if the piece is a white king
    bool Space::is_white_king() const
    {
        return (color_of_space == 'W');
    }
    //make the space empty of pieces
    void Space::make_empty() 
    {
        color_of_space = 'e';
    }
    //make the space unplayable
    void Space::make_unplayable()
    {
        color_of_space = 'u';
    }
    //make the piece blue
    void Space::make_blue()
    {
        color_of_space = 'b';
    }
    //make the piece white
    void Space::make_white()
    {
        color_of_space = 'w';
    }
    //make the piece a blue king
    void Space::make_blue_king()
    {
        color_of_space = 'B';
    }
    //make the piece a white king
    void Space::make_white_king()
    {
        color_of_space = 'W';
    }
    //output function
    void Space::output(ostream& outs)const
    {
        if(color_of_space == 'e')
        {
            cout << B_BLACK << "     " << RESET;
        }
        else if(color_of_space == 'u')
        {
            cout << B_WHITE << "     " << RESET;
        }
        else if(color_of_space == 'b')
        {
            cout << B_BLACK << BLUE << "  b  " << RESET;
        }
        else if(color_of_space == 'w')
        {
            cout << B_BLACK << WHITE << "  w  " << RESET;
        }
        else if(color_of_space == 'B')
        {
            cout << B_BLACK << BLUE << "  B  " << RESET;
        }
        else if(color_of_space == 'W')
        {
            cout << B_BLACK << WHITE << "  W  " << RESET;
        }
        else
        {
            cout << "-----------------------------" << endl;
            cout << "ERROR IN THE OUTPUT FUNCTION!" << endl;
            cout << "-----------------------------" << endl;
        }
        
    }
    //overloaded << operator
    ostream& operator <<(ostream& outs, const Space& s)
    {
        s.output(outs);
        return outs;
    }
}
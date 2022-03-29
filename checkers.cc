/**
* @file checkers.cc
* @brief file that contains all the functions for the checkers class
* @author Adam Dubsky
*/

#include "game.h"
#include "checkers.h"

///g++ -Wall checkers.cc checkers.h colors.h game.cc game.h samplemain.cc space.cc space.h
/// testing three slashes for doxygen
namespace main_savitch_14
{
    //default contructor
    Checkers::Checkers()
    {
        game::restart();
        bluePieces = 12;
        whitePieces = 12;
    }

    ///Restarts the game at the beginning
    void Checkers::restart()
    {
        //both of these loops are making the board all empty; loop is vertical
        for(size_t i = 0; i < 8; i++)
        {
            //this loop is for horizontal
            for(size_t j = 0; j < 8; j++)
            {
                board[i][j].make_empty();
            }
        }
        //both of these loops are making the board have blue/white spaces; loop is vertical
        for(size_t i = 0; i < 8; i++)
        {
            //this loop is for horizontal
            for(size_t j = 0; j < 8; j++)
            {
                if((i + j) % 2 == 0)
                {
                    board[i][j].make_unplayable();
                }
                //add sum of the two numbers in 2D array, if odd, it places it
                else if(i < 3 && (i + j) % 2 == 1)
                {
                    board[i][j].make_blue();
                }
                else
                {
                    board[i][j].make_white();
                }
            }
            //handles the two middle spaces that have no space; this algorithm is odd, but it works
            if(i == 2)
            {
                //must add 1 to i b/c it is doing the next one (use new temp variable)
                int tempI = i + 1;
                //loop that is going to go through and fill in the two middle sections
                for(size_t middleSections = 0; middleSections < 2; middleSections++)
                {
                    for(size_t j = 0; j < 8; j++)
                    {
                        if((tempI + j) % 2 == 0)
                        {
                            board[tempI][j].make_unplayable();
                        }
                        else if((tempI + j) % 2 == 1)
                        {
                            board[tempI][j].make_empty();
                        }
                    } //closes the loop that is going through the row for middle section
                    tempI = tempI + 1;
                } //closes the loop dealing with the middle section
                //the skip over the middle two spots that start with no pieces
                i = i + 2;
            } //closes if(i == 2)
        } //closes the for loop making colored spaces
    }

    //Display the status of the current game
    void Checkers::display_status() const
    {
        //arrays for the labels
        char numberLabel[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};
        char letterLabel[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
        cout << " ";
        //print the letter labels at the top
        for(size_t i = 0; i < 8; i++)
        {
            cout << "   ";
            cout << letterLabel[i];
            cout << " ";
        }
        cout << endl;
        //print the rest of the board
        for(size_t i = 0; i < 8; i++)
        {
            cout << numberLabel[i] << " ";
            for(size_t j = 0; j < 8; j++)
            {
                cout << board[i][j];
            }
            cout << endl;
        }
        cout << endl;
        cout << WHITE << "White Pieces Left: " << whitePieces << RESET << endl;
        cout << BLUE << "Blue Pieces Left: " << bluePieces << RESET << endl;
        cout << "______________________________________________" << endl;
        //print out which player's turn it is
        if(next_mover() == HUMAN)
        {
            cout << WHITE << "White's Turn (To Forfeit, Type 'Forfeit')" << RESET << endl;
        }
        else if(next_mover() == COMPUTER)
        {
            cout << BLUE << "Blue's Turn (To Forfeit, Type 'Forfeit')" << RESET << endl;
        }
        cout << endl;
    }

    //Returns true if the given move is legal for the next player
    bool Checkers::is_legal(const std::string& move) const
    {
        //forfeit statement
        if(move == "Forfeit")
        {
            return 1;
        }
        //checks the bounds of what the user entered to see if it goes off the board
        for(size_t i = 0; i < move.length(); i++)
        {
            //used for columns
            if(i % 2 == 0)
            {
                //returns false if any char in it does not fit
                if(move[i] < 'A' || move[i] > 'H')
                {
                    return 0;
                }
            }
            //used for rows
            if(i % 2 == 1)
            {
                //returns false if any char in it does not fit
                if(move[i] < '1' || move[i] > '8')
                {
                    return 0;
                }
            }
        }

        //get the rows and column numbers with the ASCII values
        int row = 0, column = 0, row2 = 0, column2 = 0;
        column = (int) move[0] - 'A';
        row = (int) move[1] - '1';
        column2 = (int) move[2] - 'A';
        row2 = (int) move[3] - '1';

        //if spot is unplayable, (white space)
        if(board[row][column].is_unplayable() || board[row2][column2].is_unplayable())
        {
            return 0;
        }

        //Player 1 (White)
        if(next_mover() == HUMAN)
        {
            //if it is a regular white piece
            if(board[row][column].is_white())
            {
                //if both spots are white
                if(board[row][column].is_white() && (board[row2][column2].is_white() || board[row2][column2].is_white_king()))
                {
                    return 0;
                }
                //if the first spot is white and the next spot is empty
                if(board[row][column].is_white() && board[row2][column2].is_empty())
                {
                    //statement checking if the area is 1 up and 1 to the left or one to the right
                    if(row2 - row == -1 && (column2 - column == 1 || column2 - column == -1))
                    {
                        return 1;
                    }
                    else
                    {
                        //go to the jump helper function and check to see if the move is a jump
                        return is_jump_legal_white(move);
                    }
                    //should never occur, but will if error happens
                    cout << "Error for White Player" << endl;
                    return 0;
                }
            }
            //if it is a white king
            else if(board[row][column].is_white_king())
            {
                //if both spots are white kings
                if(board[row][column].is_white_king() && (board[row2][column2].is_white() || board[row2][column2].is_white_king()))
                {
                    return 0;
                }
                //if the first spot is a white king and the next spot is empty
                if(board[row][column].is_white_king() && board[row2][column2].is_empty())
                {
                    //statement that checks if it is moving up or down 1 space and left or right 1 space
                    if((row2 - row == -1 || row2 - row == 1) && (column2 - column == 1 || column2 - column == -1))
                    {
                        return 1;
                    }
                    else
                    {
                        //go to the jump helper function and check to see if the move is a jump
                        return is_jump_legal_white_king(move);
                    }
                    //should never occur, but will if error happens
                    cout << "Error for White Player" << endl;
                    return 0;
                }
            }
            //occurs if their is an error
            //cout << "Error"<< endl;
            return 0;
        }   
        
        //Player 2 (Blue)
        else if(next_mover() == COMPUTER)
        {
            //if it is a regular white piece
            if(board[row][column].is_blue())
            {
                //if both spots are blue
                if(board[row][column].is_blue() && board[row2][column2].is_blue())
                {
                    return 0;
                }
                //if the first spot is blue and the next spot is empty
                if(board[row][column].is_blue() && board[row2][column2].is_empty())
                {
                    //if the distance is one down or 1 to the left or right
                    if(row2 - row == 1 && ((column2 - column == 1 || column2 - column == -1)))
                    {
                        return 1;
                    }
                    //checks the jump helper function to check if it is a jump statement
                    else
                    {
                        return is_jump_legal_blue(move);
                    }   
                }
                //should never occur, but will if error happens
             //   cout << "Error for Blue Player" << endl;
                return 0;
            }
            else if(board[row][column].is_blue_king())
            {
                //if both spots are blue kings
                if(board[row][column].is_blue_king() && (board[row2][column2].is_blue() || board[row2][column2].is_blue_king()))
                {
                    return 0;
                }
                //if the first spot is a white king and the next spot is empty
                if(board[row][column].is_blue_king() && board[row2][column2].is_empty())
                {
                    //statement that checks if it is moving up or down 1 space and left or right 1 space
                    if((row2 - row == -1 || row2 - row == 1) && (column2 - column == 1 || column2 - column == -1))
                    {
                        return 1;
                    }
                    else
                    {
                        //go to the jump helper function and check to see if the move is a jump
                        return is_jump_legal_blue_king(move);
                    }
                    //should never occur, but will if error happens
                    //cout << "Error for Blue Player" << endl;
                    return 0;
                }
            }
            //occurs if their is an error
         //   cout << "Error"<< endl;
            return 0;
        }
        //used to show not your turn or if error; should never show
        cout << "ERROR" << endl;
        return 0;
    }

    //The next player makes a specified move
    void Checkers::make_move(const std::string& move)
    {
        //statement checking if Forfeit is typed
        if(move == "Forfeit")
        {
            if(next_mover() == HUMAN)
            {
                whitePieces = 0;
            }
            else if(next_mover() == COMPUTER)
            {
                bluePieces = 0;
            }
        }

        //get the rows and column numbers with the ASCII values
        int row = 0, column = 0, row2 = 0, column2 = 0;
        column = (int) move[0] - 'A';
        row = (int) move[1] - '1';
        column2 = (int) move[2] - 'A';
        row2 = (int) move[3] - '1';

        //Player 1 (White)
        if(next_mover() == HUMAN)
        {
            //if the starting piece is a normal white piece
            if(board[row][column].is_white())
            {
                //go through the loop that is used based off of how long the string is (if it is a jump or normal move)
                for(size_t i = 1; i < move.length() / 2; i++)
                {
                    //will make the original spot empty and the next spot white
                    board[row][column].make_empty();
                    //make the final spot a white king if it is on the other side of the board
                    if(row2 == 0 && (column2 == 1 || column2 == 3 || column2 == 5 || column2 == 7))
                    {
                        board[row2][column2].make_white_king();
                    }
                    //if not, make the spot a regular white piece
                    else
                    {
                        board[row2][column2].make_white();
                    }
                    //statement used if it is jump to the left
                    if((row2 - row == -2 && column2 - column == -2) 
                        && (board[row - 1][column - 1].is_blue() || board[row - 1][column - 1].is_blue_king()))
                    {
                        //make the spot it jumped over empty
                        board[row - 1][column - 1].make_empty(); 
                        remove_blue_piece();
                    }
                    //statement used if it is jump to the right
                    else if((row2 - row == -2 && column2 - column == 2)
                        && (board[row - 1][column + 1].is_blue() || board[row - 1][column + 1].is_blue_king()))
                    {                        
                        //make the spot it jumped over empty
                        board[row - 1][column + 1].make_empty();
                        remove_blue_piece();
                    }
                    //change the values of the rows and columns for multiple inputted strings
                    row = row2;
                    column = column2;
                    row2 = move[3 + 2 * i] - '1';
                    column2 = move[2 + 2 * i] - 'A';
                }
            }
            //if the starting piece is a white king
            else if(board[row][column].is_white_king())
            {
                //go through the loop that is used based off of how long the string is (if it is a jump or normal move)
                for(size_t i = 1; i < move.length() / 2; i++)
                {
                    //will make the original spot empty and the next spot white
                    board[row][column].make_empty();
                    board[row2][column2].make_white_king();

                    //statement used if it is jump up to the left
                    if((row2 - row == -2 && column2 - column == -2) 
                         && (board[row - 1][column - 1].is_blue() || board[row - 1][column - 1].is_blue_king()))
                    {
                        //make the spot it jumped over empty
                        board[row - 1][column - 1].make_empty(); 
                        remove_blue_piece();
                    }
                    //statement used if it is jump up to the right
                    else if((row2 - row == -2 && column2 - column == 2)
                        && (board[row - 1][column + 1].is_blue() || board[row - 1][column + 1].is_blue_king()))
                    {                        
                        //make the spot it jumped over empty
                        board[row - 1][column + 1].make_empty();
                        remove_blue_piece();
                    }
                    //statement used if it is jump down to the left
                    else if((row2 - row == 2 && column2 - column == -2)
                        && (board[row + 1][column - 1].is_blue() || board[row + 1][column - 1].is_blue_king()))
                    {
                        //make the spot it jumped over empty
                        board[row + 1][column - 1].make_empty(); 
                        remove_blue_piece();
                    }
                    //statement used if it is jump down to the right
                    else if((row2 - row == 2 && column2 - column == 2)
                        && (board[row + 1][column + 1].is_blue() || board[row + 1][column + 1].is_blue_king()))
                    {
                        //make the spot it jumped over empty
                        board[row + 1][column + 1].make_empty();
                        remove_blue_piece();
                    }
                    //change the values of the rows and columns for multiple inputted strings
                    row = row2;
                    column = column2;
                    row2 = move[3 + 2 * i] - '1';
                    column2 = move[2 + 2 * i] - 'A';
                }
            }
        }   
        //Player 2 (Blue)
        else if(next_mover() == COMPUTER)
        {
            if(board[row][column].is_blue())
            {
                //go through the loop that is used based off of how long the string is (if it is a jump or normal move)
                for(size_t i = 1; i < move.length() / 2; i++)
                {
                    board[row][column].make_empty();
                    //make the final spot a BLUE king if it is on the other side of the board
                    if(row2 == 7 && (column2 == 0 || column2 == 2 || column2 == 4 || column2 == 6))
                    {
                        board[row2][column2].make_blue_king();
                    }
                    //if not, make the spot a regular blue piece
                    else
                    {
                        board[row2][column2].make_blue();
                    }
                    //statement used if it is jump to the left
                    if((row2 - row == 2 && column2 - column == -2) 
                        && (board[row + 1][column - 1].is_white() || board[row + 1][column - 1].is_white_king()))
                    {
                        //make the spot it jumped over empty
                        board[row + 1][column - 1].make_empty(); 
                        remove_white_piece();
                    }
                    //statement used if it is jump to the right
                    else if((row2 - row == 2 && column2 - column == 2)
                        && (board[row + 1][column + 1].is_white() || board[row + 1][column + 1].is_white_king()))
                    {                        
                        //make the spot it jumped over empty
                        board[row + 1][column + 1].make_empty();
                        remove_white_piece();
                    }
                    //change the values of the rows and columns for multiple inputted strings
                    row = row2;
                    column = column2;
                    row2 = move[3 + 2 * i] - '1';
                    column2 = move[2 + 2 * i] - 'A';
                }
            }
            else if(board[row][column].is_blue_king())
            {
                //go through the loop that is used based off of how long the string is (if it is a jump or normal move)
                for(size_t i = 1; i < move.length() / 2; i++)
                {
                    //will make the original spot empty and the next spot white
                    board[row][column].make_empty();
                    board[row2][column2].make_blue_king();

                    //statement used if it is jump up to the left
                    if((row2 - row == -2 && column2 - column == -2) 
                         && (board[row - 1][column - 1].is_white() || board[row - 1][column - 1].is_white_king()))
                    {
                        //make the spot it jumped over empty
                        board[row - 1][column - 1].make_empty(); 
                        remove_white_piece();
                    }
                    //statement used if it is jump up to the right
                    else if((row2 - row == -2 && column2 - column == 2)
                        && (board[row - 1][column + 1].is_white() || board[row - 1][column + 1].is_white_king()))
                    {                        
                        //make the spot it jumped over empty
                        board[row - 1][column + 1].make_empty();
                        remove_white_piece();
                    }
                    //statement used if it is jump down to the left
                    else if((row2 - row == 2 && column2 - column == -2)
                        && (board[row + 1][column - 1].is_white() || board[row + 1][column - 1].is_white_king()))
                    {
                        //make the spot it jumped over empty
                        board[row + 1][column - 1].make_empty(); 
                        remove_white_piece();
                    }
                    //statement used if it is jump down to the right
                    else if((row2 - row == 2 && column2 - column == 2)
                        && (board[row + 1][column + 1].is_white() || board[row + 1][column + 1].is_white_king()))
                    {
                        //make the spot it jumped over empty
                        board[row + 1][column + 1].make_empty();
                        remove_white_piece();
                    }
                    //change the values of the rows and columns for multiple inputted strings
                    row = row2;
                    column = column2;
                    row2 = move[3 + 2 * i] - '1';
                    column2 = move[2 + 2 * i] - 'A';
                }
            }
        }
        //Checks for failure... should never occur
        else
        {
            cout << "Error in Make Move" << endl;
        }
        game::make_move(move);
    }
    
    //Return true if the current game is finished
    bool Checkers::is_game_over() const 
    {
        //return true if the pieces count hits 0, meaning the game is over
        if(bluePieces == 0 || whitePieces == 0)
        {
            return 1;
        }
        //return false if the game is not over
        return 0;
    }
    
    //Determines who wins the game
    game::who Checkers::winning() const
    {
        if(bluePieces == 0 && whitePieces != 0)
        {
            return HUMAN;
        }
        else if(whitePieces == 0 && bluePieces != 0)
        {
            return COMPUTER;
        }
        cout << "Error for the Winner!" << endl;
        return NEUTRAL;
    }

    //checks to see if the jump move is legal or not for white
    bool Checkers::is_jump_legal_white(const std::string& move) const
    {
        //initialize variables while temps are used for the loop
        int row = 0, column = 0, row2 = 0, column2 = 0, tempRow = 0, tempColumn = 0, tempRow2 = 0, tempColumn2 = 0;
        column = (int) move[0] - 'A';
        row = (int) move[1] - '1';
        column2 = (int) move[2] - 'A';
        row2 = (int) move[3] - '1';
        tempRow = row;
        tempColumn = column;
        tempRow2 = row2;
        tempColumn2 = column2;
        
        //go through the look according to the length   ex: A4C3 is length 2
        for(size_t i = 1; i < move.length() / 2; i++)
        {
            //detects if the left diagonal is legal or not
            if(board[row][column].is_white() && board[tempRow2][tempColumn2].is_empty() 
                && (tempRow2 - tempRow == -2) && (tempColumn2 - tempColumn == -2)
                && (board[tempRow - 1][tempColumn - 1].is_blue() || board[tempRow - 1][tempColumn - 1].is_blue_king()))
            {
                tempRow = tempRow2;
                tempColumn = tempColumn2;
                tempRow2 = move[3 + 2 * i] - '1';
                tempColumn2 = move[2 + 2 * i] - 'A';
            }
            //detects if the top right diagonal is legal or not
            else if(board[row][column].is_white() && board[tempRow2][tempColumn2].is_empty()
                && (tempRow2 - tempRow == -2) && (tempColumn2 - tempColumn == 2)
                && (board[tempRow - 1][tempColumn + 1].is_blue() || board[tempRow - 1][tempColumn + 1].is_blue_king()))
            {
                tempRow = tempRow2;
                tempColumn = tempColumn2;
                tempRow2 = move[3 + 2 * i] - '1';
                tempColumn2 = move[2 + 2 * i] - 'A';
            }
            //if neither of them work, return false
            else
            {
                return 0;
            }
        }
        //return true otherwise
        return 1;
    }

    //checks to see if the jump move is legal or not for blue
    bool Checkers::is_jump_legal_blue(const std::string& move) const
    {
        //initialize variables while temps are used for the loop
        int row = 0, column = 0, row2 = 0, column2 = 0, tempRow = 0, tempColumn = 0, tempRow2 = 0, tempColumn2 = 0;
        column = (int) move[0] - 'A';
        row = (int) move[1] - '1';
        column2 = (int) move[2] - 'A';
        row2 = (int) move[3] - '1';
        tempRow = row;
        tempColumn = column;
        tempRow2 = row2;
        tempColumn2 = column2;
        
        //go through the look according to the length   ex: A4C3 is length 2
        for(size_t i = 1; i < move.length() / 2; i++)
        {
            //detects if the left diagonal is legal or not
            if(board[row][column].is_blue() && board[tempRow2][tempColumn2].is_empty() 
                && (tempRow2 - tempRow == 2) && (tempColumn2 - tempColumn == -2)
                && (board[tempRow + 1][tempColumn - 1].is_white() || board[tempRow + 1][tempColumn - 1].is_white_king()))
            {
                tempRow = tempRow2;
                tempColumn = tempColumn2;
                tempRow2 = move[3 + 2 * i] - '1';
                tempColumn2 = move[2 + 2 * i] - 'A';
            }
            //detects if the top right diagonal is legal or not
            else if(board[row][column].is_blue() && board[tempRow2][tempColumn2].is_empty()
                && (tempRow2 - tempRow == 2) && (tempColumn2 - tempColumn == 2)
                && (board[tempRow + 1][tempColumn + 1].is_white() || board[tempRow + 1][tempColumn + 1].is_white_king()))
            {
                tempRow = tempRow2;
                tempColumn = tempColumn2;
                tempRow2 = move[3 + 2 * i] - '1';
                tempColumn2 = move[2 + 2 * i] - 'A';
            }
            //if neither of them work, return false
            else
            {
                return 0;
            }
        }
        //return true otherwise
        return 1;
    }

    //checks to see if the jump move is legal or not for white king
    bool Checkers::is_jump_legal_white_king(const std::string& move) const
    {
        //initialize variables while temps are used for the loop
        int row = 0, column = 0, row2 = 0, column2 = 0, tempRow = 0, tempColumn = 0, tempRow2 = 0, tempColumn2 = 0;
        column = (int) move[0] - 'A';
        row = (int) move[1] - '1';
        column2 = (int) move[2] - 'A';
        row2 = (int) move[3] - '1';
        tempRow = row;
        tempColumn = column;
        tempRow2 = row2;
        tempColumn2 = column2;
        
        //go through the look according to the length   ex: A4C3 is length 2
        for(size_t i = 1; i < move.length() / 2; i++)
        {
            //detects if the up left diagonal is legal or not
            if(board[row][column].is_white_king() && board[tempRow2][tempColumn2].is_empty() 
                && (tempRow2 - tempRow == -2) && (tempColumn2 - tempColumn == -2)
                && (board[tempRow - 1][tempColumn - 1].is_blue() || board[tempRow - 1][tempColumn - 1].is_blue_king()))
            {
                tempRow = tempRow2;
                tempColumn = tempColumn2;
                tempRow2 = move[3 + 2 * i] - '1';
                tempColumn2 = move[2 + 2 * i] - 'A';
            }
            //detects if the top up right diagonal is legal or not
            else if(board[row][column].is_white_king() && board[tempRow2][tempColumn2].is_empty()
                && (tempRow2 - tempRow == -2) && (tempColumn2 - tempColumn == 2)
                && (board[tempRow - 1][tempColumn + 1].is_blue() || board[tempRow - 1][tempColumn + 1].is_blue_king()))
            {
                tempRow = tempRow2;
                tempColumn = tempColumn2;
                tempRow2 = move[3 + 2 * i] - '1';
                tempColumn2 = move[2 + 2 * i] - 'A';
            }
            //detects if the down left diagonal is legal or not
            else if(board[row][column].is_white_king() && board[tempRow2][tempColumn2].is_empty() 
                && (tempRow2 - tempRow == 2) && (tempColumn2 - tempColumn == -2)
                && (board[tempRow + 1][tempColumn - 1].is_blue() || board[tempRow + 1][tempColumn - 1].is_blue_king()))
            {
                tempRow = tempRow2;
                tempColumn = tempColumn2;
                tempRow2 = move[3 + 2 * i] - '1';
                tempColumn2 = move[2 + 2 * i] - 'A';
            }
            //detects if the top down right diagonal is legal or not
            else if(board[row][column].is_white_king() && board[tempRow2][tempColumn2].is_empty()
                && (tempRow2 - tempRow == 2) && (tempColumn2 - tempColumn == 2)
                && (board[tempRow + 1][tempColumn + 1].is_blue() || board[tempRow + 1][tempColumn + 1].is_blue_king()))
            {
                tempRow = tempRow2;
                tempColumn = tempColumn2;
                tempRow2 = move[3 + 2 * i] - '1';
                tempColumn2 = move[2 + 2 * i] - 'A';
            }
            //if none of them work, return false
            else
            {
                return 0;
            }
        }
        //return true otherwise
        return 1;
    }
    
    //checks to see if the jump move is legal or not for blue king
    bool Checkers::is_jump_legal_blue_king(const std::string& move) const
    {
        //initialize variables while temps are used for the loop
        int row = 0, column = 0, row2 = 0, column2 = 0, tempRow = 0, tempColumn = 0, tempRow2 = 0, tempColumn2 = 0;
        column = (int) move[0] - 'A';
        row = (int) move[1] - '1';
        column2 = (int) move[2] - 'A';
        row2 = (int) move[3] - '1';
        tempRow = row;
        tempColumn = column;
        tempRow2 = row2;
        tempColumn2 = column2;
        
        //go through the look according to the length   ex: A4C3 is length 2
        for(size_t i = 1; i < move.length() / 2; i++)
        {
            //detects if the up left diagonal is legal or not
            if(board[row][column].is_blue_king() && board[tempRow2][tempColumn2].is_empty() 
                && (tempRow2 - tempRow == -2) && (tempColumn2 - tempColumn == -2)
                && (board[tempRow - 1][tempColumn - 1].is_white() || board[tempRow - 1][tempColumn - 1].is_white_king()))
            {
                tempRow = tempRow2;
                tempColumn = tempColumn2;
                tempRow2 = move[3 + 2 * i] - '1';
                tempColumn2 = move[2 + 2 * i] - 'A';
            }
            //detects if the top up right diagonal is legal or not
            else if(board[row][column].is_blue_king() && board[tempRow2][tempColumn2].is_empty()
                && (tempRow2 - tempRow == -2) && (tempColumn2 - tempColumn == 2)
                && (board[tempRow - 1][tempColumn + 1].is_white() || board[tempRow - 1][tempColumn + 1].is_white_king()))
            {
                tempRow = tempRow2;
                tempColumn = tempColumn2;
                tempRow2 = move[3 + 2 * i] - '1';
                tempColumn2 = move[2 + 2 * i] - 'A';
            }
            //detects if the down left diagonal is legal or not
            else if(board[row][column].is_blue_king() && board[tempRow2][tempColumn2].is_empty() 
                && (tempRow2 - tempRow == 2) && (tempColumn2 - tempColumn == -2)
                && (board[tempRow + 1][tempColumn - 1].is_white() || board[tempRow + 1][tempColumn - 1].is_white_king()))
            {
                tempRow = tempRow2;
                tempColumn = tempColumn2;
                tempRow2 = move[3 + 2 * i] - '1';
                tempColumn2 = move[2 + 2 * i] - 'A';
            }
            //detects if the top down right diagonal is legal or not
            else if(board[row][column].is_blue_king() && board[tempRow2][tempColumn2].is_empty()
                && (tempRow2 - tempRow == 2) && (tempColumn2 - tempColumn == 2)
                && (board[tempRow + 1][tempColumn + 1].is_white() || board[tempRow + 1][tempColumn + 1].is_white_king()))
            {
                tempRow = tempRow2;
                tempColumn = tempColumn2;
                tempRow2 = move[3 + 2 * i] - '1';
                tempColumn2 = move[2 + 2 * i] - 'A';
            }
            //if none of them work, return false
            else
            {
                return 0;
            }
        }
        //return true otherwise
        return 1;
    }

    //removes a white piece from the total
    void Checkers::remove_white_piece()
    {
        whitePieces = whitePieces - 1;
    }

    //removes a white piece from the total
    void Checkers::remove_blue_piece()
    {
        bluePieces = bluePieces - 1;
    }

    // Return a pointer to a copy of myself 
    game* Checkers::clone() const
    {
        return new Checkers(*this);
    }

    // Compute all the moves that the next player can make
    void Checkers::compute_moves(std::queue<std::string>& moves) const
    {
        //initialize strings that will be used for the move; use example e.g. A2B3
        string temp = "XXXX";    

        //used for the first row; e.g. = 2
        for(char row = '1'; row < '9'; row++)
        {
            temp[1] = row;

           //used for the first column; e.g. = A
            for(char column = 'A'; column < 'I'; column++)
            {
                temp[0] = column;               

                //used for the second row; e.g. = 3
                for(char row2 = '1'; row2 < '9'; row2++)
                {
                    temp[3] = row2;

                    //used for the second column; e.g. = B
                    for(char column2 = 'A'; column2 < 'I'; column2++)
                    {
                        temp[2] = column2;
                        //statement that will check if the move is legal
                        if(is_legal(temp))
                        {
                            moves.push(temp);
                        }
                    }
                }
            }
        }
    }

    // Evaluate a board position
    // NOTE: positive values are good for the computer.
    int Checkers::evaluate() const
    {
        int bestMove = 0;

        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                if(board[i][j].is_blue())
                {
                    bestMove++;
                }
                else if(board[i][j].is_blue_king())
                {
                    bestMove = bestMove + 3;
                }
                else if(board[i][j].is_white())
                {
                    bestMove--;
                }
                else if(board[i][j].is_white_king())
                {
                    bestMove = bestMove - 3;
                }
            }
        }
        return bestMove;
    }


}



#include <iostream>   // For cin, cout
#include <fstream>    // For file input
#include <chrono>     // Used in pausing for some milliseconds using sleep_for(...)
#include <thread>     // Used in pausing for some milliseconds using sleep_for(...)
#include <iomanip> 
#include <stdlib.h>
using namespace std;

// Global constants
const int MaxNumberOfBoards = 118;
const int BoardRows = 8;
const int BoardColumns = 10;
const int NumberOfPresetBoardParValues = 60;    // After the first 60 par values, the default par value is 15
const char FileName[] = "boards.txt";
const int SleepAmount = 150;                    // Length of time to pause, in milliseconds


//------------------------------------------------------------------------------------------------------------------
// Class used to store a game board
class Board
{
    private:
      int board[BoardRows*BoardColumns];
      int boardNumber;
      int parValue;

    public:
      Board(); // default constructor
      Board(int boardIndex, int allBoards[BoardRows*BoardColumns], int levelParValues ); // constructor accepting values
      //get the board number
      int GetboardNumber(int level) {
        return level;
      }
      //get the parValue...
      int GetParValue()
      {
        return parValue;
      }
      //set boardnumber
      void SetboardNumber(int level)
      {
        boardNumber = level;
      }
      //set the board
      void Setboard(int* board)
      {
        *board = board[BoardRows*BoardColumns];
      }
      //set the parValue...
      void SetParValue(int boardIndex)
      {
         parValue = boardIndex;
      }
      //Utility memeber functions
      void displayBoard();//displays the form of the board
      void changingTheBoard(int board[]);//
      void updateBoard(int row, int col, char direction);
      void checkForMatches();//changes the board from its origninal int form to characters (symbols)        
      bool checkForNextLevel();
      int move(char rowChar, int col, char direction, int &moveNumber);
      void drop(int destination);
      void destroy();
      void gravity();
      void currentPieceGravity(int currentPiece);
      bool checkSpaceUnderSymbol();
};  // end class Board

//default board constructor...
Board::Board()
{    
for(int i = 0; i < BoardRows * BoardColumns; i++){
    board[i] = -1;
}
    boardNumber = -1;
    parValue = -1;  
}

//fully qualify Board constructor...
Board:: Board(int boardIndex, int allBoards[BoardRows*BoardColumns], int levelParValues )
{
    for(int i = 0; i < BoardRows*BoardColumns; i++)
    {
        board[i] = allBoards[i];
    }    
    boardNumber = boardIndex;
    parValue = levelParValues;
}

 void Board::changingTheBoard(int board[]){ //change the number values of the board into symbols
 for(int i = 0; i < BoardRows*BoardColumns; i++){
               if(board[i] == 0){board[i] = ' ';}
          else if(board[i] == 1){board[i] = '&';}                                 
          else if(board[i] == 2){board[i] = '@';}                  
          else if(board[i] == 3){board[i] = '+';}                                 
          else if(board[i] == 4){board[i] = '%';}                                 
          else if(board[i] == 5){board[i] = '^';}                  
          else if(board[i] == 6){board[i] = '#';}                    
          else if(board[i] == 7){board[i] = '=';}                    
          else if(board[i] == 8){board[i] = '*';}                    
          else if(board[i] == 9){board[i] = '.';}
      }
}

 void Board::checkForMatches(){ //checks and eliminates all symbol matches
    //returns true if matches have been found and board has been updated
     
    while(checkSpaceUnderSymbol() == true){
      gravity();         //keep dropping remaining pieces until it hits floor
    }
     
    bool pendingDeletion[BoardRows*BoardColumns] = {0};
    char symbol[7] = {'&','@','+','%','^','=','*'};
    for(int s = 0; s < 7; s++){
        bool matchesFound = false;
        for(int i = 0; i < BoardRows*BoardColumns; i++)
        {
            //check left if not at left portion of board
            if ( !(i%BoardColumns == 0)  )
            {
                if (board[i] == symbol[s] && board[i]==board[i-1]){
                    pendingDeletion[i]=true;
                    matchesFound = true;
                }
            }
            //check right if not at right portion of board
            if ( !(i%BoardColumns == BoardColumns-1) )
            {
                if (board[i] == symbol[s] && board[i]==board[i+1]){
                    pendingDeletion[i]=true;
                    matchesFound = true;
                }
            }
            //check upper if not at top portion of board
            if ( !(i<BoardColumns) )
            {
                if (board[i] == symbol[s] && board[i]==board[i-BoardColumns]){
                    pendingDeletion[i]=true;
                    matchesFound = true;
                }
            }
            //check lower if not at bottom portion of board
            if ( !(i>=(BoardColumns*BoardRows-BoardColumns)) )
            {
                if (board[i] == symbol[s] && board[i]==board[i+BoardColumns]){
                    pendingDeletion[i]=true;
                    matchesFound = true;
                }
            }
        }
        if (matchesFound){ //if matches are found, delete all the matches and redisplay the board
            for(int i = 0; i < BoardRows*BoardColumns; i++)
            {
                if(pendingDeletion[i]){
                    board[i]=' ';
                }
            }
            displayBoard();
            checkForMatches();
        }
    }
 }

 bool Board::checkForNextLevel(){  //function that checks for the next level
   for(int i = 0; i < BoardRows*BoardColumns; i++){
     if(board[i] == '@' || board[i] == '&'){
       return false;
     }
   }
   return true;
 }

 void Board::displayBoard(){   //display the board
   cout << "    Board " << boardNumber << "  par " << parValue << endl;
   cout << setw(7);
   for(int col = 0; col < 10; col ++){
     cout << col << " ";
   }
   cout << endl;
   cout << setw(5);
   for(int col = 0; col < 23; col++){
     cout << "-";
   }
   cout << endl;
   cout << "  A" << " | ";
   for(char row = 'A'; row <= 'H'; row++){
     for(int j = 0; j < BoardColumns * BoardRows; j++){
       if(j == 0){
         cout << setw(1);
       }
       else if (j % 10 == 0){
          cout << "| " << row;
          row++;
          cout << endl;
          cout << setw(3)<< row << " | ";
       }
       //print out the board values
       changingTheBoard(board);
       cout <<(char)board[j] << " ";
     }
     cout << "| " << row;
   }
   cout << endl;
   cout << setw(5);
   for(int col = 0; col < 23;col++){
      cout << "-";
   }
   cout << endl;
   cout << setw(7);
   for(int col = 0; col < 10; col++){
      setw(4);
      cout << col << " ";
   }
   cout << endl << endl;
 }


 void Board::gravity(){  //check if any board peices have a space below and move them down
   char symbol;
   for(int i = 0; i < BoardRows*BoardColumns; i++){ 
     if(board[i] != '.' && board[i] != ' '){
       symbol=board[i];
       while(board[i+10] == ' '){
         board[i] = ' ';
         i+=10;
         board[i] = symbol;
         displayBoard();
       }
     }
   }
 }

 void Board::currentPieceGravity(int currentPiece){ //check if the current piece moved has a space below and move it down (debug function for gravity)
   char symbol=board[currentPiece];
   while(board[currentPiece+10] == ' '){
     board[currentPiece] = ' ';
     currentPiece+=10;
     board[currentPiece] = symbol;
     displayBoard();
   }
 }
    
 bool Board::checkSpaceUnderSymbol(){ //checks for space under current symbol
   char symbol;
   for(int i = 0; i < BoardRows*BoardColumns; i++){
     if(board[i] != '.' && board[i] != ' '){
       symbol=board[i];
       while(board[i+10] == ' '){
         return true;
       }
     }
   }
   return false;
 }

 int Board::move(char rowChar, int col, char direction, int &moveNumber){ //**main** function that plays the game
   //returns -1 for failure
   //returns 0 for success
   if (rowChar < 'a' || rowChar >= 'a'+BoardRows){                         //check if user entered "a - h"
       cout <<"Move value is out of bounds. Please retry." << endl; 
       return -1;
   }
   if (col < 0 || col >= BoardColumns){                                    //check if user entered "0 - 10"
       cout << "Move value is out of bounds. Please retry." << endl; 
       return -1;
   }
   if (!(direction == 'l' || direction =='r')){                              //check if user entered "l or r"
       cout << "Invalid move direction. Please retry." << endl;
       return -1;
   }
   
   int row = rowChar%'a'* BoardColumns;
   int usersSelection = row+col;
   char symbol = board[usersSelection];    //symbol at user selection
   int usersDestination;
   if (symbol == '.' || symbol == ' '){                                    //check if user is attempting to move valid charecter
      cout << "Attempting to move an invalid character. Please retry." << endl; 
      return -1;
   }

   if(direction == 'r'){
     if (usersSelection%BoardColumns == BoardColumns-1)                    //check if user is trying to move right while already at right portion of board
     {
        cout << "Move value is out of bounds. Please retry." << endl; 
        return -1;
     }
     usersDestination = usersSelection+1;
   }
   else{
     if (usersSelection%BoardColumns == 0)                                              //check if user is trying to move left while already at left portion of board
     {
         cout << "Move value is out of bounds. Please retry." << endl; 
         return -1;
     }
     usersDestination = usersSelection-1;
   }
         
   if(board[usersDestination] == ' '){                                     //check if user is trying to move into open space
       moveNumber++;
       board[usersSelection] = ' ';
       board[usersDestination] = symbol;
   } else {
       cout << "Attempting to move into a non-empty space. Please retry." << endl;    
       return -1;
   }
   
   displayBoard();
   currentPieceGravity(usersDestination);
   checkForMatches(); //keep updating board for effects of gravity and matches
 }

//------------------------------------------------------------------------------------------------------------------
// Class used to read in and store all game boards
class AllBoards
{
    public:
        //------------------------------------------------------------------------------------------------------------------
        // Constructor that reads in data from the data file.
        AllBoards();    // Declaration.  See the definition outside the class, below.
    
        // Get and Set member functions.
    
        //------------------------------------------------------------------------------------------------------------------
        // Find and return a particular board.
        Board getBoard( int boardIndex)
        {
            // Validate board index number
            if( boardIndex < 0 || boardIndex > MaxNumberOfBoards) {
                cout << "In getBoard() inside AllBoards class, boardIndex "
                     << boardIndex << " is out of range. Exiting..." << endl;
                exit( -1);
            }
            
            // Construct a Board from one of all the boards
            Board newBoard( boardIndex, allBoards[ boardIndex], levelParValues[ boardIndex]);
            return newBoard;
        }
    
        //------------------------------------------------------------------------------------------------------------------
        // Retrieve the par value
        int getParValue( int boardIndex) {
            int theParValue = 15;   // default
            if( boardIndex < NumberOfPresetBoardParValues) {
                theParValue = levelParValues[ boardIndex];
            }
            return theParValue;
        } // end getParValue()
    
    private:
        int par = 0;             // The number of moves it should take to solve this level
        int currentLevel = -1;   // Which board we're on
        int allBoards[ 118][ BoardRows * BoardColumns];   // Array to store all 118 8x10 boards
    
        // Par values for levels 0 through 59.  Default is 15 after that.
        const int levelParValues[ NumberOfPresetBoardParValues] =
            // 0   1   2   3   4   5   6   7   8   9
            { 4,  3, 14,  5,  6,  4,  4, 14,  5, 25,  //  0 -  9
             12, 17, 17,  8,  4, 12,  9,  8, 12,  8,  // 10 - 19
             10, 10, 16, 13, 20, 14, 10,  9, 12, 14,  // 20 - 29
             15, 13, 20,  8, 15, 10, 10, 11,  7, 23,  // 30 - 39
              8, 11, 16, 14, 12, 13, 13,  3, 35, 18,  // 40 - 49
             26, 10, 13, 18, 26, 12, 15,  5, 22, 15}; // 50 - 59
}; //end class allBoards


//------------------------------------------------------------------------------------------------------------------
// AllBoards constructor
AllBoards::AllBoards()
 {
    // Read in all the boards from the data file
    ifstream inputFileStream;  // declare the input file stream

    // Open input file and verify file open worked.
    inputFileStream.open( FileName);
    if( !inputFileStream.is_open()) {
        cout << "Could not find input file. " << FileName << ".  Exiting..." << endl;
        exit( -1);
    }

    // Read the five lines of comments at the top of the datafile.  Datafile structure is:
    //    // Originally from Vexed v2.0 - globals.c "Global variable declarations"
    //    // Copyright (C) 1999 James McCombe (cybertube@earthling.net)
    //    // September 1,2001 - Version 2.0 changes by Mark Ingebretson (ingebret@yahoo.com) and others.
    //    // Oct 13, 2019  Format changed by Dale Reed (reed @ uic.edu)
    //    // Covered by the GNU General Public License https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
    //    // Level 000
    //    9 9 9 9 9 9 9 9 9 9
    //    9 9 9 9 9 9 9 9 9 9
    //    9 9 9 2 1 0 0 9 9 9
    //    9 9 9 9 9 0 0 9 9 9
    //    9 9 9 0 0 0 0 9 9 9
    //    9 9 9 1 0 0 2 9 9 9
    //    9 9 9 9 1 2 9 9 9 9
    //    9 9 9 9 9 9 9 9 9 9
    //    // Level 001
    //    9 9 9 9 9 9 9 9 9 9
    //    9 9 9 9 9 9 9 9 9 9
    // ...
    
    // Read and discard the five lines of comments at the beginning of the file.
    string inputLine;
    for( int i=0; i<5; i++) {
        getline(inputFileStream, inputLine);
    }
    
    // Now read each of the 118 (MaxNumberOfBoards) boards.  Each one starts with a comment that has the board number.
    for( int boardIndex = 0; boardIndex < MaxNumberOfBoards; boardIndex++) {
        // Read the comment with the board number
        getline(inputFileStream, inputLine);
        
        // For each board now read the BoardRows * BoardColumns number of board values, as integers
        for( int row=0; row<BoardRows; row++) {
            for( int col=0; col<BoardColumns; col++) {
                inputFileStream >> allBoards[ boardIndex][ row * BoardColumns + col];
            }
            // Get rid of end-of-line character at the end of each board, so we can get a clean start on reading the
            //   first line of the next board, on the line with the comment and board number.
            getline(inputFileStream, inputLine);
        }
    }//end for( int boardIndex=0...

    // Close the input file.
    inputFileStream.close();
}//end AllBoards() constructor


//------------------------------------------------------------------------------------------------------------------
void displayDirections()
{
    cout << "Welcome to Vexed!  \n"
         << "The objective is to place identical pieces next to each other, so that they vanish,   \n"
         << "clearing the board completely within the indicated number of moves. On each move      \n"
         << "enter the row, column, and direction (L or R) to move, in either upper or lower       \n"
         << "case. You may also enter 'x' to exit the program, or 'r' to reset the current level. \n"
         << endl;
}


//------------------------------------------------------------------------------------------------------------------
// Driver for the program, using the classes and functions declared above
int main()
{
    AllBoards allTheBoards;     // Reads in and stores all the boards from a data file
    int currentBoardIndex = 0;  // Starting board index
    Board theBoard;             // The board instance, that is set to hold the values for each level
    int score = 0;              // Score accumulates par points for each level finished
    char userRow;               //input character for users row selection
    int userCol;                //input integer for users column selection
    char direction;             //input character for users direction
    int moveNumber = 1;         //default move number = 1
    bool isPlaying = true;      //isPlaying is automatically true if the program is running
    displayDirections();        //Display directions function
    theBoard = allTheBoards.getBoard(currentBoardIndex);
    theBoard.displayBoard();
    
    while(isPlaying){
      //parse user input
      cout << moveNumber << ". Your move: \n";
      cin >> userRow;
      if(userRow == 'r'){
          theBoard = allTheBoards.getBoard(currentBoardIndex);
          theBoard.displayBoard();
          moveNumber = 1;
          continue;
      }
      if(userRow == 'x'){  //if user inputs just 'x' then exit the game
        return 0;
      }
      cin >> userCol;
      if(userRow == 's' && userCol >= 0){  //if user decides to set the level, set the level to desired level
        theBoard = allTheBoards.getBoard(userCol);
        theBoard.displayBoard();
        moveNumber = 1;
        continue;
      }
      cin >> direction;
      theBoard.move(userRow, userCol, direction, moveNumber); //get all the user inputs and move 
        
      //check if board is empty (move to next level) else exceed par (restart the level)
      if(theBoard.checkForNextLevel())
      {
          if(moveNumber-1 <= theBoard.GetParValue()) //proceed to next level
          {
            moveNumber = 1;
            score += theBoard.GetParValue();
            cout << "Congratulations! On to the next level." << endl;
            cout << "Score: " << score << endl;
            currentBoardIndex++;
            theBoard = allTheBoards.getBoard(currentBoardIndex);
            theBoard.displayBoard();
          } 
          else
          {
            cout << "Sorry, you took " << moveNumber-1 << " moves and you must finish within " << theBoard.GetParValue() << " moves before moving on.";
            moveNumber = 1;
            theBoard = allTheBoards.getBoard(currentBoardIndex);
            theBoard.displayBoard();
          }
      }
    }
    return 0;
}
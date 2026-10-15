/*
Program Name: EECS 348 Assignment 4

Description: This is C++ program that reads five Sudoku puzzles and prints every solution using
            recursive depth-first search with backtracking.

Inputs: puzzle1.txt, puzzle2.txt, puzzle3.txt, puzzle4.txt, puzzle5.txt

Output: Each original puzzle, all solutions, or "No solution found".
        
Collaborator: Gemini was used as the final version of code from two GenAI(Claude & Gemini) generated comparison code. 
              I modified the Gemini's generated code by implementing input validation, checking if there is any conflicting numbers.
              I also improved error handling, and adding better comments on each line for better readability. 

Other sources: 
Claude - https://claude.ai/ 
Gemini - https://gemini.google.com/app

Author: Saad Hasnain

Creation Date: 10/12/2026

Revision Date: 10/12/2026

Revisions: None 
*/
#include <iostream>
#include <fstream> 
#include <vector>
#include <string>

// SudokuGame class encapsulates all data and operations for solving a Sudoku puzzle
class SudokuGame { // sudokuGame class created
private:
    std::string filename; // Reads files
    char originalGrid[9][9]; // Keep orignal clues unchanged
    char currentGrid[9][9]; // Working board used by backtracking
    int solutionCount; // Number of solution printed
    std::string errorMessage; // Explain file reading problems

    // Helper method to check if placing a digit at a specific cell is valid
    bool isValid(int row, int col, char num) { //
        // Check row and column constraints
        for (int i = 0; i < 9; i++) { // for loop iterating through 0 to 8 
            if (currentGrid[row][i] == num) return false; // Checks row conflict
            if (currentGrid[i][col] == num) return false; // Checks column conflict
        }

        // Check 3x3 block constraints
        int startRow = row - (row % 3); // First row of block 3x3
        int startCol = col - (col % 3); // First column of the block
        for (int r = 0; r < 3; r++) { // iterates each of the three rows inside the 3x3 block
            for (int c = 0; c < 3; c++) { // iterates each of the three columns inside the 3x3 block.
                if (currentGrid[startRow + r][startCol + c] == num) return false; // Check if same digits already exist or not
            }
        }

        return true; // Return true because no conflicts were found
    }

    bool originalPuzzleIsValid() { // check the starting clues before searching
        for (int row = 0; row < 9; row++) { // for loop - it will iterates every row in board
            for (int col = 0; col < 9; col++) { // for loop - it will iterates every column in board for each row
                char digit = currentGrid[row][col]; // Save the current sell digit
                if (digit == '_') continue; // Skip the cell digits if its empty
                currentGrid[row][col] = '_'; // Temporarily empties the cell to prevent checking the digit against itself
                bool valid = isValid(row, col, digit); // Checks if the saved digit conflicts with other clues
                currentGrid[row][col] = digit; // Restore the clue
                if (!valid) return false; 
            }
        }
        return true; // Returns true when all starting clues are consistent.
    }
    
    // Recursive depth-first search and backtracking method to find all solutions
    void solve(int row, int col) {
        // Base case: If we have successfully traversed past the last row, a solution is found
        if (row == 9) { // Checks if the search has reached the row after the end row or not.
            solutionCount++; // Increment the counter for completed solution
            std::cout << "Solution " << solutionCount << ":\n"; // Prints solution count
            displayGrid(currentGrid); // Display/prints the completed sudoku board
            return; 
        }

        // Determine coordinates for the next cell
        int nextRow = (col == 8) ? row + 1 : row; // Calculates next row, moving down when the current colum is the last column
        int nextCol = (col == 8) ? 0 : col + 1; // Calculates next column, reset it to zero when moving to another row

        // If the current cell is pre-filled, skip to the next cell
        if (currentGrid[row][col] != '_') {
            solve(nextRow, nextCol); // Skips the cell that is already filled and continue over to the next one
            return; // This will stops processing the current cell 
        }

        // Try placing digits from '1' to '9'
        for (char num = '1'; num <= '9'; num++) {
            if (isValid(row, col, num)) {
                currentGrid[row][col] = num; // Place valid digit in the current cell
                solve(nextRow, nextCol);     // Recursively try to solve the remaining cell
                currentGrid[row][col] = '_'; // Removes candidiate digit to backtrack 
            }
        }
    }

    // Helper method to print any 9x9 grid
    void displayGrid(char grid[9][9]) { 
        for (int i = 0; i < 9; i++) { // This for loop iterates through all 9 rows of board
            for (int j = 0; j < 9; j++) { // It will iterates through all 9 columns of the current row
                std::cout << grid[i][j] << " "; // Prints current cell 
            }
            std::cout << "\n"; // Moves result to the next line
        }
        std::cout << "\n"; // Adds a blank line after printing the complete grid
    }

public:
    // Constructor initializes the filename and solution counter
    SudokuGame(const std::string& fname) : filename(fname), solutionCount(0) {}

    // Method to read the puzzle grid from the designated text file
    bool loadPuzzle() {
        std::ifstream file(filename); // opens puzzle file
        if (!file.is_open()) { // check for file if its missing or not accessible
            errorMessage = "Could not open file " + filename; // error message - if the file can't open it will display error message
            return false;
        }
        char value;
        for (int i = 0; i < 9; i++) { // for loop - iterates nine rows
            for (int j = 0; j < 9; j++) { // for loop - Read nine cells of each row
                if (!(file >> value)) { // Checks for missing cells or any file read failures 
                    errorMessage = "Puzzle must contain exactly 81 cells"; // error message - if the cells are missing 
                    return false;
                }
                if (value != '_' && (value < '1' || value > '9')) { // Only '_' and digits 1 through 9 are allowed
                    errorMessage = "Invalid character in puzzle file"; // if above command wont match the error message will display
                    return false; 
                }
                originalGrid[i][j] = value; // Save puzzle
                currentGrid[i][j] = value; // Initialize working puzzle
            }
        }

        if (file >> value) { // Check for extra non whitespace data if exist
            errorMessage = "Puzzle contains more than 81 cells"; // error message
            return false;
        }
        if (file.bad()) { // Report an unexpected error for reading file
            errorMessage = "Error reading puzzle file"; // error message for reading puzzle file 
            return false;
        }
        return true; // File contains 81 cells
    }

    // Method to manage the solving lifecycle for the puzzle
    void process() {
        std::cout << "========================================\n";
        std::cout << "Puzzle File: " << filename << "\n";
        std::cout << "========================================\n";

        if (!loadPuzzle()) { // stops puzzle if file is invalid
            std::cout << "Error: " << errorMessage << "\n\n"; // prints an error message including filename
            return; // stops if file not opens
        }

        std::cout << "Original Puzzle:\n";
        displayGrid(originalGrid); // shows the starting board unchanged

        if (!originalPuzzleIsValid()) { // Added - reject conflicting clues
            std::cout << "No solution found\n\n"; // Print the message if no solution found
            return; // Avoid any recursive search that is unnecessary
        }

        solutionCount = 0; // Reset counter to 0
        solve(0, 0); // start recursive search at first cell

        // If no solutions were found, display notification
        if (solutionCount == 0) { // Checks for solution count if it is 0
            std::cout << "No solution found.\n\n"; // prints how many solutions were printed
        } else {
            std::cout << "Total valid solutions found: " << solutionCount << "\n\n"; // prints total valid solutions that were found
        }
    }
};

int main() {
    // Array containing the five required puzzle file names
    std::string puzzleFiles[5] = { // Required 5 puzzle filenames
        "puzzle1.txt", 
        "puzzle2.txt", 
        "puzzle3.txt", 
        "puzzle4.txt", 
        "puzzle5.txt"
    };

    // Process each puzzle file sequentially using the object-oriented approach
    for (int i = 0; i < 5; i++) { // for loop that process all five files
        SudokuGame game(puzzleFiles[i]); // creates one puzzle object
        game.process(); // Read validate and solve that puzzle
    }

    return 0; // terminates program
}
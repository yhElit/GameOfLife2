#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Timing.h"
#include <omp.h>

using namespace std;

vector<vector<char>> loadBoard(string fileName)
{
	vector<vector<char>> board;
	string line;
	string word;
	int rowSize;
	int columnSize;

	// Open a file in read mode
	ifstream inFile(fileName);

	if (inFile.fail())
	{
		cout << "\n--> Unable to open or find file!";
		return board;
	}

	// get first line
	inFile >> line;

	// used for breaking words
	stringstream s(line);

	// column size
	getline(s, word, ',');
	columnSize = stof(word);

	// row size
	getline(s, word);
	rowSize = stof(word);

	// save each line to vector board 
	for (int r = 0; r < rowSize; r++)
	{
		vector<char> tmp;
		inFile >> line;

		// save each character in line to a vector
		for (char letter : line)
		{
			tmp.push_back(letter);
		}

		board.push_back(tmp);
	}
	cout << "\n";

	// print board
	/*cout << "\nInput:\n";
	for (int i = 0; i < rowSize; i++)
	{
		cout << i << ": ";
		for (int j = 0; j < columnSize; j++)
		{
			cout << board[i][j];
		}
		cout << "\n";
	}
	*/
	inFile.close();

	cout << "--> Board loaded successfully!\n";

	return board;
}

void saveBoard(string fileName, vector<vector<char>> board)
{
	// File pointer
	ofstream outFile;

	// creates and opens a new file
	outFile.open(fileName, ios::out);

	if (!outFile) {
		cout << "\n--> File not created!";
	}
	else {

		// save rows and columns in first line
		outFile << board[0].size() << "," << board.size() << std::endl;

		// save board
		for (int i = 0; i < board.size(); i++)
		{
			for (int j = 0; j < board[0].size(); j++)
			{
				outFile << board[i][j];
			}
			outFile << endl;
		}
		outFile.close();

		// print board
		/*cout << " Output:\n";
		for (int i = 0; i < board.size(); i++)
		{
			cout << i << ": ";
			for (int j = 0; j < board[0].size(); j++)
			{
				cout << board[i][j];
			}
			cout << "\n";
		}*/
		cout << "--> Board saved successfully!\n";
	}

}

vector<vector<char>> gol_seq_old(vector<vector<char>> board, int generations)
{
	vector<vector<char>> nextBoard = board;
	int aliveNeighbors = 0;
	int colSize = board[0].size();
	int row, col;

	for (int g = 1; g <= generations; g++)
	{
		//cout << "\n";
		//cout << "Generation " << g << ":\n";

		for (row = 0; row < board.size(); row++)
		{	
			for (col = 0; col < colSize; col++)
			{

				// count of alive neighbors
				aliveNeighbors = 0;

				// pattern
				// 1 2 3
				// 4 X 9
				// 6 7 8

				// case for cell "X" is not on the border & corners (no wrap around needed)
				if (row > 0 && row < board.size() - 1 && col > 0 && col < colSize - 1)
				{
					// 1 
					if (board[row - 1][col - 1] == 'x') { aliveNeighbors++; }

					// 2
					if (board[row - 1][col] == 'x') { aliveNeighbors++; }

					// 3
					if (board[row - 1][col + 1] == 'x') { aliveNeighbors++; }

					// 4
					if (board[row][col - 1] == 'x') { aliveNeighbors++; }

					// 6
					if (board[row + 1][col - 1] == 'x') { aliveNeighbors++; }

					// 7
					if (board[row + 1][col] == 'x') { aliveNeighbors++; }

					// 8
					if (board[row + 1][col + 1] == 'x') { aliveNeighbors++; }

					// 9
					if (board[row][col + 1] == 'x') { aliveNeighbors++; }
				}

				// case for cell "X" is on the top row but not in the top corners
				else if (row == 0 && col > 0 && col < colSize - 1)
				{
					// 1 wrap around
					if (board[board.size() - 1][col - 1] == 'x') { aliveNeighbors++; }

					// 2 wrap around
					if (board[board.size() - 1][col] == 'x') { aliveNeighbors++; }

					// 3 wrap around
					if (board[board.size() - 1][col + 1] == 'x') { aliveNeighbors++; }

					// 4
					if (board[row][col - 1] == 'x') { aliveNeighbors++; }

					// 6
					if (board[row + 1][col - 1] == 'x') { aliveNeighbors++; }

					// 7
					if (board[row + 1][col] == 'x') { aliveNeighbors++; }

					// 8
					if (board[row + 1][col + 1] == 'x') { aliveNeighbors++; }

					// 9
					if (board[row][col + 1] == 'x') { aliveNeighbors++; }
				}

				// case for cell "X" is on the bottom row but not in the bottom corners
				else if (row == board.size() - 1 && col > 0 && col < colSize - 1)
				{
					// 1 
					if (board[row - 1][col - 1] == 'x') { aliveNeighbors++; }

					// 2
					if (board[row - 1][col] == 'x') { aliveNeighbors++; }

					// 3
					if (board[row - 1][col + 1] == 'x') { aliveNeighbors++; }

					// 4
					if (board[row][col - 1] == 'x') { aliveNeighbors++; }

					// 6 wrap around
					if (board[0][col] == 'x') { aliveNeighbors++; }

					// 7 wrap around
					if (board[0][col - 1] == 'x') { aliveNeighbors++; }

					// 8 wrap around
					if (board[0][col + 1] == 'x') { aliveNeighbors++; }

					// 9 
					if (board[row][col + 1] == 'x') { aliveNeighbors++; }
				}

				// case for cell "X" is on the left side but not in the corners
				else if (col == 0 && row > 0 && row < board.size() - 1)
				{
					// 1 wrap around
					if (board[row - 1][colSize - 1] == 'x') { aliveNeighbors++; }

					// 2
					if (board[row - 1][col] == 'x') { aliveNeighbors++; }

					// 3
					if (board[row - 1][col + 1] == 'x') { aliveNeighbors++; }

					// 4 wrap around
					if (board[row][colSize - 1] == 'x') { aliveNeighbors++; }

					// 6 wrap around
					if (board[row + 1][colSize - 1] == 'x') { aliveNeighbors++; }

					// 7
					if (board[row + 1][col] == 'x') { aliveNeighbors++; }

					// 8
					if (board[row + 1][col + 1] == 'x') { aliveNeighbors++; }

					// 9
					if (board[row][col + 1] == 'x') { aliveNeighbors++; }
				}

				// case for cell "X" is on the right side but not in the corners
				else if (col == colSize - 1 && row > 0 && row < board.size() - 1)
				{
					// 1 
					if (board[row - 1][col - 1] == 'x') { aliveNeighbors++; }

					// 2
					if (board[row - 1][col] == 'x') { aliveNeighbors++; }

					// 3 wrap around
					if (board[row - 1][0] == 'x') { aliveNeighbors++; }

					// 4
					if (board[row][col - 1] == 'x') { aliveNeighbors++; }

					// 6
					if (board[row + 1][col - 1] == 'x') { aliveNeighbors++; }

					// 7
					if (board[row + 1][col] == 'x') { aliveNeighbors++; }

					// 8 wrap around
					if (board[row + 1][0] == 'x') { aliveNeighbors++; }

					// 9 wrap around
					if (board[row][0] == 'x') { aliveNeighbors++; }
				}

				// case for cell "X" is on the left top corner
				else if (row == 0 && col == 0)
				{
					// 1 wrap around
					if (board[board.size() - 1][colSize - 1] == 'x') { aliveNeighbors++; }

					// 2 wrap around
					if (board[board.size() - 1][col] == 'x') { aliveNeighbors++; }

					// 3 wrap around
					if (board[board.size() - 1][col + 1] == 'x') { aliveNeighbors++; }

					// 4 wrap around
					if (board[0][colSize - 1] == 'x') { aliveNeighbors++; }

					// 9 
					if (board[0][1] == 'x') { aliveNeighbors++; }

					// 6 wrap around
					if (board[1][colSize - 1] == 'x') { aliveNeighbors++; }

					// 7
					if (board[1][0] == 'x') { aliveNeighbors++; }

					// 8
					if (board[1][1] == 'x') { aliveNeighbors++; }
				}

				// case for cell "X" is on the right top corner
				else if (row == 0 && col == colSize - 1)
				{
					// 1 wrap around
					if (board[board.size() - 1][col - 1] == 'x') { aliveNeighbors++; }

					// 2 wrap around
					if (board[board.size() - 1][col] == 'x') { aliveNeighbors++; }

					// 3 wrap around
					if (board[board.size() - 1][0] == 'x') { aliveNeighbors++; }

					// 4 
					if (board[0][col - 1] == 'x') { aliveNeighbors++; }

					// 9 
					if (board[0][0] == 'x') { aliveNeighbors++; }

					// 6 
					if (board[1][col - 1] == 'x') { aliveNeighbors++; }

					// 7
					if (board[1][col] == 'x') { aliveNeighbors++; }

					// 8 wrap around
					if (board[1][0] == 'x') { aliveNeighbors++; }
				}

				// case for cell "X" is on the left bottom corner
				else if (row == board.size() - 1 && col == 0)
				{
					// 1 wrap around
					if (board[row - 1][colSize - 1] == 'x') { aliveNeighbors++; }

					// 2 
					if (board[row - 1][col] == 'x') { aliveNeighbors++; }

					// 3 
					if (board[row - 1][col + 1] == 'x') { aliveNeighbors++; }

					// 4 wrap around
					if (board[row][colSize - 1] == 'x') { aliveNeighbors++; }

					// 9 
					if (board[row][col + 1] == 'x') { aliveNeighbors++; }

					// 6 wrap around
					if (board[0][colSize - 1] == 'x') { aliveNeighbors++; }

					// 7 wrap around
					if (board[0][0] == 'x') { aliveNeighbors++; }

					// 8 wrap around
					if (board[0][1] == 'x') { aliveNeighbors++; }
				}

				// case for cell "X" is on the right bottom corner
				else if (row == board.size() - 1 && col == colSize - 1)
				{
					// 1 
					if (board[row - 1][col - 1] == 'x') { aliveNeighbors++; }

					// 2 
					if (board[row - 1][col] == 'x') { aliveNeighbors++; }

					// 3 wrap around
					if (board[row - 1][0] == 'x') { aliveNeighbors++; }

					// 4 
					if (board[row][col - 1] == 'x') { aliveNeighbors++; }

					// 9 wrap around
					if (board[row][0] == 'x') { aliveNeighbors++; }

					// 6 wrap around
					if (board[0][col - 1] == 'x') { aliveNeighbors++; }

					// 7 wrap around
					if (board[0][col] == 'x') { aliveNeighbors++; }

					// 8 wrap around
					if (board[0][0] == 'x') { aliveNeighbors++; }
				}

				// Rule 1
				if (board[row][col] == '.' && aliveNeighbors == 3) { nextBoard[row][col] = 'x'; }

				// Rule 2
				if ((board[row][col] == 'x' && aliveNeighbors == 2) || (board[row][col] == 'x' && aliveNeighbors == 3)) { nextBoard[row][col] = 'x'; }

				// Rule 3 & 4
				if ((board[row][col] == 'x' && aliveNeighbors < 2) || (board[row][col] == 'x' && aliveNeighbors > 3)) { nextBoard[row][col] = '.'; }

				//cout << nextBoard[i][j];
			}
			//cout << "\n";
		}
		board = nextBoard;
	}
	return board;
}

vector<vector<char>> gol_omp_old(vector<vector<char>> board, int generations, int threads)
{
	vector<vector<char>> nextBoard = board;
	int aliveNeighbors = 0;
	int colSize = board[0].size();
	int row, col;

	// Optimization: creating a pre-calculated array of indices that correspond to the neighbors of a given cell
	//const int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	//const int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

	for (int g = 1; g <= generations; g++)
	{
		#pragma omp parallel for collapse(2) private(row, col, aliveNeighbors) schedule(dynamic) num_threads(threads)
		for (int row = 0; row < board.size(); row++)
		{
			for (int col = 0; col < colSize; col++)
			{
				int aliveNeighbors = 0;

				// Optimization: Wrapping the indices using modulo operator instead of many if-statements
				int row_1 = (row - 1 + board.size()) % board.size();
				int row_2 = (row + 1 + board.size()) % board.size();
				int col_1 = (col - 1 + colSize) % colSize;
				int col_2 = (col + 1 + colSize) % colSize;

				// 1 
				if (board[row_1][col_1] == 'x') { aliveNeighbors++; }

				// 2
				if (board[row_1][col] == 'x') { aliveNeighbors++; }

				// 3
				if (board[row_1][col_2] == 'x') { aliveNeighbors++; }

				// 4
				if (board[row][col_1] == 'x') { aliveNeighbors++; }

				// 6
				if (board[row_2][col_1] == 'x') { aliveNeighbors++; }

				// 7
				if (board[row_2][col] == 'x') { aliveNeighbors++; }

				// 8
				if (board[row_2][col_2] == 'x') { aliveNeighbors++; }

				// 9
				if (board[row][col_2] == 'x') { aliveNeighbors++; }

				// Optimization: to reduce the number of if statements by using a pre-calculated array of indices
				// this way you can iterate through the array of indices rather than writing out separate if statements for each neighbor
				/*for (int i = 0; i < 8; i++)
				{
					int new_row = (row + dx[i] + board.size()) % board.size();
					int new_col = (col + dy[i] + colSize) % colSize;
					if (board[new_row][new_col] == 'x') { aliveNeighbors++; }
				}*/

				// Rule 1
				if (board[row][col] == '.' && aliveNeighbors == 3) { nextBoard[row][col] = 'x'; }

				// Rule 2
				if ((board[row][col] == 'x' && aliveNeighbors == 2) || (board[row][col] == 'x' && aliveNeighbors == 3)) { nextBoard[row][col] = 'x'; }

				// Rule 3 & 4
				if ((board[row][col] == 'x' && aliveNeighbors < 2) || (board[row][col] == 'x' && aliveNeighbors > 3)) { nextBoard[row][col] = '.'; }

			}
		}
		board = nextBoard;
	}
	return board;
}

char* gol_seq_1d(char* board, int generations, int threads, int boardSize, int colSize)
{
	// Optimization:
	// Instead of using a 2D vector a 1D array is used to map the 2D coordinates to 1D coordinates
	// this would reduce the memory overhead and cache misses
	// + Wrapping the indices and calculate the neighbors using modulo operator instead of many if-statements
	// see gol_omp_old() for other approaches 

	char* nextBoard = new char[boardSize * colSize];

	for (int g = 1; g <= generations; g++)
	{
		for (int row = 0; row < boardSize; row++)
		{
			for (int col = 0; col < colSize; col++)
			{
				int aliveNeighbors = 0;

				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						if (i != 0 || j != 0)
						{
							int currRow = (row + i + boardSize) % boardSize;
							int currCol = (col + j + colSize) % colSize;
							if (board[currRow * colSize + currCol] == 'x')
								aliveNeighbors++;
						}
					}
				}

				if (board[row * colSize + col] == '.' && aliveNeighbors == 3)
				{
					nextBoard[row * colSize + col] = 'x';
				}
				else if ((board[row * colSize + col] == 'x' && aliveNeighbors == 2) || (board[row * colSize + col] == 'x' && aliveNeighbors == 3))
				{
					nextBoard[row * colSize + col] = 'x';
				}
				else
				{
					nextBoard[row * colSize + col] = '.';
				}
			}
		}
		std::swap(board, nextBoard);
	}
	delete[] nextBoard;
	return board;
}

char* gol_omp_1d(char* board, int generations, int threads, int boardSize, int colSize)
{
	// Optimization:
	// Instead of using a 2D vector a 1D array is used to map the 2D coordinates to 1D coordinates
	// this would reduce the memory overhead and cache misses
	// + Wrapping the indices and calculate the neighbors using modulo operator instead of many if-statements
	// see gol_omp_old() for other approaches 

	char* nextBoard = new char[boardSize * colSize];

	for (int g = 1; g <= generations; g++)
	{
		#pragma omp parallel for num_threads(threads) schedule(dynamic) collapse(2)
		for (int row = 0; row < boardSize; row++)
		{
			for (int col = 0; col < colSize; col++)
			{
				int aliveNeighbors = 0;

				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						if (i != 0 || j != 0)
						{
							int currRow = (row + i + boardSize) % boardSize;
							int currCol = (col + j + colSize) % colSize;
							if (board[currRow * colSize + currCol] == 'x')
								aliveNeighbors++;
						}
					}
				}

				if (board[row * colSize + col] == '.' && aliveNeighbors == 3)
				{
					nextBoard[row * colSize + col] = 'x';
				}
				else if ((board[row * colSize + col] == 'x' && aliveNeighbors == 2) || (board[row * colSize + col] == 'x' && aliveNeighbors == 3))
				{
					nextBoard[row * colSize + col] = 'x';
				}
				else
				{
					nextBoard[row * colSize + col] = '.';
				}
			}
		}
		std::swap(board, nextBoard);
	}
	delete[] nextBoard;
	return board;
}

int main(int argc, char* argv[])
{
	// Get Singleton instance
	Timing* timing = Timing::getInstance();
	// Start recording the setup time
	timing->startSetup();

	string loadFileName;
	string saveFileName;
	int generations = 1;
	bool measure = false;
	int counter;
	vector<vector<char>> board;
	bool omp = false;
	bool seq = false;
	int threads = 1;

	// Handle command line parameters
	if (argc == 1)
		cout << "\nNo Extra Command Line Argument Passed Other Than Program Name";
	if (argc >= 2)
	{
		cout << "\nNumber Of Arguments Passed: " << argc;
		cout << "\n----Following Are The Command Line Arguments Passed----";

		for (counter = 0; counter < argc; counter++)
			printf("\nargv[%d]: %s", counter, argv[counter]);

		for (counter = 0; counter < argc; counter++)
		{
			if (argv[counter] == NULL)
				break;

			if (string(argv[counter]) == "--load" && counter + 1 < argc)
				loadFileName = argv[counter + 1];

			if (string(argv[counter]) == "--save" && counter + 1 < argc)
				saveFileName = argv[counter + 1];

			if (string(argv[counter]) == "--generations" && counter + 1 < argc)
				generations = atoi(argv[counter + 1]);

			if (string(argv[counter]) == "--measure")
				measure = true;

			if (string(argv[counter]) == "--mode" && counter + 1 < argc)
				if(string(argv[counter + 1]) == "omp")
					omp = true;
				else
					seq = true;

			if (string(argv[counter]) == "--threads" && counter + 1 < argc)
				threads = atoi(argv[counter + 1]);
		}
	}

	if (loadFileName.empty())
	{
		cout << "no filename given to load";
		return EXIT_SUCCESS;
	}

	if (saveFileName.empty())
	{
		cout << "no filename given to save";
		return EXIT_SUCCESS;
	}

	// Load board
	board = loadBoard(loadFileName);
	if (board.empty()) return EXIT_SUCCESS;

	// Stop recording the setup time
	timing->stopSetup();

	// Convert 2D vector to 1D for a faster calculation
	int rows = board.size();
	int cols = board[0].size();
	char* board_1D = new char[rows * cols];
	for (int row = 0; row < rows; row++)
		for (int col = 0; col < cols; col++)
			board_1D[row * cols + col] = board[row][col];

	// Calculate new board after n generations
	if (omp == true)
	{	
		// Start recording the Calculation time
		timing->startComputation();

		board_1D = gol_omp_1d(board_1D, generations, threads, board.size(), board[0].size());
		
		// Stop recording the Calculation time
		timing->stopComputation();
	}

	if (seq == true)
	{
		// Start recording the Calculation time
		timing->startComputation();

		board_1D = gol_seq_1d(board_1D, generations, threads, board.size(), board[0].size());

		// Stop recording the Calculation time
		timing->stopComputation();
	}

	// Start recording the Finalization time
	timing->startFinalization();

	// Convert 1D array back to 2D vector
	vector<vector<char>> newBoard(board.size(), vector<char>(board[0].size()));
	for (int row = 0; row < board.size(); row++)
		for (int col = 0; col < board[0].size(); col++)
			newBoard[row][col] = board_1D[row * board[0].size() + col];

	cout << "\nGeneration " << generations;

	// Save new board to file
	saveBoard(saveFileName, newBoard);

	// Stop recording the Finalization time
	timing->stopFinalization();

	// Save and output this string
	string output = timing->getResults();
	cout << "\nSetup time | Calculation time | Finalization time";
	cout << "\n" << output << "\n";

	return EXIT_SUCCESS;
}

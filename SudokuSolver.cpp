/* This is a program that finds solutions to sudoku problems
*
* Written By: Cedric Blake
*
*/

/*Pseudo code
*
* test if all values on the row and column are not the same as the current value (only have to test coordinates that come before the current coordinate)
* 	1) if so, place the value on the stack and on the current coordinates and test if the current location is at the end of the row
* 		1a) if not, then move right on the row by 1 space and start over from step 1 (RESTART)
* 		1b) if so, then test if we are on the last row of the vector (the next vector in the vector of vectors)
* 			1ba) if not, then move to the first element of the next vector and start over from step 1 (RESTART)
*			1bb) if so, then print the values of the vector of vectors and end program (ENDING CONDITION)
* 	2) otherwise, test if the current value being placed is equal to the value of the users input for the size of the grid
* 		2a) if not, then incriment the value and start over from step 1 (RESTART)
* 		2b) if so, then we must move back by one space, retreave the top value from the stack, incriment that value by 1 and start over from step 1 (SHIFT BACK AND RESTART)
* 
*/

//std::this_thread::sleep_for (std::chrono::seconds(1)); slows down the output to the console by delaying the program. useful for debugging infinite loops
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <thread>
#include <chrono>
#include <map>

using std::cout;
using std::vector;

//dont deal with the stack yet here
//this function tests if the value is repeated along the row or column that it lies.
bool rowsAndCols(vector<vector<int>>& matrix, vector<vector<int>>::iterator y, vector<int>::iterator x, int v) {  //dont want to mess with the original values of cordX/ cordY
	vector<int> testVector; //will be used to test the elements of a column
	//this nested for-loop tests if there are any values on the row that are the same as the current value
	int count = 0; //will count how many spaces x is from the begining of the row, used for column comparison
	for (auto k = y->begin(); k != x; ++k) { //ges up until the current value of x (dont want to check x with itself)
		if (*k == v) {
			return true;
		}
		count++;
	}

	//checks the elements of a column to see if any one on them are equal to the current value
	for (auto i = matrix.begin(); i != y; ++i) {  //stop at y, we dont have to test entire matrix for every element, as we add elements in order from coordinate (0,0) to ("square","square")
		auto j = i->begin() + count; //offset the j pointer by count
		if (v == *j){
			return true;
		}
	}

	return false;
}

//to generalize the subsquare definition, we shall allow the offsets of the begining and the end of the for loops to be dictated by the users input. we should add to a variable dedicated as the arguement of this function
//the value we add to the arguement is the root of the users input, for the size of the rows and columns of every subsquare is equal to the root of the user's input
//also, we must run this function for the same amount of times as we have subsquares. therefore, we must run it "user input" times.

//subRow is the right offset of the subsquare and subCol is the down offset of subSquare. this will be (root * location of subRow/ subCol) for each parameter
//subRow/ subCol will be integers from 0 to "root"

//TODO: need to fix this function. it causes an infinite loop when backtracking. and for whatever reason, it wont allow me to add a variable to the pointers for offsetting. maybe add the offset as you did in the first fucntion?
//TODO: a 25 by 25 grid was giving me an infinite loop when the 9 by 9 grid worked fine (without checking subsquares). why is this?
bool subSquares(vector<vector<int>>& matrix, int v, int r, int sRow, int sCol) {
	int countEqual = 0; //this is to test if there is more than 1 version of a value within a subsquare
	// have to check all values to the end of the subsquare
	int offsetRow = sRow*r;
	int offsetCol = sCol*r;
	//cout<<"sRow is: "<<sRow<<"\n";
	//cout<<"offsetRow is: "<<offsetRow<<"\n";
	//cout<<"the end address is: "<<&(*(matrix.begin() + offsetRow + r))<<"\n";

	for (auto i = matrix.begin() + offsetRow; i != matrix.begin() + offsetRow + r; ++i) { //multiply the truncated value to achieve the correct offset
		for (auto j = i->begin() + offsetCol; j != i->begin() + (offsetCol + r); ++j) {
			//cout<<*j<<" ";
			if (*j == v){
				countEqual++;
				if (2 == countEqual){
					return true; //if the condition is true, then there is multiple instances of the same value and the set wont work
				}
			}
		}
		//cout<<"\n";
	}
	return false;
}

int main(int argc, char** argv) {
	if (1 == argc){
		cout<<"Invalid Input: no arguement found\n";
		return 1;
	} else if (3 <= argc) { //this will be changed once I want to impliment user input for the values of the vector
		cout<<"Invalid Input: Too many arguements\n";
		return 3;
	}

	//tests if the arguement is a perfect square
	const int square = std::stoi(std::string(argv[1]));
	const int root = sqrt(square);  //arguement[0] is the main function itself
	if (root != sqrt(square)) {
		cout<<"Invalid Input: Arguement is not a perfect square\n";
		return 2;
	}

	vector<vector<int>> grid(square, vector<int>(square,0)); //creates a vector of vectors

	int value = 1; //this will be the value we place and test
	std::stack<int> bTrack; //this is where successful values will be saved
	std::stack<std::map<int,bool>> memory; //this stack will check whether or not a value has already been tested and failed (i.e. lead to a dead end when testing)
	std::map<int,bool> elementMem; //will push the map value into "memory" for every time we find a successful number.
	// for (int i = 1; i < 10; ++i) {
	// 	 elementMem[i] = false; //keys are the integers 0 - 9, and they are all initialized to "false"
	// }
	//the current row we are on (which vector in the vector of vectors)
	int subRow = 0;
	int subCol = 0;
	//countRow/ countCol is used to count the current row and columb we are on. used for the subSquares function
	int countRow = 0;

	for (auto cordY = grid.begin(); cordY != grid.end();){ //the current row we are on (which vector in the vector of vectors), we dont define incriment in the for loop definition so we have more control over the location of the coordinate plane
		int countCol = 0;
		subRow = countRow / root;
		for (auto cordX = cordY->begin(); cordX != cordY->end();){ //the current column we are on (element of a vector in the vector of vectors)
			//std::this_thread::sleep_for (std::chrono::milliseconds(25));
			*cordX = value; //moving this here will make test conditions easier to define if the element is at the current location of the iteration 
			subCol = countCol / root; //truncates such that we stay in the prefered subsquare for intermediate values of cordX 
			if (false == rowsAndCols(grid, cordY, cordX, value) and false == subSquares(grid, value, root, subRow, subCol)) { //dont need to backtrack all the way if second condition isnt met, just try a new value
				bTrack.push(value);
				elementMem[value] = true; //"elementMem" is just a place holder. The real values will be stored on the "memory" stack
				memory.push(elementMem); //when we find a value works, we push it to the stack. if we find out the value leads to a dead end, we simply retrieve the map and store another value that works within it and repeat the process.
				elementMem[value] = false; //we do not want to copy truth values into different maps, so be careful of this
				cordX++;
				value = 1;
				countCol++; //incriment countCol only after we have checked that the above condition is met and we move to the next column
			} else {
				if (square != *cordX) { //if "value" is less than "square", then we only incriment the current value by 1 and test it
					value++;
				} else { //if "value" equals square, then we must backtrack
					*cordX = 0;
					do{
						//we nest this while loop within the do-while loop so that if the previous value was "square" and the next value is a value such that itself and every other value up to square has already
						//been tested for that particular element, then we can backtrack again
						while (square == value){ //the "memory" stack must track the "bTrack" stack. if we pop a value from "bTrack", we must also pop a value from "memory"

							//on the first runthrough of the while loop, we have not changed value yet. therefore we should still be on the current element, not the previous one
							//the value we take off the stack is of the element 1 space before the current element, which means we cannot take from the stack if we are at the first element of the grid
							if (cordX == cordY->begin()) { // if the value at the begining of the row is equal to "square", then we must attempt to move to the previous row
								if (cordY == grid.begin()) { //the fail condition, if the first element is equal to "square" and it doesnt work, then the problem has no solution
									cout<<"This problem has no solution";
									return 0;
								} else { //if this condition is met, then we must jump to the previous row, we must remember this 
									cordY--; //because we changed the position of cordY, we need to reinitialize cordX. this changes cordY without jumping out of the inner loop
									countRow--;
									cordX = cordY->end() - 1;
									countCol = square - 1; //places countCol at the "same place" as cordX
								} 
							} else { //if we are not at the first element in the row, then we move back 1 column
								cordX--;
								countCol--;
							}

							value = bTrack.top();
							bTrack.pop();
							elementMem = memory.top();
							memory.pop();

						}
						value++; //if this value is a key that is equal to "true" in elementMem, thensimply incriment by 1 until it either works, or it is equal to "square"
					} while (elementMem[value] == true); //if we have used this value before, then the value will be "true" in the current "elementMem" map

				}
			}
			
		}
		cordY++;
		countRow++;
	}

	//Print the Grid
	for (int i = 0; i != grid.size(); ++i) {
		for (int j = 0; j != grid[0].size(); ++j){
			cout<<std::to_string(grid[i][j])<<" ";
		}
		cout<<"\n";
	}



	// std::stack<int> reverse;
	// //change the order of the stack to print correctly
	// for (int i = 0; i != square; ++i){
	// 	for (int j = 0; j != square; ++j) {
	// 		reverse.push(bTrack.top());
	// 		bTrack.pop();
	// 	}
	// }

	// //print the stack
	// for (int i = 0; i != square; ++i){
	// 	for (int j = 0; j != square; ++j) {
	// 		cout<<reverse.top()<<" ";
	// 		reverse.pop();
	// 	}
	// 	cout<<"\n";
	// }

	//TODO: backtracking causes an infinite loop. need to fix this
	return 0;
}
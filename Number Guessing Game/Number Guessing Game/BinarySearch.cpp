#include "BinarySearch.h"
#include <iostream>

using namespace std;

BinarySearch::BinarySearch()
{
	while (!gameOver)
	Start();
}

void BinarySearch::Start()
{
	int inputNumber;
	cout << "Enter a max and min number: ";
	cin >> maxNum;
	cin >> minNum;
	cin.get();
	if (minNum >= maxNum-1)
	{
		cout << "*min must be smaller than max and there must be room for at least one more number in between\n";
		return;
	}
	//player inputs the number they would like the computer to guess
	cout << "enter a number between " << minNum << " and " << maxNum << "... dont worry, my eyes are closed." << endl << "I can't see what your typing" << endl;
	cin >> inputNumber;
	cin.get();
	//handle numbers that are out of bounds
	if (inputNumber <= minNum)
	{
		cout << "*that number is too small\n";
		return;
	}
	if (inputNumber >= maxNum)
	{
		cout << "*that number is too big\n";
		return;
	}

	cout << "I will now try to figure out what your number is\n" << endl;

	AskIfHigherOrLower(inputNumber);

}

//ask the player if their number is higher or lower than the current guess
void BinarySearch::AskIfHigherOrLower(int userNumber)
{
	static int currentGuess = NarrowGuess(0);
	int previousGuess;

	while (currentGuess != userNumber)
	{
		cout << "Is your number higher or lower than " << currentGuess << "?" << endl;
		cout << "1: Higher" << endl
			<< "2: Lower" << endl;
		//use the player's input to narrow the search
		int temp;
		cin >> temp;
		cin.get();
		previousGuess = currentGuess;
		currentGuess = NarrowGuess(temp);

		if (previousGuess == currentGuess)
		{
			//user keeps inputing incorrect data
		}
		cout << endl;
	}
	cout << "Your number was " << currentGuess << " wasn't it!!!" << endl;
	gameOver = true;

}

//utilize a binary search algorithm to look for the user's number
int BinarySearch::NarrowGuess(int direction)
{
	static int highNumber = maxNum;
	static int lowNumber = minNum;
	static int middleNumber = (maxNum + minNum) / 2;
	int lastNumberGuessed = middleNumber;

	if (direction == 1)
	{
		//limit the possibilities to the top half of the remaining numbers
		lowNumber = middleNumber;
		middleNumber = (highNumber + lowNumber) / 2;
	}
	if (direction == 2)
	{
		//limit the possibilities to the bottom half of the remaining numbers
		highNumber = middleNumber;
		middleNumber = (highNumber + lowNumber) / 2;
	}
	//check if the number changed
	if (lastNumberGuessed == middleNumber)
	{
		//user answered incorrectly on one of their answers
		if (direction == 1)
		{
			highNumber = maxNum;
			middleNumber = (highNumber + lowNumber) / 2;
		}
		if (direction == 2)
		{
			lowNumber = minNum;
			middleNumber = (highNumber + lowNumber) / 2;
		}
	}
	return middleNumber;
}


BinarySearch::~BinarySearch()
{
}

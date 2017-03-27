#pragma once
class BinarySearch
{
public:
	BinarySearch();
	void AskIfHigherOrLower(int userNumber);
	int NarrowGuess(int direction);
	void Start();
	~BinarySearch();

private:
	int minNum;
	int maxNum;
	bool gameOver = false;
};


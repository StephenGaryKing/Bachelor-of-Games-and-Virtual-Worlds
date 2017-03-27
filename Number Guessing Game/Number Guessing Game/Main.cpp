#include <iostream>
#include"BinarySearch.h"

using namespace std;

int main()
{
	BinarySearch* binarySearch;

	char input;
	cout << "choose your search method\n1:binary search\n";
	cin >> input;

	switch (input)
	{
	case '1':
		binarySearch = new BinarySearch();
		break;
	case '2':

		break;
	case '3':

		break;
	}
	delete binarySearch;
	cin.ignore();
	cin.get();
	return 0;
}

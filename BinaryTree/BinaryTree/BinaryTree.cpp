//////////////////////////////////
// Algorithms & Data Structures //
//                              //
//       Binary Trees           //
//     Written Ian Wigley       //
//                              //
//                              //
//////////////////////////////////

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// Struct to hold each word from the .PTX file
struct Node
{
	int value;
	string word; 
	Node* pLeft;
	Node* pRight;
};

// Struct to hold the word frequency information
struct Statistics
{
    int totalWords;
    int distinctWords;       
    int uniqueWords;
	int multiplyUsedWords;
};

// Declare the Prototypes
string readLine(istream& fin);
void outputResults(struct Results stats);
void gotoxy(int x, int y);
void display(Node* tree);
void deleteTree(Node* tree);

Node* readParsedFile(string fName, struct Node* tree);
Node* add(Node* tree, string word);
Node* addNode(Node* tree, Node* toAdd);

Node* copyTree(Node* tree, Node* freqOrder);
Node* addFreq(Node* freqOrder, Node* tree);
Node* addFreqNode(Node* freqOrder, Node* toAdd);

void calculateStats(Node* freqOrder, struct Statistics &stats);
void displayStats(struct Statistics &stats);

// Entry point - main function 
void main()
{
	string fileName;

	Node* tree = 0;
	Node* freqOrder = 0;
	
	Statistics stats;
	stats.distinctWords = 0;
	stats.totalWords = 0;
	stats.uniqueWords = 0;
	stats.multiplyUsedWords = 0;

	string choiceStr;
	char choice;

	cout << "Please enter the filename : ";
	cin >> fileName;
	tree = readParsedFile(fileName, tree);

	if (tree == 0)
	{
		cout << "Error, can't open file" << endl;
	}
	else
	{
		freqOrder = copyTree(tree, freqOrder);
        calculateStats(freqOrder, stats);
		do
		{
			cout << "Enter a - to display alphabetic list" << endl;
			cout << "Enter f - to display frequency ordered list" << endl;
			cout << "Enter s - to display statistics" << endl;
			cout << "Enter x - to exit\n" << endl;
			cout << "Enter choice: ";
			cin >> choiceStr;
			cout << endl;

			if(choiceStr.length() == 0)
			{
				choice = 'e';
			}
			else
			{
				choice = choiceStr[0];
			}

			switch(choice)
			{
			case 'a':
			case 'A':
				display(tree);
				cout << endl;
				break;
			case 'f':
			case 'F':
				display(freqOrder);
				cout << endl;
				break;
			case 's':
			case 'S':
				displayStats(stats);
				cout << endl;
				break;
			case 'x':
			case 'X':
				deleteTree(tree);
				deleteTree(freqOrder);
				break;
			default:
				cout << "Error, invalid choice";
				break;
			}
		}while(choice != 'x');
	}
}

// Function to read a line from an input stream as a string
string readLine(istream& fin)
{
	char buff[256];

	// Delimit the line by end of line char
	if(fin.peek()=='\n')
	{
		fin.ignore();
	}
	fin.getline(buff, 256, '\n');
	return string(buff);
}


//Function to read the intermediate file and process the words
Node* readParsedFile(string fName, struct Node* tree)
{
	string word;

	// Open file
	ifstream inFile(fName.c_str());

	// See if succeeded
	if(!inFile)
	{
		return tree;
	}
	else
	{
		// Succeeded - read file
		while(!inFile.eof())
		{
			word = readLine(inFile);
			tree = add(tree, word);
		}
	}
	return tree;
}

// Function to create a new Node
Node* add(Node* tree, string word)
{
	Node* tempPtr = new Node;
	tempPtr->word = word;
	tempPtr->pLeft  = 0;
	tempPtr->pRight = 0;
	tempPtr->value = 1;
	return addNode(tree, tempPtr);
}

// Function to add the words and frequencies to the newly created Node
Node* addNode(Node* tree, Node* toAdd)
{
	if (tree == 0)
	{
		return toAdd;
	}
	else if (toAdd->word != "")
	{
		// Insert recursively into Tree
		if(toAdd->word < tree->word && toAdd->word != tree->word)
		{
			tree->pLeft = addNode(tree->pLeft, toAdd);
			return tree;
		}

		else if(toAdd->word > tree->word && toAdd->word != tree->word)
		{
			tree->pRight = addNode(tree->pRight, toAdd);
			return tree;
		}

		// If the word already exists then don't add it, just increment the frequency counter
		if(toAdd->word == tree->word)
		{
			tree->value ++;
			return tree;
		}
	}
	return tree;
}

// Function to display the Words tree to the screen
void display(Node* tree)
{
	if (tree != 0)
	{
		display(tree->pLeft);
		cout << " (" << tree->value << ") " << tree->word  << endl;
		display(tree->pRight);
	}
}

// Function to copy the alphabetically ordered tree into a frequency ordered tree
Node* copyTree(Node* tree, Node* freqOrder)
{
	if (tree != 0)
	{
		freqOrder = addFreq(freqOrder, tree);
		copyTree(tree->pLeft, freqOrder);

		copyTree(tree->pRight, freqOrder);
	}
	return freqOrder;
}


// Function to create a new Node
Node* addFreq(Node* freqOrder, Node* tree)
{
	Node* tempPtr1 = new Node;
	tempPtr1->value = tree->value;
	tempPtr1->word = tree->word;
	tempPtr1->pLeft  = 0;
	tempPtr1->pRight = 0;
	return addFreqNode(freqOrder, tempPtr1);
}


// Function to add the words and frequencies to the newly created Node
Node* addFreqNode(Node* freqOrder, Node* toAdd)
{
	if (freqOrder == 0)
	{
		return toAdd;
	}
	else
	{
		// Insert recursively into frequency tree
		if(toAdd->value < freqOrder->value)
		{
			freqOrder->pLeft = addFreqNode(freqOrder->pLeft, toAdd);
			return freqOrder;
		}

		else if(toAdd->value > freqOrder->value)
		{
			freqOrder->pRight = addFreqNode(freqOrder->pRight, toAdd);
			return freqOrder;
		}

		// If the frequency already exists then 
		if(toAdd->value == freqOrder->value)
		{

			// Insert recursively
			if(toAdd->word < freqOrder->word)
			{
				freqOrder->pLeft = addFreqNode(freqOrder->pLeft, toAdd);
				return freqOrder;
			}

			else if(toAdd->word > freqOrder->word)
			{
				freqOrder->pRight = addFreqNode(freqOrder->pRight, toAdd);
				return freqOrder;
			}
		}
	}
	return freqOrder;
}



// Function to step through the Frequency ordered tree & gather up the required values
void calculateStats(Node* freqOrder, struct Statistics &stats)
{
	if (freqOrder != 0)
	{
		calculateStats(freqOrder->pLeft, stats);
		stats.distinctWords++;
		stats.totalWords += freqOrder->value;
		if (freqOrder->value == 1)
        {
        	stats.uniqueWords++;
        }
		else if (freqOrder->value > 1)
		{
			stats.multiplyUsedWords++;
		}
		calculateStats(freqOrder->pRight, stats);
	}
}

// Function to calculate and display the statistical values
void displayStats(struct Statistics &stats)
{
	float distinctPercent = 0;
	float uniquePercent = 0;
	float uniquePercentDistinct = 0;

	distinctPercent = (float)(stats.distinctWords * 100)/(float)stats.totalWords;
	uniquePercent = (float)(stats.uniqueWords * 100)/(float)stats.totalWords;
	uniquePercentDistinct = (float)(stats.uniqueWords * 100)/(float)stats.distinctWords;

	cout << "Total Words found = " << stats.totalWords << endl;
	cout << "Distinct Words found = " << stats.distinctWords << endl;
	cout << "Multiply-Used Words found = " << stats.multiplyUsedWords << endl;
	cout << "Unique Words found = " << stats.uniqueWords << endl;

	cout << setprecision(3);

	cout << setw(4) << "Distinct Words as a % of words = " << distinctPercent << endl;
	cout << setw(4) << "Unique Words as a % of words = " << uniquePercent << endl;
	cout << setw(4) << "Unique Words as a % of distinct words = " << uniquePercentDistinct << endl;
}

// Clean up Function to stop any memory leaks by deleting the tree structs
void deleteTree(Node* tree)
{
	if (tree !=0)
	{
		deleteTree(tree->pLeft);
		deleteTree(tree->pRight);
		delete tree;
	}
}
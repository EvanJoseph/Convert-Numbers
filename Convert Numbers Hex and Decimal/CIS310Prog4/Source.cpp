/*
Evan Joseph
CIS 310, Prog4
4/2/2017
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class Hex
{
private:
	string input;
	string op1;
	string op2;
	string op;
	string answer = "";
	int sum;
	int carry;
	vector<string> sumVector;

public:
	Hex()
	{
		input = "";
		sum = 0;
		carry = 0;
	}

	Hex(string in)
	{
		input = in;
		sum = 0;
		carry = 0;
	}

	int hexToDecimal(char myChar)	//used to convert 1 hex number to decimal so it can be used for arithmetic
	{
		if (myChar == '0' || myChar == '1' || myChar == '2' || myChar == '3' || myChar == '4' || myChar == '5' || 
			myChar == '6' || myChar == '7' || myChar == '8' || myChar == '9')
		{
			int myInt = myChar - 48;
			return myInt;
		}
		else if (myChar == 'A') return 10;
		else if (myChar == 'B') return 11;
		else if (myChar == 'C') return 12;
		else if (myChar == 'D') return 13;
		else if (myChar == 'E') return 14;
		else if (myChar == 'F') return 15;
		else return -1;
	}

	string decimalToHex(int myInt)	//used to convert 1 decimal number to hex so it can be used to print out
	{
		if (myInt >= 0 && myInt <= 9)
		{
			return to_string(myInt);
		}
		else if (myInt == 10) return "A";
		else if (myInt == 11) return "B";
		else if (myInt == 12) return "C";
		else if (myInt == 13) return "D";
		else if (myInt == 14) return "E";
		else if (myInt == 15) return "F";
		else return "";
	}
	
	void findSubstrings()		//splits up the input into op1, op, and op2, example: AABB-D -> AABB, -, D
	{
		if (input != "")
		{
			int opIndex;
			if (input.find("+") != -1)
			{
				opIndex = input.find("+");
			}
			else  if (input.find("-") != -1)
			{
				opIndex = input.find("-");
			}
			else if (input.find("*") != -1)
			{
				opIndex = input.find("*");
			}
			else if (input.find("/") != -1)
			{
				opIndex = input.find("/");
			}
			else if (input.find("$") != -1)
			{
				opIndex = input.find("$");
			}

			op = input.substr(opIndex, 1);		//substr(starting from this index, number of characters to include)
			op1 = input.substr(0, opIndex);
			op2 = input.substr(opIndex + 1, input.length() - opIndex - 2);	//the last character is an "=" so we don't want to include that
		}
	}

	void printSubstrings()		//used for debugging purposes
	{
		cout << op1 << " " << op << " " << op2 << endl;
	}

	void printInput()			//prints the input and answer
	{
		cout << input;
	}

	void reverseVector()		//reverses the vector with the answer in it. so it can be in the right order
	{
		reverse(sumVector.begin(), sumVector.end());
	}

	void updateAnswer()
	{
		answer = "";		//DELETE THIS ????????
		for (int i = 0; i < sumVector.size(); i++)
		{
			answer.append(sumVector[i]);
		}
		op1 = answer;
		sumVector.clear();
	}

	void printAnswer()			//prints the final answer
	{
		cout << answer << endl;
	}

	void callOp()				//checks which arithmetic function to call
	{
		if (op == "+")
		{
			add(op1, op2);
		}
		else if (op == "-")
		{
			subtract(op1, op2);
		}
		else if (op == "*")
		{
			multiply();
		}
		else if (op == "/")
		{
			divide();
		}
		else if (op == "$")
		{
			//power();		//commented out because it runs for several minutes,calls the add function an obscene number of times
		}
	}

	//these 2 convert functions are not used in arithmetic computation. I did that digit-by-digit. This is for boolean expressions and converting various numbers like a loop counter
	int convertLargeHexToDecimal(string myLargeHex)	
	{
		reverse(myLargeHex.begin(), myLargeHex.end());
		int decimal = 0;
		for (int i = 0; i < myLargeHex.length(); i++)
		{
			decimal += (hexToDecimal(myLargeHex[i]) * pow(16,i));
		}
		return decimal;
	}

	string convertLargeDecimalToHex(int myLargeDecimal)
	{
		string hex = "";
		int counter = 0;
		while (myLargeDecimal >= 16)
		{
			int remainder = myLargeDecimal % 16;
			hex.append(decimalToHex(remainder));
			myLargeDecimal /= 16;
			counter++;
		}
		int remainder = myLargeDecimal % 16;
		hex.append(decimalToHex(remainder));

		return hex;
	}

	void add(string operator1, string operator2)	//adds two hex numbers together
	{
		reverse(operator1.begin(), operator1.end());
		reverse(operator2.begin(), operator2.end());

		while (operator1 != "" && operator2 != "")
		{
			sum = hexToDecimal(operator1[0]) + hexToDecimal(operator2[0]) + carry;
			if (sum >= 16)
			{
				sum -= 16;
				carry = 1;
			}
			else
			{
				carry = 0;
			}
			sumVector.push_back(decimalToHex(sum));

			operator1.erase(0, 1);
			operator2.erase(0, 1);
		}
		while (operator1 != "")	//if the hex numbers weren't the same length, one of them will still need to be dealt with
		{
			sum = hexToDecimal(operator1[0]) + carry;
			if (sum >= 16)
			{
				sum -= 16;
				carry = 1;
			}
			else
			{
				carry = 0;
			}
			sumVector.push_back(decimalToHex(sum));

			operator1.erase(0, 1);
		}
		while (operator2 != "")
		{
			sum = hexToDecimal(operator2[0]) + carry;
			if (sum >= 16)
			{
				sum -= 16;
				carry = 1;
			}
			else
			{
				carry = 0;
			}
			sumVector.push_back(decimalToHex(sum));

			operator2.erase(0, 1);
		}
		if (carry == 1)
		{
			sumVector.push_back("1");
			carry = 0;
		}

		reverseVector();		//reverses the sumVector so it can be the correct answer (we were pushing back each hex digit)
		updateAnswer();
	}

	void subtract(string operator1, string operator2)	//subtracts two hex numbers
	{
		reverse(operator1.begin(), operator1.end());
		reverse(operator2.begin(), operator2.end());

		while (operator1 != "" && operator2 != "")
		{
			sum = hexToDecimal(operator1[0]) - hexToDecimal(operator2[0]);
			int j = 1;
			if (sum < 0)
			{
				sum += 16;
				while (operator1[j] == '0')	//when borrowing, 0's can't be borrowed from, so we change to F and skip it to borrow from its neighbor
				{
					operator1[j] = 'F';
					j++;
				}

				if (operator1[j] == 'A')
				{
					operator1[j] = '9';
				}
				else operator1[j]--;				//borrowing
			}
			sumVector.push_back(decimalToHex(sum));
			operator1.erase(0, 1);
			operator2.erase(0, 1);
		}
		while (operator1 != "")
		{
			sum = hexToDecimal(operator1[0]);
			if(!(operator1.size() == 1 && sum == 0)) sumVector.push_back(decimalToHex(sum));	//when there is nothing else to subtract, just bring the digit down unless

			operator1.erase(0, 1);
		}

		reverseVector();		//reverses the sumVector so it can be the correct answer (we were pushing back each hex digit)
		updateAnswer();
	}

	void multiply()
	{
		int op2Decimal = convertLargeHexToDecimal(op2);
		op2 = op1;
		for (int i = 0; i < op2Decimal - 1; i++)	//simulates multiplication by continuously adding
		{
			add(op1, op2);
		}
	}

	void divide()		//simulates division by continuosly subtracting
	{
		string remainder = "";
		int counter = 0;
		subtract(op1, op2);
		counter++;
		subtract(op1, op2);
		counter++;
		while (convertLargeHexToDecimal(op1) > convertLargeHexToDecimal(op2))
		{
			subtract(op1, op2);
			counter++;
		}
		remainder = op1;
		answer = "quotient " + convertLargeDecimalToHex(counter) + ", remainder " + remainder;	//following the format specified in the instructions
	}

	void power()
	{
		//convert op2 to decimal and loop that many times to call multiply()
		//5$4 = 5 ^ 4 = 5 * 5 * 5 * 5 (3 multiply() calls) = 25* 5 * 5 = 125 * 5 = 625
		// = 5+5+5+5+5+...+5 (125 add() calls)
		int op2Decimal = convertLargeHexToDecimal(op2);
		
		//op2 = op1;
		for (int i = 0; i < op2Decimal-1; i++)
		{
			multiply();
		}
	}
};


int main()
{
	fstream inputFile;
	string input;
	inputFile.open("input.txt", ios::in);	//using file input, as specified in the instructions
	if (!inputFile)
	{
		cout << "There was an ERROR opening the file!" << endl;
		return 0;
	}

	while (getline(inputFile, input))
	{
		Hex * myHex = new Hex(input);
		myHex->findSubstrings();
		myHex->printInput();
		myHex->callOp();
		myHex->printAnswer();
	}

	inputFile.close();
	//system("pause");
	return 0;
}
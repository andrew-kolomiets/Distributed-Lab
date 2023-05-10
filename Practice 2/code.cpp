///*********************************************************

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <bitset>
#include <cstdint>
#include <sstream>
#include <algorithm>

using namespace std;

///*********************************************************

uint hexToDec(const string &hexString)
{
	for (char c : hexString)
	{
		if (!std::isxdigit(c))
		{
			throw std::invalid_argument("Invalid hexadecimal number");
		}
	}
	return stoul(hexString, nullptr, 16);
}

string decToHex(uint decimalValue)
{
	stringstream stream;
	stream << hex << decimalValue;
	return stream.str();
}

string toBinaryString(uint num)
{
	string binary = "";
	for (int i = 31; i >= 0; i--)
	{
		if ((num >> i) & 1)
		{
			binary += "1";
		}
		else
		{
			binary += "0";
		}
	}
	// reverse(binary.begin(),binary.end());

	return binary;
}

///*********************************************************
class MyBigInt
{
private:
	vector<uint> number;
	int system;

public:
	void setHex(string &hexString);
	string getHex();

	MyBigInt();
	MyBigInt(string &hexString);
	MyBigInt(const MyBigInt &other);
	~MyBigInt();

	void INV();
	MyBigInt &XOR(MyBigInt &firstNum, MyBigInt &secondNum);
	MyBigInt &OR(MyBigInt &firstNum, MyBigInt &secondNum);
	MyBigInt &AND(MyBigInt &firstNum, MyBigInt &secondNum);
	void shiftR(int size);
	void shiftL(int size);
	void shiftL_(int size);

	MyBigInt &ADD(MyBigInt &firstNum, MyBigInt &secondNum);
	MyBigInt &SUB(MyBigInt &firstNum, MyBigInt &secondNum);

	MyBigInt &MUL(MyBigInt &firstNum, MyBigInt &secondNum);
	MyBigInt &DIV(MyBigInt &firstNum, MyBigInt &secondNum);

	// void MOD(MyBigInt &firstNum, MyBigInt &secondNum);
	// void POWMOD(MyBigInt &firsNum, MyBigInt &secondNum);
};
///*********************************************************

MyBigInt::MyBigInt(string &hexString)
{
	number.clear();
	system = 8 * sizeof(uint);
	uint temp = 0;

	for (int i = hexString.length(); i > 0; i -= system / 4)
	{
		int start_idx = max(i - system / 4, 0);
		string chunk = hexString.substr(start_idx, i - start_idx);

		if (chunk.length() == system / 4)
		{
			number.push_back(hexToDec(chunk));
		}
		else
		{
			while (chunk.length() % (system / 4) != 0)
			{
				chunk = "0" + chunk;
			}
			number.push_back(hexToDec(chunk));
		}
	}
	reverse(number.begin(), number.end());
}

MyBigInt::MyBigInt(const MyBigInt &other)
{
	this->number.clear();
	system = 8 * sizeof(uint);
	this->number = other.number;
}

MyBigInt::MyBigInt()
{
	number.clear();
	system = 8 * sizeof(uint);
}

MyBigInt::~MyBigInt(){

};

///*********************************************************

void MyBigInt::setHex(string &hexString)
{
	number.clear();

	system = 8 * sizeof(uint);

	vector<string> chunks;

	for (int i = hexString.length() - 1; i >= 0; i -= system / 4)
	{
		int start_pos = i - system / 4 + 1;
		if (start_pos < 0)
		{
			start_pos = 0;
		}
		chunks.insert(chunks.begin(), hexString.substr(start_pos, i - start_pos + 1));
	}

	for (uint i = 0; i < chunks.size(); i++)
	{
		// cout<<"+"<<chunks[i]<<endl;
		number.push_back(hexToDec(chunks[i]));
	}
	reverse(number.begin(), number.end());
}

string MyBigInt::getHex()
{

	string temp = "";

	for (int i = 0; i < number.size(); i++)
	{
		string var = decToHex(number[i]);

		// cout<<endl<<var.size()<<" --- "<<system<<endl;
		while (var.length() < (system / sizeof(uint)))
		{
			var = "0" + var;
		}

		temp = var + temp;
	}

	while (temp.size() > 1 && temp[0] == '0')
	{
		temp.erase(0, 1);
	}

	return temp;
}

///*********************************************************

void MyBigInt::INV()
{

	for (int i = 0; i < number.size(); i++)
	{
		number[i] = ~number[i];
	}
}

MyBigInt &MyBigInt::XOR(MyBigInt &firstNum, MyBigInt &secondNum)
{
	if (firstNum.number.size() > secondNum.number.size())
	{
		while (firstNum.number.size() != secondNum.number.size())
		{
			secondNum.number.push_back(0);
		}
	}
	if (firstNum.number.size() < secondNum.number.size())
	{
		while (firstNum.number.size() != secondNum.number.size())
		{
			firstNum.number.push_back(0);
		}
	}

	int size = max(firstNum.number.size(), secondNum.number.size());

	for (int i = 0; i < size; i++)
	{
		(this->number).push_back(firstNum.number[i] ^ secondNum.number[i]);
	}

	return *this;
}

MyBigInt &MyBigInt::OR(MyBigInt &firstNum, MyBigInt &secondNum)
{
	if (firstNum.number.size() > secondNum.number.size())
	{
		while (firstNum.number.size() != secondNum.number.size())
		{
			secondNum.number.push_back(0);
		}
	}
	if (firstNum.number.size() < secondNum.number.size())
	{
		while (firstNum.number.size() != secondNum.number.size())
		{
			firstNum.number.push_back(0);
		}
	}

	int size = max(firstNum.number.size(), secondNum.number.size());

	for (int i = 0; i < size; i++)
	{
		this->number.push_back(firstNum.number[i] | secondNum.number[i]);
	}

	return *this;
}

MyBigInt &MyBigInt::AND(MyBigInt &firstNum, MyBigInt &secondNum)
{
	if (firstNum.number.size() > secondNum.number.size())
	{
		while (firstNum.number.size() != secondNum.number.size())
		{
			secondNum.number.push_back(0);
		}
	}
	if (firstNum.number.size() < secondNum.number.size())
	{
		while (firstNum.number.size() != secondNum.number.size())
		{
			firstNum.number.push_back(0);
		}
	}

	int size = max(firstNum.number.size(), secondNum.number.size());

	for (int i = 0; i < size; i++)
	{
		this->number.push_back(firstNum.number[i] & secondNum.number[i]);
	}

	return *this;
}

void MyBigInt::shiftR(int n)
{

	int cell_shift = n / system;
	int bit_shift = n % system;

	for (int i = 0; i < cell_shift; i++)
	{
		number.erase(number.begin());
	}

	for (int j = 0; j < bit_shift; j++)
	{

		for (int i = 0; i < number.size() - 1 && number.size() != 0; i++)
		{

			if ((number[i + 1]) % 2 == 0)
			{
				number[i] = (number[i]) >> 1;
			}
			else
			{
				number[i] = (number[i]) >> 1;
				number[i] |= 0x80000000;
			}
		}

		number[number.size() - 1] >>= 1;
	}
}

void MyBigInt::shiftL(int n)
{
	int cell_shift = n / system;
	int bit_shift = n % system;

	for (int i = 0; i < cell_shift; i++)
	{
		number.insert(number.begin(), 0);
	}

	for (int i = 0; i < bit_shift; i++)
	{
		bool triger = false;

		for (int j = 0; j < number.size(); j++)
		{
			if (number[j] == 0)
			{
				continue;
			}
			else
			{
				if ((number[j] & 0x80000000) != 0x80000000)
				{
					number[j] = (number[j] << 1);

					if (triger == true)
					{
						number[j] |= 0x1;
						triger = false;
					}
				}
				else
				{
					if (triger == true)
					{
						number[j] &= 0x7FFFFFFF;
						number[j] = (number[j] << 1);
						number[j] |= 0x1;
						triger = true;
					}
					else
					{
						number[j] &= 0x7FFFFFFF;
						number[j] = (number[j] << 1);
						triger = true;
					}
				}
			}
		}

		if (triger == true)
		{
			number.push_back(uint(1));
			triger = false;
		}
	}
}

///*********************************************************
MyBigInt &MyBigInt::ADD(MyBigInt &firstNum, MyBigInt &secondNum)
{
	if (firstNum.number.size() > secondNum.number.size())
	{
		while (firstNum.number.size() != secondNum.number.size())
		{
			secondNum.number.push_back(0);
		}
	}
	if (firstNum.number.size() < secondNum.number.size())
	{
		while (firstNum.number.size() != secondNum.number.size())
		{
			firstNum.number.push_back(0);
		}
	}

	vector<uint> result;

	uint temp, digit;
	uint carry = 0;
	if (system == 32)
	{
		digit = uint(pow(2, system)) ^ 0x1ffffffff;
	}
	else
	{
		digit = uint(pow(2, system)) ^ 0x1ffffffffffffffff;
	}

	for (uint i = 0; i < max(firstNum.number.size(), secondNum.number.size()); i++)
	{
		temp = firstNum.number[i] + secondNum.number[i] + carry;
		result.push_back(temp & digit);
		carry = temp / pow(2, system);
	}

	if (carry != 0)
	{
		result.push_back(carry);
	}

	this->number = result;

	return *this;
}

MyBigInt &MyBigInt::SUB(MyBigInt &firstNum, MyBigInt &secondNum)
{

	if (firstNum.number.size() > secondNum.number.size())
	{
		while (firstNum.number.size() != secondNum.number.size())
		{
			secondNum.number.push_back(0);
		}
	}
	if (firstNum.number.size() < secondNum.number.size())
	{
		while (firstNum.number.size() != secondNum.number.size())
		{
			firstNum.number.push_back(0);
		}
	}

	reverse(firstNum.number.begin(), firstNum.number.end());
	reverse(secondNum.number.begin(), secondNum.number.end());

	vector<uint> result;

	uint temp, digit;
	uint borrow = 0;

	if (system == 32)
	{
		digit = uint(pow(2, system)) ^ 0x1ffffffff;
	}
	else
	{
		digit = uint(pow(2, system)) ^ 0x1ffffffffffffffff;
	}

	for (uint i = 0; i < max(firstNum.number.size(), secondNum.number.size()); i++)
	{
		temp = firstNum.number[i] - secondNum.number[i] - borrow;
		if (temp >= 0)
		{
			result.push_back(temp);
			borrow = 0;
		}
		else
		{
			result.push_back(digit + temp);
			borrow = 1;
		}
	}

	reverse(result.begin(), result.end());

	this->number = result;

	return *this;
}

MyBigInt &MyBigInt::MUL(MyBigInt &firstNum, MyBigInt &secondNum)
{
	if(firstNum.number.size()>secondNum.number.size())
	{
		while(firstNum.number.size()!=secondNum.number.size())
		{
			secondNum.number.push_back(0);
		}
	}
	if(firstNum.number.size()<secondNum.number.size())
	{
		while(firstNum.number.size()!=secondNum.number.size())
		{
			firstNum.number.push_back(0);
		}
	}

	vector <uint> result;


	return *this;
}

MyBigInt &MyBigInt::DIV(MyBigInt &firstNum, MyBigInt &secondNum)
{
	// if(firstNum.number.size()>secondNum.number.size())
	// {
	// 	while(firstNum.number.size()!=secondNum.number.size())
	// 	{
	// 		secondNum.number.push_back(0);
	// 	}
	// }
	// if(firstNum.number.size()<secondNum.number.size())
	// {
	// 	while(firstNum.number.size()!=secondNum.number.size())
	// 	{
	// 		firstNum.number.push_back(0);
	// 	}
	// }

	// vector <uint> result;

	// this->number=result;

	return *this;
}

///*********************************************************
int main()
{

	string test_1 = "51bf608414ad5726a3c1bec098f77b1b54ffb2787f8d528a74c1d7fde6470ea4";
	string test_2 = "403db8ad88a3932a0b7e8189aed9eeffb8121dfac05c3512fdb396dd73f6331c";

	MyBigInt a, b, c, d, e, f;

	a.setHex(test_1);
	b.setHex(test_2);

	cout << a.getHex() << endl;
	cout << "XOR" << endl;
	cout << b.getHex() << endl;
	cout << "Result" << endl;
	c.XOR(a, b);
	cout << c.getHex() << endl;

	cout << endl;

	test_1 = "36f028580bb02cc8272a9a020f4200e346e276ae664e45ee80745574e2f5ab80";
	test_2 = "70983d692f648185febe6d6fa607630ae68649f7e6fc45b94680096c06e4fadb";

	a.setHex(test_1);
	b.setHex(test_2);

	cout << a.getHex() << endl;
	cout << "ADD" << endl;
	cout << b.getHex() << endl;
	cout << "Result" << endl;
	d.ADD(a, b);
	cout << d.getHex() << endl;
	cout << endl;

	test_1 = "33ced2c76b26cae94e162c4c0d2c0ff7c13094b0185a3c122e732d5ba77efebc";
	test_2 = "22e962951cb6cd2ce279ab0e2095825c141d48ef3ca9dabf253e38760b57fe03";

	a.setHex(test_1);
	b.setHex(test_2);

	cout << a.getHex() << endl;
	cout << "SUB" << endl;
	cout << b.getHex() << endl;
	cout << "Result" << endl;
	e.SUB(a, b);
	cout << e.getHex() << endl;
	cout << endl;

	return 0;
}
///*********************************************************

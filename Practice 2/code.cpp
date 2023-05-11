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

int countBits(uint number) {
    int count = 0;
    while (number != 0) {
        count++;
        number >>= 1; 
    }
    return count;
}

void removeTrailingZeros(vector<uint>& vec) {
    while (!vec.empty() && vec.back() == 0) {
        vec.pop_back();
    }
}

///*********************************************************

class MyBigInt
{
private:
	vector<uint> number;
	int system;

public:
	MyBigInt &setHex(string &hexString);
	string getHex();

	MyBigInt();
	MyBigInt(string &hexString);
	MyBigInt(const MyBigInt &other);
	~MyBigInt();

	MyBigInt &operator = (const MyBigInt& other);

	MyBigInt &INV();
	MyBigInt &XOR(MyBigInt &firstNum, MyBigInt &secondNum);
	MyBigInt &OR(MyBigInt &firstNum, MyBigInt &secondNum);
	MyBigInt &AND(MyBigInt &firstNum, MyBigInt &secondNum);
	MyBigInt &shiftR(int size);
	MyBigInt &shiftL(int size);

	MyBigInt &ADD(MyBigInt &firstNum, MyBigInt &secondNum);
	MyBigInt &SUB(MyBigInt &firstNum, MyBigInt &secondNum);

	MyBigInt &MUL(MyBigInt &firstNum, MyBigInt &secondNum);
	void DIV(MyBigInt &firstNum, MyBigInt &secondNum,MyBigInt &R,MyBigInt &Q);

	MyBigInt &MOD(MyBigInt &firstNum, MyBigInt &secondNum);
	// void POWMOD(MyBigInt &firsNum, MyBigInt &secondNum);


	int COMPARE(MyBigInt &firstNum, MyBigInt &secondNum);

	
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
MyBigInt& MyBigInt::operator = (const MyBigInt& other)
{
	this->number=other.number;
	
	return *this;
}
///*********************************************************

MyBigInt & MyBigInt::setHex(string &hexString)
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

	return *this; 
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

MyBigInt & MyBigInt::INV()
{

	for (int i = 0; i < number.size(); i++)
	{
		number[i] = ~number[i];
	}

	return *this;
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

MyBigInt & MyBigInt::shiftR(int n)
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
	
	return *this; 
}

MyBigInt & MyBigInt::shiftL(int n)
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

	return *this; 
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

	uint64_t temp, digit;
	uint carry = 0;
	if (system == 32)
	{
		digit = uint64_t(pow(2, system)) ^ 0x1ffffffff;
	}
	else
	{
		digit = uint64_t(pow(2, system)) ^ 0x1ffffffffffffffff;
	}

	for (uint i = 0; i < max(firstNum.number.size(), secondNum.number.size()); i++)
	{
		temp = (uint64_t)firstNum.number[i] + (uint64_t)secondNum.number[i] + carry;
		result.push_back(temp & digit);
		carry = temp / (uint64_t)pow(2, system);
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

	vector<uint> result;

	uint64_t borrow = 0;

	for (uint i = 0; i < max(firstNum.number.size(), secondNum.number.size()); i++)
	{
		if (firstNum.number[i] >= secondNum.number[i] + borrow)
		{
			result.push_back(firstNum.number[i]-(secondNum.number[i]+ borrow));
			borrow = 0;
		}
		else
		{
			result.push_back(pow(2,system) - ((secondNum.number[i]+ borrow)-firstNum.number[i]));
			borrow = 1;
		}
	}

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

	uint64_t digit;

	if (system == 32)
	{
		digit = uint(pow(2, system)) ^ 0x1ffffffff;
	}
	else
	{
		digit = uint(pow(2, system)) ^ 0x1ffffffffffffffff;
	}

	MyBigInt result;

	for (size_t i = 0; i < (max(firstNum.number.size(), secondNum.number.size())); i++)
	{
		result.number.push_back(0);
	}
	

	for (int i = 0; i < (max(firstNum.number.size(), secondNum.number.size())); i++)
	{
		vector <uint> temp;

		uint64_t carry=0;

		for (int k = 0; k < (max(firstNum.number.size(), secondNum.number.size())); k++)
		{
			uint64_t mask = 0xFFFFFFFF;
			temp.push_back(((((uint64_t)firstNum.number[k]*(uint64_t)secondNum.number[i])&mask)+(( uint64_t)carry&mask)&mask));
			carry=(uint64_t)firstNum.number[k]*(uint64_t)secondNum.number[i]/pow(2,system)+( uint64_t)carry/pow(2,system);
		}
		
		temp.push_back((uint)carry);

		MyBigInt tmp;

		
		for (int j = 0; j < i; j++)
		{
			temp.insert(temp.begin(),0);
		}

		tmp.number=temp;

		result.ADD(tmp,result);

	}

	this->number=result.number;

	return *this;
}

void MyBigInt::DIV(MyBigInt &firstNum, MyBigInt &secondNum,MyBigInt &R,MyBigInt &Q)
{
	removeTrailingZeros(firstNum.number);
	removeTrailingZeros(secondNum.number);

	int k = secondNum.number.size()*system-system+countBits(secondNum.number[secondNum.number.size()-1]);

	R.number=firstNum.number;
	Q.number.push_back(0);

	while(COMPARE(R,secondNum)==1||COMPARE(R,secondNum)==0)
	{
		MyBigInt tmp=secondNum;

		int t = R.number.size()*system-system+countBits(R.number[R.number.size()-1]) ;
		
		tmp.shiftL(t-k);
		
		if((COMPARE(R,tmp)==-1))
		{	
			t=t-1;
			tmp=secondNum;
			tmp.shiftL(t-k);
		}
		R.SUB(R,tmp);

		removeTrailingZeros(R.number);

		MyBigInt temp;
		temp.number.push_back(1);
		temp.shiftL(t-k);
		
		Q.ADD(Q,temp);

		removeTrailingZeros(Q.number);

	}

}

MyBigInt &MyBigInt::MOD(MyBigInt &firstNum, MyBigInt &secondNum)
{
	int k=secondNum.number.size()-1;

	string one="1"; MyBigInt B(one);

	B.shiftL(2*(k+1));

	MyBigInt nu, R,Q;

	nu.DIV(B,secondNum, R,Q);

    nu=;

	MyBigInt q=firstNum;


	if (k-1 >= q.number.size()) 
	{
		q.number.clear();
	} else 
	{
		q.number.erase(q.number.begin(), q.number.begin() + k-1);
	}

	q.MUL(q,nu);

	if (k+1 >= q.number.size()) 
	{
		q.number.clear();
	} else 
	{
		q.number.erase(q.number.begin(), q.number.begin() + k+1);
	}

	MyBigInt r=r.SUB(firstNum,q.MUL(q,secondNum));

	while(r.COMPARE(r,secondNum)==1||r.COMPARE(r,secondNum)==0)
	{
		r.SUB(r,secondNum);
	}

	this->number=r.number;

	return r ;
}

int MyBigInt::COMPARE(MyBigInt &firstNum, MyBigInt &secondNum)
{
	
    if (firstNum.number.size() > secondNum.number.size())
    {
        return 1;
    }
    else if (firstNum.number.size() < secondNum.number.size())
    {
        return -1;
    }

    for (int i = firstNum.number.size() - 1; i >= 0; i--)
    {
        if (firstNum.number[i] > secondNum.number[i])
        {
            return 1;
        }
        else if (firstNum.number[i] < secondNum.number[i])
        {
            return -1;
        }
    }

    // The numbers are equal
    return 0;
	
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

	test_1 = "7d7deab2affa38154326e96d350deee1";
	test_2 = "97f92a75b3faf8939e8e98b96476fd22";

	a.setHex(test_1);
	b.setHex(test_2);

	cout << a.getHex() << endl;
	cout << "MUL" << endl;
	cout << b.getHex() << endl;
	cout << "Result" << endl;
	e.MUL(a, b);
	cout << e.getHex() << endl;
	cout << endl;

	test_1 = "33ced2c76b26cae94e162c4c0d2c0ff7c13094b0185a3c122e732d5ba77efebc";
	test_2 = "97f92a75b3faf8939e8e98b96476fd22";

	a.setHex(test_1);
	b.setHex(test_2);

	MyBigInt R,Q;

	cout << a.getHex() << endl;
	cout << "DIV" << endl;
	cout << b.getHex() << endl;
	cout << "Result" << endl;
	e.DIV(a, b,R,Q);
	cout <<"Reminder: "<< R.getHex() << endl;
	cout <<"Quoter: "<< Q.getHex() << endl;
	cout << endl;


	test_1 = "33ced2c76b26cae94e162c4c0d2c0ff7c13094b0185a3c122e732d5ba77efebc";
	test_2 = "97f92a75b3faf8939e8e98b96476fd22";

	a.setHex(test_1);
	b.setHex(test_2);

	cout << a.getHex() << endl;
	cout << "DIV" << endl;
	cout << b.getHex() << endl;
	cout << "Result" << endl;
	e.MOD(a, b);
	cout << e.getHex() << endl;
	cout << endl;


	

	return 0;
}
///*********************************************************

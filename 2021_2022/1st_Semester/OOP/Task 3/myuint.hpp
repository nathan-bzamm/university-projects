#include <iostream>
#include <math.h>
#include <vector>

std::vector<bool> Integer_to_Binary(int input);

std::vector<bool> reverseBinary(std::vector<bool> back);

void printNum(std::vector<bool> number);

bool isPow2(int number);

int foo();

template < int T>
class bigInt
{
    public:
        std::vector<bool> bigInteger;

        bigInt();
        bigInt(int input);
        
        // void print_bigInt(bigInt<T> bigInteger);

        bigInt<T> operator+(bigInt<T> num);
        bigInt<T> operator-(bigInt<T> num);
        bigInt<T> operator*(bigInt<T> num);
        bigInt<T> operator<<(int num);
        bigInt<T> operator>>(int num);
        bigInt<T> operator/(bigInt<T> num);
        bigInt<T> operator%(bigInt<T> num);

        template<typename type>
        type convertToInteger();
};

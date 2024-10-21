#include <iostream>
#include <vector>
#include <math.h>
#include "myuint.hpp"


std::vector<bool> Integer_to_Binary(int input) //Function used to convert the integer into a binary number
{
    std::vector<bool> binaryNumbers;

    while (input != 0)
    {
        binaryNumbers.push_back((input % 2 == 0 ? false: true));
        input /= 2;
    }

    binaryNumbers = reverseBinary(binaryNumbers); //will reverse the order of the binary numbers to print correctly

    return binaryNumbers;
}

std::vector<bool> reverseBinary(std::vector<bool> back) //function to reverse the order of the binary numbers in order to print correctly
{
    std::vector<bool> correction;

    //rbegin() is used to reach the right hand side of the binary numbers
    //in order to traverse the numbers correctly
    for (auto i = back.rbegin(); i != back.rend(); i++) 
    {
        correction.push_back(*i);
    }

    return correction;
}

std::vector<bool> onesComp(std::vector<bool> number)
{
    std::vector<bool> ones;


    for(auto i = number.begin(); i != number.end(); i++)
    {
        if(*i)
        {
            ones.push_back(false);
        }
        else
        {
            ones.push_back(true);
        }
    }

    return ones;
}

void printNum(std::vector<bool> num) //function to print the binary number
{
    for (auto i = num.begin(); i != num.end(); i++)
    {
        std::cout << *i; //will traverse through the 1s and 0s
    }
}

bool powerOf2(int num)//will determine if the number is a power of 2
{
    if(num <=0)
    {
        return false;
    }

    do
    {
        if(num%2 !=0)
        {
            return false;
        }

        num = num/2;
    }while(num != 1);

    return true;
}

// int foo(){
//     bigInt<2048> i(5);
//     bigInt<2048> j = (i << 1000) + 23;

//     return j.convertToInteger<int>();
// }


template <int T> //creating a constructor with no parameters
bigInt<T>::bigInt()
{
    if(!powerOf2(T))
    {
        std::cerr << "bigInt will only accept an unsigned integer which is a power of 2" << std::endl;
    }
}


template <int T> //creating a constructor which takes an int parameter
bigInt<T>::bigInt(int input)
{
    if(!powerOf2(T))
    {
        std::cerr << "bigInt will only accept an unsigned integer which is a power of 2" << std::endl;
    }
    else
    {
        std::vector<bool> binaryNumber = Integer_to_Binary(input);

        if(T >= binaryNumber.size())
        {
            int size = binaryNumber.size();
            for(auto i = 0; i < (T - size); i++)
            {
                bigInteger.push_back(false);
            }

            for(auto i = binaryNumber.begin(); i != binaryNumber.end(); i++)
            {
                bigInteger.push_back(*i);
            }
        }
        else
        {
            std::cout << "Size of the store is too small" << std::endl;
        }

    }
}

//function to print bigInt
template <int T>
void print_bigInt(bigInt<T> bigInteger){
    
    for (auto i = bigInteger.bigInteger.begin(); i != bigInteger.bigInteger.end(); ++i){
        std::cout << *i; //this is also printing 1s and 0s
    }
    std::cout << std::endl;
}

template <int T>
bigInt<T> bigInt<T>::operator+(bigInt<T> num) //overloading of +
{
    std::vector<bool> x = bigInt<T>::bigInteger;
    std::vector<bool> y = num.bigInteger;
    std::vector<bool> sum;

    bool cFlag = false;
    //First find the larger of the two numbers
    if(x.size() > y.size())
    {
        //iterate over the smaller number
        auto j = x.rbegin();//function to start from the back 
        for(auto i = y.rbegin(); i != y.rend(); i++, j++)
        {
            //perform binary addition
            if(*i & *j)
            {
                if(cFlag)
                {
                    sum.push_back(true);
                    cFlag = true;
                }
                else
                {
                    sum.push_back(false);
                    cFlag = true;
                }
            }
            else if(*i & (!*j))
            {
                if(cFlag)
                {
                    sum.push_back(false);
                    cFlag = true;
                }
                else
                {
                    sum.push_back(true);
                    cFlag = false;
                }
            }
            else if(!(*i) & *j)
            {
                if(cFlag)
                {
                    sum.push_back(false);
                    cFlag = true;
                }
                else{
                    sum.push_back(true);
                    cFlag = false;
                }
            }
            else if(!(*i) & *j)
            {
                if(cFlag)
                {
                    sum.push_back(true);
                    cFlag = false;
                }
                else
                {
                    sum.push_back(false);
                    cFlag = false;
                }
            }
        }

        while( j != x.rend())
        {
            sum.push_back(*j);
            j++;        }
    }

    //if y is larger than x
    else
    {
         auto j = x.rbegin();//function to start from the back 
        for(auto i = y.rbegin(); i != y.rend(); i++, j++)
        {
            //perform binary addition
            if(*i & *j)
            {
                if(cFlag)
                {
                    sum.push_back(true);
                    cFlag = true;
                }
                else
                {
                    sum.push_back(false);
                    cFlag = true;
                }
            }
            else if(*i & !(*j))
            {
                if(cFlag)
                {
                    sum.push_back(false);
                    cFlag = true;
                }
                else
                {
                    sum.push_back(true);
                    cFlag = false;
                }
            }
            else if(!(*i) & *j)
            {
                if(cFlag)
                {
                    sum.push_back(false);
                    cFlag = true;
                }
                else{
                    sum.push_back(true);
                    cFlag = false;
                }
            }
            else if(!(*i) & !(*j))
            {
                if(cFlag)
                {
                    sum.push_back(true);
                    cFlag = false;
                }
                else
                {
                    sum.push_back(false);
                    cFlag = false;
                }
            }
        }

        while( j != x.rend())
        {
            sum.push_back(*j);
            j++;        
        }
    }

    if(cFlag)
    {
        sum.push_back(true);
    }

    sum = reverseBinary(sum);

    bigInt<T> bigIntegerSum;
    bigIntegerSum.bigInteger = sum;

    return bigIntegerSum;
}

template <int T>
bigInt<T> bigInt<T>::operator-(bigInt<T> num)//overloading of -
{
    std::vector<bool> x = bigInt<T>::bigInteger;
    std::vector<bool> y = num.bigInteger;
    //bigInts must be of the same size
    if(x.size() != y.size())
    {
        if(x.size() > y.size())
        {
            y = reverseBinary(y);
           
            while(x.size() > y.size())
            {
                y.push_back(false);
            }

            y = reverseBinary(y);
        }
        else
        {
            x = reverseBinary(x);

            while(y.size() > x.size())
            {
                x.push_back(false);
            }

            x = reverseBinary(x);
        }
    }

    bigInt<T> y_onescomp;
    y_onescomp.bigInteger = onesComp(y);

    bigInt<T> one(1);

    bigInt<T> y_twoscomp = y_onescomp + one;

    bigInt<T> xNumber;
    xNumber.bigInteger = x;

    bigInt<T> diff = xNumber + y_twoscomp;//getting the difference
    std::vector<bool> t = diff.bigInteger;

    t = reverseBinary(t);
    t.pop_back();
    t = reverseBinary(t);
    diff.bigInteger = t;

    return diff;
}

template <int T>
bigInt<T> bigInt<T>::operator>>(int n)//overloading of >>
{
    std::vector<bool> num = bigInt<T>::bigInteger;

    if(num.size() <= n) //if statement just in case left shifts is larger than length of bigInt
    {
        bigInt<T> error(0);

        return error; //empty bigInt is returned
    }

    for(int i = 0; i < n; i++)
    {
        num.pop_back();
    }

    num = reverseBinary(num);

    //maintaining the original length of bigInt
    for( int i = 0; i < n; i++)
    {
        num.push_back(false);
    }

    num = reverseBinary(num);

    bigInt<T> shifted;
    shifted.bigInteger = num;

    return shifted;
}

template <int T>
bigInt<T> bigInt<T>::operator<<(int n)//overloading of <<
{
    std::vector<bool> num = bigInt<T>::bigInteger;

    //avoiding overload errors
     for( int i = 0; i < n; i++)
    {
        num.push_back(false);
    }

    bigInt<T> shifted;
    shifted.bigInteger = num;

    return shifted;
}

template <int T>
bigInt<T> bigInt<T>::operator*(bigInt<T> num)//overloading of *
{
    std:: vector<bool> x = bigInt::bigInteger;
    std:: vector<bool> y = num.bigInteger;

    bigInt<T> t;
    t.bigInteger = x;

    bigInt<T> product;
    product.bigInteger = x;

    //standard binary multiplication
    int count = 0;
    for(auto i = y.rbegin(); i != y.rend(); i++, count++)
    {
        if(*i){
            t = t << count;
            product = product + t;
        }
    }

    return product;
}

template <int T>
bigInt<T> bigInt<T>::operator/(bigInt<T> num)//overloading of /
{
    std:: vector<bool> x = bigInt::bigInteger;
    std:: vector<bool> y = num.bigInteger;

    bigInt<T> number;
    number.bigInteger = x;

    bigInt<T> div;
    div.bigInteger = y;

    bigInt<T> minus;

    bool negative = false;//used to identify negative number
    int count = 0;

    //will count the amount of times the number is subtracted
    //until a negative number is reached
    do 
    {
        minus = number - div;

        auto i = minus.bigInteger.begin();
        if(*i)
        {
            negative = true;
        }
        else
        {
            count++;
            number.bigInteger = minus.bigInteger;
        }
    }while(!negative);

    bigInt<T> result = count;

    return result;
}

template <int T>
bigInt<T> bigInt<T>::operator%(bigInt<T> num)//overloading of %
{
    std:: vector<bool> x = bigInt::bigInteger;
    std:: vector<bool> y = num.bigInteger;

  
    bigInt<T> number;
    number.bigInteger = x;

    
    bigInt<T> div;
    div.bigInteger = y;

    bigInt<T> minus;

    bool negative = false;
    int count = 0;

    //Same as division, instead remainder is returned
    do
    {
        minus = number - div;

        auto i = minus.bigInteger.begin();
        if(*i)
        {
            negative = true;
        }
        else
        {
            count += 1;
            number.bigInteger = minus.bigInteger;
        }
    }while(!negative);

    return number;
}

template <int T>
template <typename type>
type bigInt<T>::convertToInteger()//function to convert BigInt to int
{
    std::vector<bool> num = bigInt<T>::bigInteger;
    std::vector<bool> net;

    int total;
    int count = 1;
    int bigIntegerSize = num.size();


    if(bigIntegerSize > 32)
    {
        auto p = num.rbegin();
        for(int i = 0; i < 32; i++, p++)
        {
            net.push_back(*p);
        }

        net = reverseBinary(net);
    }

    for(auto i = num.rbegin(); i != num.rend(); i++)
    {
        if(*i)
        {
            total = total + ((int)(pow(2, count)));
        }
        count += 1;
    }

    return total/2;
}

//test case

int test(){

    std::cout << std::endl;
    
    std::cout << "creating bigInt x via bigInt(int num)" << std::endl;
    bigInt<8> x(86);
    print_bigInt(x);
    std::cout << "executing x.convertToInteger " << x.convertToInteger<int>() << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "creating bigInt y via bigInt() = int num" << std::endl;
    bigInt<8> y = 3;
    print_bigInt(y);
    std::cout << "executing y.convertToInteger " << y.convertToInteger<int>() << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "executing x + y and storing it in bigInt z " << std::endl;
    bigInt<8> z = x + y;
    print_bigInt(z);
    std::cout << "executing c.convertToInteger " << z.convertToInteger<int>() << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "executing x - y and storing it in bigInt m" << std::endl;
    bigInt<8> m = x - y;
    print_bigInt(m);
    std::cout << "executing d.convertToInteger " << m.convertToInteger<int>() << std::endl;
    
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "executing y << 2 and storing it as bigInt h" << std::endl;
    bigInt<8> h = y << 2 ;
    print_bigInt(h);
    std::cout << "executing e.convertToInteger " << h.convertToInteger<int>() << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "executing y >> 2 and storing it as bigInt f" << std::endl;
    bigInt<8> f = y >> 2;
    print_bigInt(f);
    std::cout << "executing f.convertToInteger " << f.convertToInteger<int>() << std::endl;

    // std::cout << std::endl;
    // std::cout << std::endl;

    // std::cout << "executing x * y and storing it as bigInt product" << std::endl;
    // bigInt<8> product = x * y;
    // print_bigInt(product);
    // std::cout << "executing prod.convertToInteger " << product.convertToInteger<int>() << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "executing x / y and storing it in bigInt division" << std::endl;
    bigInt<8> division = x/y;
    print_bigInt(division);
    std::cout << "executing div.convertToInteger " << division.convertToInteger<int>() << std::endl;
    
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "executing x % y and storing it in bigInt modulus" << std::endl;
    bigInt<8> modulus = x%y;
    print_bigInt(modulus);
    std::cout << "executing mod.convertToInteger " << modulus.convertToInteger<int>() << std::endl;

    return 0;
}

int foo(){
    bigInt<2048> i(5);
    bigInt<2048> j = (i << 1000) + 23;

    return j.convertToInteger<int>();
}

int main()
{
    std::cout << "The Main Method: " << std::endl;

    test();

    // std::cout << foo() << std::endl;

    return 0;
}
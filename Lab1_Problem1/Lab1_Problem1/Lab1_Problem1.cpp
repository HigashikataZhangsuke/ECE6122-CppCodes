/*
Author:<Yinzhe Zhang>
Class:ECE6122
Last Date Modified:<20/09/2021>

Description:

This cpp file aims at using functions to give prime factors of a given unsigned long number.
The command line for compling this file should be "g++ Lab1_Problem1 -std=c++11 -o Lab1_Problem1".

*/

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <algorithm>
# include <stdio.h> 
# include <math.h> 

bool compareStrings(std::string str1, std::string str2)
/*
This is a simple function used to compare two strings(all formed by numbers).
Input: Two strings.
Output: Bool value
*/
{
    bool flag = false;
    size_t len1 = str1.length();
    size_t len2 = str2.length();
    // Input has more digits than thresh.
    if (len1 > len2)
    {
        flag = true; 
    }
    // Same digits but input is bigger.
    else if (len1==len2)
    {
        flag = (str1 >= str2); 
    }
    else
    {
        flag = false;
    }
    return flag;
}

bool isValid(int argnum, std::string inputArgument)
/*
This is the function used to judge whether current input is a vaild number.
Input: A string.
Output: Bool value
*/
{
    const char* string = inputArgument.data(); 
    //More than one parameter will be filtered.One for filename and one for parameter.
    bool flagOfInputNumber = (argnum > 2)? false:true;
    //Input charactersand minus number will be filtered.
    bool flagOfInputArgument = std::all_of(string, string + strlen(string), [](unsigned char c) { return ::isdigit(c); });
    //Check if we meet a overflow
    bool flagOfInputRange = (compareStrings(string, "18446744073709551615") == true)? false:true;
    return (flagOfInputArgument && flagOfInputNumber && flagOfInputRange);
}

void writeFile(bool ifWrite, bool isPrime, std::string factorList)
/*
This is a function to write output info to the file.
Input: Two bool values and a string.
*/
{
    std::ofstream myfile;
    myfile.open("output1.txt");
    //Under this condition, the input is vaild.
    if (ifWrite == true)
    {
        //And we do find the factors for the input number.
        if (isPrime == true)
        {
            myfile << factorList;
        }
        //Or we need to report no prime factors.
        else
        {
            myfile << "No prime factors";
        }

    }
    //Or we need to report invalid.
    else
    {
        myfile << "Invalid Input";
    }
    myfile.close(); 
}

bool GetPrimeFactors(const unsigned long ulInputNumber, std::string &strOutput)
/*
This is a function to judge whether the input number is a prime number.And then it will get prime factors for composite imput number.
Input: An unsigned long number and a string
Output: Bool values
*/
{
    int n = ulInputNumber;
    // Find if the input has prime factors.
    bool haveprimeflag = (ulInputNumber != 0) and (ulInputNumber != 1); 
    if (haveprimeflag == true)
    {
        //Note we need to get all 2 out first:
        while (n%2 == 0) 
        {
            n = n / 2;
            strOutput.append("2 "); //Get factors and save them by append.
        }
        //Now deal with odd factors: start from 3,and since only odd num will be factor now so i+=2.
        for (int i = 3; i <= sqrt(n); i = i + 2) 
        {
            while (n % i == 0)
            {
                n = n / i;
                std::string I = std::to_string(i);
                strOutput.append(I+" ");  
            }

        }
        //For prime number:
        if (n > 2)
        {
            std::string N = std::to_string(n);
            strOutput.append(N + " ");
        }
    }    
    return haveprimeflag;
}

int main(int argc, char* argv[])
/*
This is the main function, used to call GetPrimeFactors as well as other functions.
Input: A command line parameter.
*/
{
    //Null input here!
    if (argc != 2)
    {
        writeFile(false, false, "0");
    }
    else
    {
        //Input must be valid, and only one input is accepted.
        if (isValid(argc, argv[1]) == true)
        {
            std::string Output; 
            unsigned long inputNum = std::stoul(argv[1], nullptr, 0);
            bool haveprimefactor = GetPrimeFactors(inputNum, Output);
            writeFile(true, haveprimefactor, Output);
        }
        else
        {
            writeFile(false, false, "0");
        }
    }
    return 0;
}



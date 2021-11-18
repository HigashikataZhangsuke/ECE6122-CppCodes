/*
Author:<Yinzhe Zhang>
Class:ECE6122
Last Date Modified:<23/09/2021>

Description:

This cpp file aims at using functions to solve Langton's Ant problem.
The command line for compling this file should be "g++ Lab1_Problem2 -std=c++11 -o Lab1_Problem2"

Refrence of the splitString function: https://www.cnblogs.com/carsonzhu/p/5859552.html
*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <cstring>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <array>
#include <vector>
#include <sstream>

struct pointData
/*
This is a simple struct which will record coordinate and color of a certain grid.
Datatype: A 2D array will record coordinate, and an interger number will record color.
*/
{
	std::array<int, 2> coor ;
    //Use 0 to represent White and 1 for black.This will let us do a simple add operation to get the final result.
	int clr; 
};

std::string calculateNextCoordinate(const std::array<int,2> &coor,const int &direction)
/*
This is the function used to calculate Next step's Coordinate of the ant.
Input: An array and an interger number
Ouput: A string.
*/
{
    std::string str;
    std::array<int, 2> currcoorarray;
    // different direction will result different new coordinate: here 0 for up; 1 for right; 2 for down and 3 for left.
    switch (direction) 
    {
        case(0):
            currcoorarray[0] = coor[0];
            currcoorarray[1] = coor[1] + 1;
            break;
        case(1):
            currcoorarray[0] = coor[0] + 1;
            currcoorarray[1] = coor[1];
            break;
        case(2):
            currcoorarray[0] = coor[0];
            currcoorarray[1] = coor[1] - 1;
            break;
        case(3):
            currcoorarray[0] = coor[0] - 1;
            currcoorarray[1] = coor[1];
            break;
    }
    std::string coor1 = std::to_string(currcoorarray[0]);
    std::string coor2 = std::to_string(currcoorarray[1]);
    // concat the strings
    str = coor1+" "+coor2;
    return str;
}

void splitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
/*
This is the function used to split strings. The split flag is blank space. For source of this function , please refer to the head comments of this Cpp file.
Input: Two strings and a vector
*/
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2 - pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}

int getxCoor(std::string str)
/*
This is a simple function used to get x coordinate of a point from its string label.
Input: A string
Output: An interger number
*/
{
    int xCoor;
    std::vector<std::string> ves;
    std::stringstream ss;
    splitString(str, ves," ");
    ss << ves[0];
    ss >> xCoor;
    return xCoor;
}

int getyCoor(std::string str)
/*
This is a simple function used to get y coordinate of a point from its string label.
Input: A string
Output: An interger number
*/
{
    int yCoor;
    std::vector<std::string> ves;
    std::stringstream ss;
    splitString(str, ves, " ");
    ss << ves[1];
    ss >> yCoor;
    return yCoor;
}

void step(std::unordered_map<std::string, pointData> &gridmap, int &direction, std::string &currCorr)
/*
This is the function used to let the ant move one step, and get the next node's coordinate that it will go.
Input: An unordered_map, an interger number and a string
*/
{
    //We didn't find current point in the map, so we need to insert it.
    if (gridmap.count(currCorr) == 0)
    {
        //Basic info of this point
        pointData Curr;
        int x = getxCoor(currCorr);
        int y = getyCoor(currCorr);
        Curr.coor = {x,y};    
        //Change color,note it must be black since the ant didn't visit this node before.
        Curr.clr = 1;
        gridmap.insert({ currCorr, Curr });
        //Update coordinate.
        currCorr = calculateNextCoordinate(Curr.coor, direction);
        //Update direction.
        //If the next coordinate doesn't exist
        if (gridmap.count(currCorr) == 0)
        {
            //There is no need to judge since it must be a new node with white color.
            direction = (direction + 1) % 4;
        }
        else 
        {
            if (gridmap.at(currCorr).clr == 1) 
            {
                //Black point
                direction = (direction + 3) % 4;
            }
            else
            {
                //White point
                direction = (direction + 1) % 4; 
            }
        }
    }
    //We have already visited this node, now just need to change its color.
    else
    {
        //Get this node's info
        pointData Curr;
        Curr = gridmap.at(currCorr);
        gridmap.at(currCorr).clr = (gridmap.at(currCorr).clr + 1) % 2;
        //Update coordinate
        currCorr = calculateNextCoordinate(Curr.coor, direction);
        //Update direction
        //If the next coordinate doesn't exist 
        if (gridmap.count(currCorr) == 0)
        {
            //There is no need to judge since it must be a new node with white color.
            direction = (direction + 1) % 4; 
        }
        else 
        {
            if (gridmap.at(currCorr).clr == 1) 
            {
                //Black point
                direction = (direction + 3) % 4; 
            }
            else
            {
                //White point
                direction = (direction + 1) % 4; 
            }
        }
    }
}

int GetAmount(unsigned long steps)
/*
This is the function used to get total amout of the black blocks
Input: An unsigned long number
Output: An int number
*/
{
    //initialize the condition, we assume that the ant start at (0,0) and its direction is 0 ¡ª¡ª up.
    unsigned long count = 0;
    int Num = 0;
    std::string Coor = "0 0";
    //You could change this to get another test case.
    int direction = 0;
    std::unordered_map<std::string, pointData> Antmap;
    while (count < steps)
    {
        count++;
        //Move a step.
        step( Antmap, direction, Coor);
    }
    for (auto& x : Antmap)
    {
        //Adding the color variables up.   
        Num = Num + x.second.clr; 
    }
    return Num;
}

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
    else if (len1 == len2)
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
    bool flagOfInputNumber = (argnum > 2) ? false : true;
    //Input charactersand minus number will be filtered.
    bool flagOfInputArgument = std::all_of(string, string + strlen(string), [](unsigned char c) { return ::isdigit(c); });
    //Check if we meet a overflow
    bool flagOfInputRange = (compareStrings(string, "18446744073709551615") == true) ? false : true;
    return (flagOfInputArgument && flagOfInputNumber && flagOfInputRange);
}

void writeFile(bool ifWrite, std::string blacknum)
/*
This is a function to write output info to the file we want.
Input: One bool values and a string.
*/
{
    std::ofstream myfile;
    myfile.open("output2.txt");
    if (ifWrite == true)
    {
        myfile << blacknum;

    }
    else
    {
        myfile << "Invalid Input";
    }
    myfile.close();
}

int main(int argc, char* argv[])
/*
This is the main function, used to call functions defined above to solve problem.
Input: A command line parameter.
*/
{
    //Null input here!
    if (argc != 2)
    {
        writeFile(false, "0");
    }
    else
    {
        //Input must be valid, and only one input is accepted.
        if (isValid(argc, argv[1]) == true)
        {
            unsigned long inputNum = (argv[1], nullptr, 0);
            int blacknum = GetAmount(inputNum);
            writeFile(true, std::to_string(blacknum));
        }
        else
        {
            writeFile(false, "0");
        }
    }
    return 0;
}

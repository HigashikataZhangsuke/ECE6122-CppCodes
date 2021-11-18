/*
Author:<Yinzhe Zhang>
Class:ECE6122
Last Date Modified:<24/09/2021>

Description:

This cpp file aims at using functions to solve the White Cell reflection problem.
The command line for compling this file should be "g++ Lab1_Problem3 -std=c++11 -o Lab1_Problem3"

Refrence of the isNumber function: https://stackoverflow.com/questions/29169153/how-do-i-verify-a-string-is-valid-double-even-if-it-has-a-point-in-it
*/

#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <math.h>  
#include <fstream>
#include <array>
#include <cstdlib>

bool judgeValidity(const std::array<double, 2> &point)
/*
This is the function used to judge if the new intersect point we get is vaild.
Input: An array
Output: bool value
*/
{
    //The point need to be in the triangle, and for (0,0) case, it is also a vaild point since not a corner this time.
    return ((point[0] < 10) && (point[0] > -10) && (point[1] <= sqrt(300)) && (point[1] >= 0) && (point[0] / point[1] <= 1 / sqrt(3)) && (point[0] / point[1] >= -1 / sqrt(3)) || ((point[0] == 0) && (point[1] == 0)) ? true : false);
}

void calculateIntersectpoint(std::array<double, 2> &point, const std::array<double, 4>  &line, int &whichEdge)
/*
This is the function used to calculate intersect points. At the same time it also updates which edge we used as the reflect plane.
Input: Two array and an interger number
*/
{
    //Each time when we reflect, there will be two potential intersect points
    std::array<double, 2> ppoint1;
    std::array<double, 2> ppoint2;
    //Set two judge so that when facing the corner case could output error. If boost needed could be eliminated.
    bool judge1,judge2;
    switch (whichEdge)
    {
        //Last time we reflect at AB edge.
        case(0):
            //Intersect point can only located on AC/BC edge
            //AC edge
            ppoint1[0] = line[0] - (line[0] * sqrt(3) + line[1]) / (sqrt(3) * line[2] + line[3]) * line[2];
            ppoint1[1] = -sqrt(3) * ppoint1[0];
            //BC edge
            ppoint2[0] = line[0] + (line[1] - line[0] * sqrt(3)) / (sqrt(3) * line[2] - line[3]) * line[2];
            ppoint2[1] = sqrt(3) * ppoint2[0];
            //Judge
            judge1 = judgeValidity(ppoint1);
            judge2 = judgeValidity(ppoint2);
            if (judge1 == true)
            {
                point = ppoint1;
                whichEdge = 1;
            }
            else if (judge2 == true)
            {
                point = ppoint2;
                whichEdge = 2;
            }
            else
            {
                std::cout << "error" << std::endl;
            }
            break;
        //Last time we reflect at AC edge.
        case(1):
            //Intersect point can only located on AB/BC edge
            //AB edge
            ppoint1[0] = line[0] + line[2] * (sqrt(300) - line[1]) / line[3];
            ppoint1[1] = sqrt(300);
            //BC edge
            ppoint2[0] = line[0] + (line[1] - line[0] * sqrt(3)) / (sqrt(3) * line[2] - line[3]) * line[2];
            ppoint2[1] = sqrt(3) * ppoint2[0];
            //Judge
            judge1 = judgeValidity(ppoint1);
            judge2 = judgeValidity(ppoint2);
            if (judge1 == true)
            {
                point = ppoint1;
                whichEdge = 0;
            }
            else if (judge2 == true)
            {
                point = ppoint2;
                whichEdge = 2;
            }
            else
            {
                std::cout << "error" << std::endl;
            }
            break;
        //Last time we reflect at BC edge.
        case(2):
            //Intersect point can only located on AB/AC edge
            //AB edge
            ppoint1[0] = line[0] + line[2] * (sqrt(300) - line[1]) / line[3];
            ppoint1[1] = sqrt(300);
            //AC edge
            ppoint2[0] = line[0] - (line[0] * sqrt(3) + line[1]) / (sqrt(3) * line[2] + line[3]) * line[2];
            ppoint2[1] = -sqrt(3) * ppoint2[0];
            //Judge
            judge1 = judgeValidity(ppoint1);
            judge2 = judgeValidity(ppoint2);
            if (judge1 == true)
            {
                point = ppoint1;
                whichEdge = 0;
            }
            else if (judge2 == true)
            {
                point = ppoint2;
                whichEdge = 1;
            }
            else
            {
                std::cout << "error" << std::endl;
            }
            break;
    }
}

void calculateLineEqualtion(std::array<double, 4> &line,const std::array<double, 2> &normline, const std::array<double, 2> &point)
/*
This is the function for calculating reflect line equations.At the same time it also updates the start point of the line.
Input: Three arrays.
*/
{
    //Update start coordinate with intersect point's coordinates.
    line[0] = point[0];
    line[1] = point[1];
    //Update direction vector.
    double curr;
    curr = line[2];
    line[2] = line[2] - 2.0 * (line[2] * normline[0] + line[3] * normline[1]) * normline[0];
    line[3] = line[3] - 2.0 * (curr * normline[0] + line[3] * normline[1]) * normline[1];
}

bool isNumber(const std::string& s)
/*
This is the function used for checking if the input is a double number. For refrence please check the head comments.
Input: A string.
*/
{
    char* end = nullptr;
    double val = strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0' && val != HUGE_VAL;
}

bool isValid(int argNum, std::string inputArgument)
/*
This is the function used to judge whether current input is vaild.
Input: An int number and a string.
Output: Bool value
*/
{
    const char* string = inputArgument.data(); 
    //More than one parameter will be filtered.
    bool flagOfInputNumber = ((argNum != 2)?false:true);
    //Not a double number will be filtered.
    bool flagOfInputArgument = ((isNumber(string) == false)? false:true); 
    //Out of range will be filtered.
    bool flagOfInputRange = ((std::stod(string) >= 10.0 || std::stod(string) <= -10.0)?false:true);
    return (flagOfInputArgument && flagOfInputNumber && flagOfInputRange);
}

void writeFile(bool ifValid, int reflectNum)
/*
This is a function to write output info to the file we want.
Input: One bool values and a string.
*/
{
    std::ofstream myfile;
    myfile.open("output3.txt"); //Open the file we will write.
    if (ifValid == true) //Under this condition, the input is vaild.
    {
        myfile << reflectNum;

    }
    else //Or we need to report something wrong happened.
    {
        myfile << "Invalid Input";
    }
    myfile.close(); //Close the file.
}

int main(int argc, char* argv[])
/*
This is the main function, used to call functions defined above to solve problem.
Input: A command line parameter.
*/
{
    //Null input!
    if (argc != 2)
    {
        writeFile(false, 0);
    }
    else
    {
        if (isValid(argc, argv[1]) == true) 
        {
            //Initialize the problem
            double initx = std::stod(argv[1]);
            std::array<double, 2> intersectPoint = { initx,sqrt(300)};
            //Use a d4 vector with its elements are start point coordinates + direction vector to a represent line.
            std::array<double, 4> line = {0.0,0.0, initx ,sqrt(300) } ; 
            //The normal vectors of edges. Standardized already.
            std::array<double, 2> lineABNorm = { 0.0,-1.0 };
            std::array<double, 2> lineACNorm = { sqrt(3)/2,0.5 };
            std::array<double, 2> lineBCNorm = { -sqrt(3)/2,0.5 };
            int reflectcount = 0;
            bool out = false;
            //At which edge reflected? Start with AB edge.
            int whichEdge = 0;
            while (out == false)
            {
                //Different reflections.
                switch (whichEdge)
                {
                    //Reflect at AB
                    case(0):
                        calculateLineEqualtion(line,lineABNorm, intersectPoint);
                        break;
                    //reflect at AC
                    case(1):
                        calculateLineEqualtion(line, lineACNorm, intersectPoint);
                        break;
                    //reflect at BC
                    case(2):
                        calculateLineEqualtion(line, lineBCNorm, intersectPoint);
                        break;
                }
                //Then calculate new intersect point.
                calculateIntersectpoint(intersectPoint, line, whichEdge);
                out = ((intersectPoint[1] <= 0.01) && (intersectPoint[1] >= 0) ? true : false);
                reflectcount += 1;
            }
            writeFile(true, reflectcount);
        }
        else
        {
            writeFile(false, 0);
        }
    }
    return 0;
}


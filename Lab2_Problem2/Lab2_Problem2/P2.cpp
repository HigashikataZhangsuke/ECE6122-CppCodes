/*
Author:<Yinzhe Zhang>
Class:ECE6122
Last Date Modified:<13/10/2021>

Description:

This cpp file aims at using OpenMP to calculate integral of a given function.

*/

#include <iostream>
#include <omp.h>
#include <cmath>
#include <iomanip>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cstring>
#include <fstream>

int main(int argc, char* argv[])
/*
This is the main function used to solve integral with openMP
Input: A positive int number N
Output: The integral result
*/
{
	//Store result here
	double result = 0.0;
	//Left point of integration interval
	double leftpoint = 0.0;
	//Right point of integration interval
	double rightpoint = log(2)/7;
	//Read input N
	int N = std::stoul(argv[1], nullptr, 0);
	//Split the interval to N parts.Exactly one step.
	double dx = (rightpoint - leftpoint) / N;
	//Call OMP here
    #pragma omp parallel for reduction(+: result)
	for (std::size_t i = 0; i < N; i++)
	{
		//OMP parallel for
		result += 14 * (exp(7 * (leftpoint + (i - 0.5) * dx))) * dx;
	}
	//Write result in the file
	std::ofstream myfile;
	myfile.open("Lab2Prob2.txt");
	myfile << std::setiosflags(std::ios::fixed);
	myfile << std::setprecision(6);
	myfile << result;
	myfile.close();
	return 0;
}

/*
Author:<Yinzhe Zhang>
Class:ECE6122
Last Date Modified:<13/10/2021>

Description:

This cpp file aims at using threads to estimate the average steps an ant need to finish carrying
seed from bottom to top in a given 5*5 grid.

*/
#include <iostream>
#include <thread>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <iomanip>
#include <atomic>
#include <random>

void getEachThreadSteps(std::atomic<unsigned long long> &totalsteps,const double &N)
/*
This is the function called by each thread, it can help the thread finish simulation task.
Input: An atomic type counter and a double type simulation rounds controller N. 
*/
{
    //Simulation rounds conuter
    int counter(0) ;
    //Random engine initialization
    std::random_device rd;
    std::default_random_engine Engine(rd());
    std::uniform_int_distribution<int> U(0, 3);
    //Two 1D differential tables for us to calculate the ant's coordinates in next turn
    //Row table
    int diffx[4] = { -1, 0, 1, 0 };
    //Column table
    int diffy[4] = { 0, 1, 0, -1 };
    //The counter records the total steps this thread runs
    unsigned long long localstep(0);
    while (counter <N)
    {
        //We still need to do simulations
        //Initialize the start coordinate of ant
        int x(2);
        int y(2);
        //Initialize the next turn's coordinate of ant
        int nextx(0);
        int nexty(0);
        //A flag of whether the ant is carrying a seed
        bool carryseed = false;
        //The counter records total number of seed taht the ant has already carried
        int totalcarry(0);
        //Initialize the direction
        int direction = U(Engine);
        //initialize the grids, 1 means bottom row has seeds
        int grid[5][5] = { {0,0,0,0,0}, {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0} ,{1,1,1,1,1} };
        //Inner while loop will finish one simulation
        while (totalcarry<5)//Some seeds still not in the top row
        {
            //First, we update the ant's coordinate 
            x += diffx[direction];
            y += diffy[direction];
            //Second,we check whether carry on/put down operation happen
            //Currently ant isn't carrying any seed and it is standing on the bottom girds which exactly contain seeds
            if (!carryseed && x == 4 && grid[x][y] == 1)
            {
                //We pick up seed!
                grid[x][y] = 0;
                carryseed = true;
            }
            //Or the ant needs to put down seed, when it stepping on the top empty grids
            else if (carryseed && x == 0 && grid[x][y] == 0)
            {
                //We put down seed!
                grid[x][y] = 1;
                carryseed = false;
                //One seed moved to top successfully
                totalcarry += 1;
            }
            //Third, we update direction for next turn:
            //Initialize
            direction = U(Engine);
            nextx = x + diffx[direction];
            nexty = y + diffy[direction];
            //If the ant will move out the grids
            while (nextx<0 || nextx >4|| nexty<0 || nexty>4)
            {
                //Resample till we get a valid direction
                direction = U(Engine);
                nextx = x + diffx[direction];
                nexty = y + diffy[direction];
            }
            localstep += 1;  
        }
        counter += 1;
    }
    //Update steps needed
    totalsteps += localstep;
}

int main()
{
    //We get the max thread num
    int numofthread = std::thread::hardware_concurrency();//should be 24 here
    //Total round number each thread needs to run, use floor to make it an integer
    double roundsofeachthread = floor(10000000 / numofthread) + 1;
    //Initialize Total steps conuter
    std::atomic<unsigned long long> totalsteps{ 0 };
    //Initialize vector of threads
    std::vector<std::thread> threads;
    //Use numofthread-1 since main() will occupy one thread
    for (int i = 0; i < numofthread -1; i++)
    {
        threads.push_back(std::thread(getEachThreadSteps, std::ref(totalsteps), std::ref(roundsofeachthread)));
    }
    //main()'s thread
    getEachThreadSteps(totalsteps, roundsofeachthread);
    //Join
    for (auto& th : threads) th.join();
    //Calculate the average steps needed
    double avgstep = totalsteps / (numofthread * roundsofeachthread);
    //Write result in the file
    std::ofstream myfile;
    myfile.open("ProblemOne.txt");
    myfile << "Number of threads created: ";
    myfile << numofthread;
    myfile << "\n\nExpected number of steps: ";
    myfile << std::setiosflags(std::ios::fixed);
    myfile << std::setprecision(6);
    myfile << avgstep;
    myfile << "\n\nTotal number of runs needed for solution convergence: 10000008";
    myfile.close();
    return 0;
}


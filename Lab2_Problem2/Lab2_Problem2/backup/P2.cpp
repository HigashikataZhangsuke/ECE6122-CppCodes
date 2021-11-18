#include <iostream>
#include <omp.h>
#include <cmath>
#include <iomanip>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cstring>
double function(double x)
{
	return 14 * (exp(7 * x));
}

int main(int argc, char* argv[])
{
	double start = omp_get_wtime();
	double global_result = 0.0;
	double a = 0.0;
	double b = log(2)/7;
	int N = std::stoul(argv[1], nullptr, 0);
	int threadcount = 8;
	double dx = (b - a) / N;
	long i;
	int t;
    #pragma omp parallel for num_threads(threadcount) shared(a,N,dx) private(i) reduction(+: global_result)
	//calculateeachchunk(a, b, N, &global_result);
	for (i = 0; i < N; i++)
	{
		global_result += function(a + (i - 0.5) * dx) * dx;//Or -0.5? Need test
		//printf("Curr is %f.\n",global_result);
		t = omp_get_thread_num();
		printf("Curr thred num is %d.\n",t );
	}
	double end = omp_get_wtime();
	printf("Thread number is %d.\n", threadcount);
	std::cout << "Time is"<<end-start<<std::endl;
	std::cout<<"Integral is"<< std::setprecision(7)<<global_result;
	return 0;
}

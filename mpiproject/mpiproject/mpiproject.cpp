#include "mpi.h"
#include <math.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

		int done = 0, n, ID, numOfProcesses, i;
		double PI25DT = 3.141592653589793238462643;
		double myPI, PI, h, sum, x;
		
		MPI_Init(&argc, &argv);// initializing the MPI
		MPI_Comm_size(MPI_COMM_WORLD, &numOfProcesses);
		MPI_Comm_rank(MPI_COMM_WORLD, &ID);

		while (!done)
		{
			if (ID == 0) 
			{
				printf("Enter the number of intervals: (press 0 for quit) ");
				scanf_s("%d", &n);
			}
			MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
			if (n == 0) 
				break;

			h = 1.0 / (double)n;
			sum = 0.0;

			for (i = ID + 1; i <= n; i += numOfProcesses)
			{
				x = h * ((double)i - 0.5);
				sum += 4.0 / (1.0 + x * x);
			}
			myPI = h * sum;

			MPI_Reduce(&myPI, &PI, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);

			if (ID == 0)
				printf("pi is approximately %.16f, Error is %.16f\n",PI, fabs(PI - PI25DT));
		}

		MPI_Finalize();
		return 0;

}

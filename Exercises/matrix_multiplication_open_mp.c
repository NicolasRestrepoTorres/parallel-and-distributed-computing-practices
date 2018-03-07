#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// int **multiplication(int first[m][n], int second[p][q]){
// 		int multiply[10][10];
// 		for (c = 0; c < m; c++) {
// 			for (d = 0; d < q; d++) {
// 				for (k = 0; k < p; k++) {
// 					sum = sum + first[c][k]*second[k][d];
// 				}
// 				multiply[c][d] = sum;
//
// 				sum = 0;
// 				}
// 		}
// 	return multiply;
// }



int main(int argc, char* argv[]){
srand(time(NULL));
int m, n, p, q, c, d, k, r, sum = 0, tid, nthreads, chunk = 10, iterations;

	m = n = p = q = atoi(argv[1]);
		int first[m][n], second[p][q], multiply[m][n];
	//printf("Hehe: %d\n", atoi(argv[1]));
	nthreads = atoi(argv[2]);

	while(1){
		//printf("Enter number of rows and columns of first matrix\n");
		//scanf("%d%d", &m, &n);
		//m = 11; n = 11;
		if( m>1024 || n>1024 ){
			printf("Invalid dimensions. The number of rows and colums must not exceed 1024\n");
		} else {
			break;
		}
	}
	//printf("Enter elements of first matrix\n");
	//printf("%d %d %d %d\n",m,n,p,q);
	for (c = 0; c < m; c++)
		for (d = 0; d < n; d++){
			//scanf("%d", &first[c][d]);
			r = rand() % 100;
			//printf("Random: %d\n", r);
			first[c][d] = r;
		}

	m = n = p = q = atoi(argv[1]);
	//printf("%d %d %d %d\n",m,n,p,q);

	while(1){
	//	printf("Enter number of rows and columns of second matrix\n");
		//scanf("%d%d", &p, &q);
		//p = 11; q = 11;
		if( p>1024 || q>1024 ){
			printf("Invalid dimensions. The number of rows and colums must not exceed 1024\n");
		} else {
			break;
		}
	}
		printf("Size: %d\n",m);
	if (n != p)
		printf("The matrices can't be multiplied with each other.\n");

	else {
		//printf("Enter elements of second matrix\n");

		for (c = 0; c < p; c++)
			for (d = 0; d < q; d++){
				//scanf("%d", &second[c][d]);
				r = rand() % 100;
				second[c][d] = r;
			}

				if(m < 5 && q < 5 && p < 5 ) nthreads = 1;

				#pragma omp parallel num_threads(nthreads)
				{
							tid = omp_get_thread_num();
							nthreads = omp_get_num_threads();
							chunk = p / nthreads;
							if (tid == 0)
								{
								//nthreads = omp_get_num_threads();
								p / nthreads;
								printf("Starting matrix multiple example with %d threads\n",nthreads);
								printf("Initializing matrices...\n");
								}
				for (c = 0; c < m; c++) {
					for (d = 0; d < q; d++) {
						for (k = tid*chunk; k < (tid*chunk)+chunk; k++) {
							sum = sum + first[c][k]*second[k][d];
						}
						#pragma omp critical
    					multiply[c][d] = sum;


						sum = 0;
						}
				}
			}


		// #pragma omp parallel
		// 	{
		//
		// 		tid = omp_get_thread_num();
		// 		if (tid == 0)
		// 			{
		// 			nthreads = omp_get_num_threads();
		// 			printf("Starting matrix multiple example with %d threads\n",nthreads);
		// 			printf("Initializing matrices...\n");
		// 			}

					//**multiply = multiplication(first, second);
					// #pragma omp for schedule (static, chunk)
				  // for (i=0; i<NRA; i++)
				  //   for (j=0; j<NCA; j++)
				  //     a[i][j]= i+j;
				  // #pragma omp for schedule (static, chunk)
				  // for (i=0; i<NCA; i++)
				  //   for (j=0; j<NCB; j++)
				  //     b[i][j]= i*j;
				  // #pragma omp for schedule (static, chunk)
				  // for (i=0; i<NRA; i++)
				  //   for (j=0; j<NCB; j++)
				  //     c[i][j]= 0;


			// #pragma omp for schedule (static, chunk)
			// for (i=0; i<m; i++)
			// 	{
			// 	printf("Thread = %d did row = %d\n",tid,i);
			// 	for(j=0; j < q; j++)
			// 		for (k=0; k < p; k++)
			// 			multiply[i][j];
			// 			// += a[i][k] * b[k][j];
			// 	}
	//	} //End of parallel region

		printf("Product of the matrices:\n");

		for (c = 0; c < m; c++) {
			for (d = 0; d < q; d++)
			printf("%d\t", multiply[c][d]);

			printf("\n");
		}
	}

	return 0;
}

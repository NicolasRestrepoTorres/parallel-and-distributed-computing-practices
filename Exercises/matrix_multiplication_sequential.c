#include <stdio.h>

int main(){

	int m, n, p, q, c, d, k, sum = 0;
	int first[10][10], second[10][10], multiply[10][10];

	while(1){
		printf("Enter number of rows and columns of first matrix\n");
		scanf("%d%d", &m, &n);
		if( m>1024 || n>1024 ){
			printf("Invalid dimensions. The number of rows and colums must not exceed 1024\n");
		} else {
			break;
		}
	}
	printf("Enter elements of first matrix\n");

	for (c = 0; c < m; c++)
		for (d = 0; d < n; d++)
			scanf("%d", &first[c][d]);

	while(1){
		printf("Enter number of rows and columns of second matrix\n");
		scanf("%d%d", &p, &q);
		if( p>1024 || q>1024 ){
			printf("Invalid dimensions. The number of rows and colums must not exceed 1024\n");
		} else {
			break;
		}
	}
	if (n != p)
		printf("The matrices can't be multiplied with each other.\n");

	else {
		printf("Enter elements of second matrix\n");

		for (c = 0; c < p; c++)
			for (d = 0; d < q; d++)
				scanf("%d", &second[c][d]);

		for (c = 0; c < m; c++) {
			for (d = 0; d < q; d++) {
				for (k = 0; k < p; k++) {
					sum = sum + first[c][k]*second[k][d];
				}

				multiply[c][d] = sum;
				sum = 0;
		  	}
		}

		printf("Product of the matrices:\n");

		for (c = 0; c < m; c++) {
		  for (d = 0; d < q; d++)
			printf("%d\t", multiply[c][d]);

		  printf("\n");
		}
	}

	return 0;
}

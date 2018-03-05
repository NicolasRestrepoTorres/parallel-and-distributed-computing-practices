#include <stdio.h>

int rows, cols;

void Blur(double image[rows][cols]) {
	
	//double convMatrix[3][3] = {{0.9,0.9,0.9},
	//						   	 {0.9,0.9,0.9},
	//						   	 {0.9,0.9,0.9}};
	
	double convMatrix[3][3] = {{1,0,1},
							   {0,1,0},
							   {1,0,1}};
	
	int r,c,i,j;
	double sum, res[rows-2][cols-2];

	for (r=0; r<rows-1; r++){
		for(c=0; c<cols-1; c++){
			sum = 0; 
			for(i=0; i<3; i++){
				for(j=0; j<3; j++){ 
					sum += image[r+i][c+j]*convMatrix[i][j];
				}
			res[r][c] = sum;
			}
		}	
	}

	for(i=0; i<3; i++){		
		for(j=0; j<3; j++){ 
			printf("%f ",res[i][j]);
		}
		printf("\n");
	}


}


void main(){
	double image[5][5] = {{1,1,1,0,0},
						  {0,1,1,1,0},
						  {0,0,1,1,1},
						  {0,0,1,1,0},
						  {0,1,1,0,0}}; 
	rows = 5;
	cols = 5;
	Blur(image);

}

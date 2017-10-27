
/* COMP10002 Assignment 1: Skyline point problem

   Name: Danish Bassi 	Student ID: 867811	 Last updated: 23/04/17
   
   This C code is a solution to the skyline point problem, where data for
   different points is input and calculates which points are not dominated 
   by the others. These are the 'skyline points' and they are output in order
   of most dominations (over other points) in a descending order. 
*/





/* Import headers/libraries */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Stage header */
#define DIV "==========" 
/* Stage numbers */
#define STAGE_NUM_ONE 1 
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_NUM_FIVE 5

/* Reading point succeeded */
#define READ_OK 1 
/* Reading point failed */
#define READ_FAILED 0 

/* Maximum number of points */
#define MAX_NUM_POINTS 99 
/* Maximum number of dimensions */
#define MAX_NUM_DIMENSIONS 10 

typedef double point_t[MAX_NUM_DIMENSIONS];




/* Function prototypes */
void stage_one(point_t one_point, int *num_points, int num_dimensions);
void stage_two(point_t points[], int *num_points, int num_dimensions, 
	double *coordinate_sums);
void stage_three(double *coordinate_sums, int num_points);
void stage_four(point_t points[], int num_points, int num_dimensions);
void stage_five(point_t points[], int num_points, int num_dimensions);

void print_stage_heading(int stage);

int read_one_point(point_t one_point, int num_dimensions);
void print_one_point(point_t one_point, int num_dimensions, int point);

void find_dominations(int domination[][3], point_t one_point, point_t points[], 
	int num_points, int num_dimensions);

void row_swap(int a[][3], int i, int j);




/* Main program binds it all together */
int
main(int argc, char *argv[]) {

	/* Storage for the input data */
	int num_dimensions;
	point_t points[MAX_NUM_POINTS];
	
	/* Storage for statistics */
	int num_points = 0; /* Number of points read in */
	double coordinate_sums[MAX_NUM_POINTS]; /* Coordinate sums of the points */
	
	/* Read the first line - number of dimensions */
	scanf("%d", &num_dimensions);
	
	/* Stage 1 */
	stage_one(points[0], &num_points, num_dimensions);

	/* Stage 2 */
	stage_two(points, &num_points, num_dimensions, 
		coordinate_sums);
	
	/* Stage 3 */
	stage_three(coordinate_sums, num_points);

	/* Stage 4 */
	stage_four(points, num_points, num_dimensions);

	/* Stage 5 */
	stage_five(points, num_points, num_dimensions);
	
	/* And done! */
	return 0;
}





/* Print out a stage heading, with a prior blank line once the stage number
   goes plus one 
*/
void 
print_stage_heading(int stage){
	/* Main job is to print the stage number passed as an argument */
	if (stage > STAGE_NUM_ONE){
		/* separate the stages */
		printf("\n");
	}
	printf("Stage %d\n%s\n", stage, DIV);
}





/* Read the coordinates of one point, store them in one_point */
int 
read_one_point(point_t one_point, int num_dimensions){
	int i = 0;
	
	/* Keep reading until num_dimensions numbers have been obtained */
	while (i < num_dimensions){
		if (scanf("%lf", one_point+i) != 1){
			return READ_FAILED;
		}
		i++;
	}
	
	return READ_OK;
}





/* Print the coordinates of each point with formatting */
void 
print_one_point(point_t one_point, int num_dimensions, int point){
	int i;
	printf("Point %02d: <", point);
	/* Check if last element of the array is being printed for proper format */
	for (i = 0; i < num_dimensions; i++){
		if (i == num_dimensions-1){
			printf("%.2lf>\n", one_point[i]);
		}
		else{
			printf("%.2lf, ", one_point[i]);
		}
	}
}





/* Find which points are dominated, along with how many of the points they
   dominate and then store data in the domination array
   
   First column in domination represents if the point is dominated by any
   other point or not (1 = dominated, 0 = not dominated). Second column
   represents how many other points a point dominates and the third column
   represents the point's number in the list
*/
void find_dominations(int domination[][3], point_t one_point, point_t points[], 
	int num_points, int num_dimensions){

	int i, j, k;
	
	/* Set all values in first and second column to 0 and the third column 
	   to the point number  
	*/
	for (i = 0; i < num_points; i++){
		for (j = 0; j < 2; j++){
			domination[i][j] = 0;
			domination[i][2] = i+1;
		}
	}
	
	i = 0;
	while(i < num_points){
		/* Assign a new point to one_point array to check through each 
		   iteration */
		j =0;
		while(j < num_dimensions){
			one_point[j] = points[i][j];
			j++;
		}
		
		for (k =0; k<num_points; k++){
			/* Check if current point is dominated by any other point */
			int count = 0;
			for (j = 0; j < num_dimensions; j++){
				if (k != i && one_point[j] <= points[k][j]){
					count++;	
				}
			}	
			/* If dominated, first column of domination equals to 1 and second
			   column (of the point it was compared with) increments to count 
			   how many times it dominates
			*/
			if (count == num_dimensions){
				domination[i][0] = 1;
				domination[k][1]++;
			}
		}		
		i++;
	}
}





/* Swaps two rows */
void row_swap(int dominations[][3], int row1, int row2){
	int tmp[3] = {}, k, num_cols = 3;
	
	for(k = 0; k < num_cols; k++){
		tmp[k] = dominations[row1][k];
	}
	
	for(k = 0; k < num_cols; k++){
		dominations[row1][k] = dominations[row2][k];
	}
	
	for(k = 0; k < num_cols; k++){
		dominations[row2][k] = tmp[k];
	}
}






/* Stage 1 - read in and print one point */
void 
stage_one(point_t one_point, int *num_points, int num_dimensions){
	/* Print stage heading for stage 1 */
	print_stage_heading(STAGE_NUM_ONE);                              
		
	read_one_point(one_point, num_dimensions);
	++*num_points;
	
	int point_num = 1;	
	print_one_point(one_point, num_dimensions, point_num);
}






/* Stage 2 - read in the rest of the points and visualise them */
void 
stage_two(point_t points[], int *num_points, int num_dimensions, 
	double *coordinate_sums){
	/* Print stage heading for stage 2 */
	print_stage_heading(STAGE_NUM_TWO);

	int i = 1, j;		

	/* Read each point into the points array */
	while (read_one_point(points[i], num_dimensions)){		
		++*num_points;
		i++;
	}
	
	for (i = 0; i < *num_points; i++){
		/* Calculate the sum of dimensions for the current point */
		for(j = 0; j < num_dimensions; j++){
			*(coordinate_sums+i) += points[i][j];
		}
		
		/* Print sum divided by 100 */
		printf("Point %02d, sum of coordinates (/100): %5.2lf |", i+1,
				*(coordinate_sums+i)/100);
		
		/* Ceiling function rounds up sum divided by 100 */
		for(j = 1; j <= ceil(*(coordinate_sums+i)/100); j++){
				/* Checks if every 10th character is being printed. If so, print 
				   '+', otherwise '-' 
				*/
				if(j%10 == 0){
					printf("%c", '+');
				}
				else{
					printf("%c", '-');	
				}	
		}
		printf("\n");
	}	
}
	





/* Stage 3 - print statistics */
void 
stage_three(double *coordinate_sums, int num_points){
	/* Print stage heading for stage 3 */
	print_stage_heading(STAGE_NUM_THREE);
	
	double temp;
	int i, largest_sum_point = 1;
	
	/* Using linear search, find the the point with the highest sum of 
	   dimensions 
	*/
	temp = coordinate_sums[0];
	for (i = 1; i < num_points; i++){
		if (*(coordinate_sums+i) > temp){
			temp = *(coordinate_sums+i);
			largest_sum_point = i+1;
		}
	}
	
	/* Print the formatted data */
	printf("Total: %d points\n", num_points);
	printf("Point of largest sum of coordinates: %02d\n", largest_sum_point);
	printf("Largest sum of coordinates: %5.2lf\n", temp);
}






/* Stage 4 - find and print the skyline points */
void 
stage_four(point_t points[], int num_points, int num_dimensions){
	/* Print stage heading for stage 4 */
	print_stage_heading(STAGE_NUM_FOUR);
	
	int domination[num_points][3];
	int i = 0;
	point_t one_point;
	
	/* Use find_dominations function to update domination array */
	find_dominations(domination, one_point, points, num_points, num_dimensions);
	
	printf("Skyline points:\n");
	for (i = 0; i < num_points; i++){
		/* If first column of any point in domination is equal to 0, it is not
		   dominated meaning it is a skyline point
		*/
		if(domination[i][0] == 0){
			print_one_point(points[i], num_dimensions, i+1);
		}
	}
}






/* Stage 5 - sort and print the skyline points by the number of points 
   they dominate
*/
void 
stage_five(point_t points[], int num_points, int num_dimensions) {
	/* Print stage heading for stage 5 */
	print_stage_heading(STAGE_NUM_FIVE);
	
	int domination[num_points][3], i, j; 
	point_t one_point;
	
	/* Use find_dominations function to update domination array */
	find_dominations(domination, one_point, points, num_points, num_dimensions);	
	
	/* Using insertion sort, sort dominance array in descending order, 
	   depending on how many other points a specific point dominates 
	   (represented by column 2 of dominance array)
	*/
	for (i = 1; i < num_points; i++){
		for (j = i-1; j >= 0; j--){
			if (domination[j+1][1] > domination[j][1]){
				row_swap(domination, j, j+1);	
			}
		}
	}
	
	/* Using insertion sort again, sort dominance array depending on if 2 
	   skyline points dominate the same amount of points, but one appears before
	   the other in the original data input (represented by column 3 of 
	   dominance array)
	*/
	for (i = 0; i < num_points-1; i++){
		for (j = i+1; j < num_points; j++){
			if (((domination[i][0] == 0) && (domination[j][0] == 0)) &&
				(domination[i][1] == domination[j][1]) && (domination[i][2] > 
					domination[j][2])){
						row_swap(domination, i, j);
			}
		}
	}
	
	
	printf("Sorted skyline points:\n");
	for (i = 0; i < num_points; i++){
		if (domination[i][0] == 0){
			print_one_point(points[(domination[i][2])-1], num_dimensions, 
				domination[i][2]);
			printf("Number of points dominated: %d\n", domination[i][1]);
		}
	}
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define STR_LEN 1000

void print_matrix (int ***rotas, int rows, int cols, int depth){
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			for (int k = 0; k < depth; k++){
				printf("%d \n", rotas[i][j][k]);
			}
			printf("\n");
		}
	}
}

void print_matrix_2d (int **matrix, int rows, int cols){
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

int ***inicialize_3d_matrix (int ***matrix, int rows, int cols, int depth){
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			for (int k = 0; k < depth; k++){
				matrix[i][j][k] = 0;
			}
		}
	}
	return matrix;
}

int **inicialize_2d_matrix (int **matrix, int rows, int cols){
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			matrix[i][j] = 0;
		}
	}
	return matrix;
}

int *inicialize_1d_array (int *array, int size){
	for (int i = 0; i < size; i++){
		array[i] = 0;
	}
	return array;
}

// Function to allocate memory for a 3D matrix
int ***allocate_3d_matrix(int depth, int rows, int cols) {
    int ***matrix;

    // Allocate memory for the depth dimension
    matrix = (int ***)malloc(depth * sizeof(int **));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed for depth dimension.\n");
        exit(1);
    }

    // Allocate memory for the rows dimension of each depth
    for (int k = 0; k < depth; k++) {
        matrix[k] = (int **)malloc(rows * sizeof(int *));
        if (matrix[k] == NULL) {
            fprintf(stderr, "Memory allocation failed for row dimension at depth %d.\n", k);
            exit(1);
        }

        // Allocate memory for the columns dimension of each row
        for (int i = 0; i < rows; i++) {
            matrix[k][i] = (int *)malloc(cols * sizeof(int));
            if (matrix[k][i] == NULL) {
                fprintf(stderr, "Memory allocation failed for column dimension at depth %d, row %d.\n", k, i);
                exit(1);
            }
        }
    }
    
    matrix = inicialize_3d_matrix(matrix,depth,rows,cols);
    return matrix;
}

int **allocate_2d_matrix(int rows, int cols) {
    int **matrix;

    // Allocate memory for the rows
    matrix = (int **)malloc(rows * sizeof(int *));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed for rows.\n");
        exit(1);
    }

    // Allocate memory for the columns of each row
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for columns of row %d.\n", i);
            exit(1);
        }
    }
    matrix = inicialize_2d_matrix(matrix, rows, cols);
    
    return matrix;
}

// Function to dynamically allocate a 1D array
int *allocate_array(int size) {
    // Allocate memory for the array
    int *array = (int *)malloc(size * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    array = inicialize_1d_array(array,size);
    return array;
}

char* generate_maxeq (int ***rotas, int **pacotes, int n, int depth, int city, int q, int p){
	char eq[STR_LEN] = "";
	char idx1[32];
	char idx2[32];
	char p_string[32];

	int *city_routes_departure = allocate_array(n);	
	int cont_city_routes_departure = 0;

	for (int i = 0; i < n; i++){
		if (rotas[city][i][0] > 0){
			city_routes_departure[cont_city_routes_departure] = i;
			cont_city_routes_departure++;
		}
	}

	
	int dest;

	strcat(eq,"max: ");
	for (int i = 0; i < cont_city_routes_departure; i++){
		dest = city_routes_departure[i];
		sprintf(idx1,"%d",city+1);
		sprintf(idx2,"%d",dest+1);
		sprintf(p_string,"%d",p);

		strcat(eq,p_string);
		strcat(eq,"*");
		strcat(eq,"x");
		strcat(eq,idx1);
		strcat(eq,idx2);
		if (i < (cont_city_routes_departure - 1)){
			strcat(eq,"+");
		}

	}

	int valor_pac;	
	char v_pac[32];
	for (int i = 0; i < q; i++){

		sprintf(idx1,"%d",i+1);
		valor_pac = pacotes[i][0];
		sprintf(v_pac,"%d",valor_pac);

		strcat(eq,"-");
		strcat(eq,"n");
		strcat(eq,idx1);
		strcat(eq,"*");
		strcat(eq,v_pac);

	}
	
	
	printf("%s\n",eq);

}

char* generate_eq (int ***rotas, int n, int depth, int city){
	char eq[STR_LEN] = "";
	char idx1[32];
	char idx2[32];

	int *city_routes_arrival = allocate_array(n);
	int *city_routes_departure = allocate_array(n);	
	int cont_city_routes_arrival = 0;
	int cont_city_routes_departure = 0;
	int cont_departure_orig = 0;
	for (int i = 0; i < n; i++){
		if (rotas[city][i][0] > 0){
			city_routes_departure[cont_city_routes_departure] = i;
			cont_city_routes_departure++;	
		}
	}
	for (int i = 0; i < n; i++){
		if (rotas[i][city][0] > 0){
			city_routes_arrival[cont_city_routes_arrival] = i;
			cont_city_routes_arrival++;
		}
	}
	
	int dest;
	for (int i = 0; i < cont_city_routes_departure; i++){
		dest = city_routes_departure[i];
		sprintf(idx1,"%d",city+1);
		sprintf(idx2,"%d",dest+1);

		strcat(eq,"x");
		strcat(eq,idx1);
		strcat(eq,idx2);
		if (i < (cont_city_routes_departure - 1)){
			strcat(eq,"+");
		}

		//printf("equation at departures equals %d \n", i);
		//printf("%s\n",eq);
	}
	if (cont_city_routes_departure == 0){
		strcat(eq,"0");
	}	
	strcat(eq,"=");
	
	int orig;
	for (int i = 0; i < cont_city_routes_arrival; i++){
		orig = city_routes_arrival[i];
		sprintf(idx1,"%d",orig+1);
		sprintf(idx2,"%d",city+1);

		strcat(eq,"x");
		strcat(eq,idx1);
		strcat(eq,idx2);
		if (i < (cont_city_routes_arrival- 1)){
			strcat(eq,"+");
		}

		//printf("equation at arrival equals %d \n", i);
		//printf("%s\n",eq);
	}

	if(cont_city_routes_arrival == 0){
		strcat(eq,"0");
	}
	
	//printf("final equation for city %d \n",city+1);
	//if (cont_city_routes_arrival != 0 && cont_city_routes_departure != 0){
	//	printf("%s\n",eq);
	//}
	if (city != 0 && city != n-1){
		printf("%s\n",eq);
	}

}


void generate_products_restric(int ***routes, int **pacotes, int row, int cols, int depth, int m, int q, int k){
	int *ori_city = allocate_array(m);
	int *dest_city = allocate_array(m);
	
	int cont_routes = 0;
	

	char eq[STR_LEN] = "";
	char idx1[32];
	char idx2[32];
	char idx3[32];
	char pidx_k[32];

	for (int i = 0; i < row; i++){
		for (int j = 0; j < cols; j++){
			if (routes[i][j][0] > 0){
				ori_city[cont_routes] = i;
				dest_city[cont_routes] = j;
				cont_routes++;
			}	
		}
	}

	for (int i = 0; i < k; i++){ 
		for (int j = 0; j < m; j++){
			int ori_city_idx = ori_city[j];
			int dest_city_idx = dest_city[j];
			sprintf(idx1,"%d",ori_city_idx+1);
			sprintf(idx2,"%d",dest_city_idx+1);

			sprintf(pidx_k,"%d", routes[ori_city_idx][dest_city_idx][i+1]);
			
			strcat(eq,"a");
			strcat(eq,idx1);
			strcat(eq,idx2);
			strcat(eq,"0");
			strcat(eq,"*");
			strcat(eq,pidx_k);	
			strcat(eq,"+");
			
			strcat(eq,"a");
			strcat(eq,idx1);
			strcat(eq,idx2);
			strcat(eq,"1");
			strcat(eq,"*");
			strcat(eq,pidx_k);	
			
			if (j < (m-1)){
				strcat(eq,"+");
			}
		}
		strcat(eq,"<=");
		for (int u = 0; u < q; u++){
			sprintf(idx1,"%d",u+1);
			sprintf(pidx_k,"%d",pacotes[u][i+1]);
			strcat(eq,"n");
			strcat(eq,idx1);
			strcat(eq,"*");
			strcat(eq,pidx_k);
			if(u < (q-1)){
				strcat(eq,"+");
			}		
		}

		printf("%s \n", eq);
		eq[0] = '\0';
	}
	
	char f_route[32] = "";

	char restric1[STR_LEN] = "";
	char restric2[STR_LEN] = "";
	char restric3[STR_LEN] = "";
	char restric4[STR_LEN] = "";
	char restric5[STR_LEN] = "";
	char restric6[STR_LEN] = "";

	for (int i = 0; i < m; i++){
		int ori_city_idx = ori_city[i];
		int dest_city_idx = dest_city[i];
		sprintf(idx1,"%d",ori_city_idx+1);
		sprintf(idx2,"%d",dest_city_idx+1);
		sprintf(f_route,"%d", routes[ori_city_idx][dest_city_idx][0]);	


		strcat(restric1,"x");
		strcat(restric1,idx1);
		strcat(restric1,idx2);
		strcat(restric1,"=");
		strcat(restric1,"a");
		strcat(restric1,idx1);
		strcat(restric1,idx2);
		strcat(restric1,"0");
		strcat(restric1,"-");
		strcat(restric1,"a");
		strcat(restric1,idx1);
		strcat(restric1,idx2);
		strcat(restric1,"1");

		
		strcat(restric2,"a");
		strcat(restric2,idx1);
		strcat(restric2,idx2);
		strcat(restric2,"0");
		strcat(restric2,">=");	
		strcat(restric2,"0");

		strcat(restric3,"a");
		strcat(restric3,idx1);
		strcat(restric3,idx2);
		strcat(restric3,"0");
		strcat(restric3,"<=");
		strcat(restric3,f_route);
		strcat(restric3,"*");
		strcat(restric3,"y");
		strcat(restric3,idx1);
		strcat(restric3,idx2);

		strcat(restric4,"a");
		strcat(restric4,idx1);
		strcat(restric4,idx2);
		strcat(restric4,"1");
		strcat(restric4,">=");	
		strcat(restric4,"0");

		strcat(restric5,"a");
		strcat(restric5,idx1);
		strcat(restric5,idx2);
		strcat(restric5,"1");
		strcat(restric5,"<=");
		strcat(restric5,f_route);
		strcat(restric5,"*");
		strcat(restric5,"(1-y");
		strcat(restric5,idx1);
		strcat(restric5,idx2);
		strcat(restric5,")");
		

		strcat(restric6,"bin ");
		strcat(restric6,"y");
		strcat(restric6,idx1);
		strcat(restric6,idx2);
		

		printf("%s \n", restric1);
		printf("%s \n", restric2);
		printf("%s \n", restric3);
		printf("%s \n", restric4);
		printf("%s \n", restric5);
		printf("%s \n", restric6);

	
		restric1[0] = '\0';
		restric2[0] = '\0';
		restric3[0] = '\0';
		restric4[0] = '\0';
		restric5[0] = '\0';
		restric6[0] = '\0';
	}
}



int main(){
	int n,m,k,q,p;
	scanf("%d", &n);
	scanf("%d", &m);
	scanf("%d", &k);
	scanf("%d", &q);
	scanf("%d", &p);
	
	//int rotas [n][n][k+1];

	int *** rotas = allocate_3d_matrix(n,n,k+1);
	int **  pacotes = allocate_2d_matrix(q,k+1);

	//int pacotes [q][k]; 
	int r1,r2,w,rec;
	for (int i = 0; i < m; i++){
		scanf("%d", &r1);
		scanf("%d", &r2);
		scanf("%d", &w);
		rotas[r1-1][r2-1][0] = w;
		rotas[r2-1][r1-1][0] = -w;
		
		for(int j = 0; j < k; j++){
			scanf("%d", &rec);
			rotas[r1-1][r2-1][j+1] = rec;
			rotas[r2-1][r1-1][j+1] = rec;
		}
			

	}
	
	for (int i = 0; i < q; i++){
		for (int j = 0; j < k+1; j++){
			scanf("%d", &pacotes[i][j]);
		}
	}

	//print_matrix(rotas,n,n,k+1);
	//print_matrix_2d(pacotes,q,k+1);	
	printf("\n\n\n\n");
	
	generate_maxeq(rotas,pacotes,n,k+1,0,q,p);	


	for (int i = 1; i < n; i++){
		generate_eq(rotas,n,k+1,i);	
	}

	generate_products_restric(rotas,pacotes,n,n,k+1,m,q,k);

	//print_matrix(rotas,n,n,k+1);	
}


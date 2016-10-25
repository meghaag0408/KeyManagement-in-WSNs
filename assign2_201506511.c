/*************************************************************************************
Topic: Implementation of the Blom’s single-space key pre-distribution scheme

Name - Megha Agarwal
Roll Number - 201506511
Course - M.Tech CSIS - PG2

*************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>


//Change the assumptions here.
#define totalnodes 600
#define lambda_value 100
#define qmax 1000
#define qmin 2

typedef struct 
{
  int x; /* x-coordinate of sensor node location in target field */
  int y; /* y-coordinate of sensor node location in target field */ 
  int neighbour_size;
  int *phynbr;
} 
sensor;

sensor sensor_node[100000];
long long int matrix_G[101][600], matrix_D[101][600], matrix_A[101][600], matrix_A_transpose[600][101];
long long int matrix_K[600][600];
// ATranpose = Transpose(D.G)

double euclidean_distance(int x1, int y1, int x2, int y2)
{  
  double diffx = x1 - x2;
  double diffy = y1 - y2;
  double diffx_sqr = pow(diffx, 2);
  double diffy_sqr = pow(diffy, 2);
  double distance = sqrt (diffx_sqr + diffy_sqr);
  return distance;
}

void generating_random_numbers(int n, FILE *fp2)
{
  time_t t;
  char str[15];
  int i, j, x, y, index, n1, n2;
  srand((unsigned) time(&t));
  sprintf(str, "%d", n);
  fprintf(fp2,"%s\n", str);
  int flag=0;

  //allocation of x-y values
  for(i=0; i<n; i++)
            {
            flag=0;
            n1 = rand() % 500 + 1;
            n2 = rand() % 500 + 1; 
            for(j=0; j<i; j++)
                    {
                      if(sensor_node[j].x == n1 && sensor_node[j].y == n2)
                        {
                          flag=1;
                          i--;
                          break;
                        }
                    }
            if(flag!=1)
                  {
                      index++;
                      sensor_node[i].x = n1;
                      sensor_node[i].y = n2;
                      sprintf(str, "%d", n1);
                      fprintf(fp2,"%s ", str);
                      sprintf(str, "%d", n2);
                      fprintf(fp2,"%s\n", str);
                  }           
            
            }
  fclose(fp2);
}

void finding_neighbour_sensor_nodes(int n)
{ 
  double distance;
  int i, j, k;
  for(i=0; i<n; i++)
  {
      sensor_node[i].neighbour_size=0; 
      int phy_neighbours_array[100000];  
      for(j=0; j<n; j++)
        {
        if(i!=j)
            {
              distance = euclidean_distance(sensor_node[i].x, sensor_node[i].y, sensor_node[j].x, sensor_node[j].y);
              if(distance<25.00)
                {
                  phy_neighbours_array[sensor_node[i].neighbour_size] = j;
                  sensor_node[i].neighbour_size++;       
                }
            }
        }
    //physical_neighbour_size = physical_neighbour_size + sensor_node[i].phynbrsize;
     sensor_node[i].phynbr  =(int*)malloc(sensor_node[i].neighbour_size*sizeof(int));
     for(k=0; k<sensor_node[i].neighbour_size; k++)
        {
          *(sensor_node[i].phynbr+k) = phy_neighbours_array[k];
        }
  }

}

void printing_neighbour_list(int n)
{
  int i, j;
  for(i=0; i<n; i++)
  {
      printf("%d\n", i);
       for(j=0; j<sensor_node[i].neighbour_size; j++)
        {
          printf("%d ", sensor_node[i].phynbr[j]);
        }
        printf("\n");
  }

}

long long int power(int x, unsigned long long int y, long long int p)
{
    long long int res=1;
    x=x%p;
    while(y>0)
    {
        if(y&1)
            res=(res*x)%p;
 
        y=y>>1;
        x=(x*x)%p;
    }
    return res;
}
 

bool miillerTest(long long int d, long long int n)
{
    long long int a = 2 + rand() % (n - 4);
    long long int x = power(a, d, n);
    if (x==1 || x==n-1)
       return true;
    while(d!=n-1)
    {
        x=(x*x)%n;
        d*= 2;
        if (x==1)      return false;
        if (x==n-1)    return true;
    }
    return false;
}
 

bool isPrime(long long int n, long long int k)
{
    long long int i;
    if (n<=1 || n==4)  return false;
    if (n<=3) return true;
 
    long long int d=n-1;
    while (d % 2 == 0)
        d /= 2;
    
    for (i=0; i<k; i++)
         if (miillerTest(d, n) == false)
              return false;
 
    return true;
}

long long int finding_power_mod(long long g, long long int n, long long int q)
{
    long long int temp=1, i;
    for(i=1; i<=n; i++)
      temp = (temp*g)%q;
    return temp;
}

long long int finding_primitive_root(long long int q)
{
   long long int i, j, g, n;
   long long int arraylist[q], hash[q], primitive_roots[q], index=0;
   for(g=2; g<q; g++)
   {
    memset(hash, 0, sizeof(hash));
    for(n=1; n<q; n++)
       {
        long long int temp = finding_power_mod(g, n, q);
        hash[temp]++;         
       }
    int primitive_flag=1;
    for(i=1; i<q; i++)
       {
        if(hash[i]!=1)
            {
              primitive_flag=0;
              break;
            }
       }
    if(primitive_flag==1)
        return g;
   }

   return 0;
}

long long int oddrand(long long int start, long long int end)
{
   long long int num = rand() % (end - start) + start;
   if(num%2==0)
    num++;
   return(num);
}

void creation_of_matrixG(long long int g, long long int n, long long int q)
{
  long long int lambda = 100;
  int i, j; 
  for(i=0; i<lambda+1; i++)
  {
      for(j=0; j<n; j++)
      {
          if(i==0)
            matrix_G[i][j]=1;
          else if(i==1)
            matrix_G[i][j]=finding_power_mod(g, j+1, q);
          else
            matrix_G[i][j]=finding_power_mod(matrix_G[1][j], i, q);
      }
  }
}

void printing_matrixG(long long int matrix_G[][600], long long int n, long long int lambda)
{
  int i, j; 
  printf("HERE\n");
  for(i=0; i<lambda+1; i++)
  {
      for(j=0; j<n; j++)
      {
          printf("%lld ", matrix_G[i][j]);
      }
   printf("\n");
  }
}

void matrix_D_generation(long long int lambda, long long int q)
{
  long long int a; 
  int i, j;
  for(i=0; i<lambda+1; i++)
    {
        for(j=0; j<lambda+1; j++)
        {
            if(i<=j)
            {
              a = rand() % (q-1) + 1;
              matrix_D[i][j]=a;
              matrix_D[j][i]=a;

            }
           
        }
    }
/*
    //Printing Matrix D
    for(i=0; i<lambda+1; i++)
    {
        for(j=0; j<lambda+1; j++)
        {
            printf("%lld ", matrix_D[i][j]);
        }
     printf("\n");
    }*/

}
void multiply_matrics_matrix_Dmatrix_G(long long int lambda, long long int n, long long int q)
{
  int i, j, k;
    for (i=0; i<lambda+1; i++)
    {
        for (j=0; j<n; j++)
        {
            matrix_A[i][j] = 0;
            for (k=0; k<lambda+1; k++)
                matrix_A[i][j] = (matrix_A[i][j] +((matrix_D[i][k]*matrix_G[k][j])%q))%q;
        }
    }
     //Transposing the matrix
    for (i=0; i<lambda+1; i++)
    {
        for (j=0; j<n; j++)
        {
            matrix_A_transpose[j][i] = matrix_A[i][j];
        }
    }

    /*
    //Printing Matrix A
    for(i=0; i<n; i++)
    {
        for(j=0; j<lambda+1; j++)
        {
            printf("%lld ", matrix_A_transpose[i][j]);
        }
     printf("\n");
    }*/
}

void creation_of_matrixK_multiplyAG(long long int lambda, long long int n, long long int q)
{
  int i, j, k;
    for (i=0; i<n; i++)
    {
        for (j=0; j<n; j++)
        {
            matrix_K[i][j] = 0;
            for (k=0; k<lambda+1; k++)
                matrix_K[i][j] = (matrix_K[i][j] +((matrix_A_transpose[i][k]*matrix_G[k][j])%q))%q;
        }
    }

    printf("Priniting subset (5*5) symmetric matrix K\n");
    //Printing Matrix K
    for(i=0; i<5; i++)
    {
        for(j=0; j<5; j++)
        {
            printf("%lld ", matrix_K[i][j]);
        }
     printf("\n");
    }
}


int main()  
{
    int L1 = 500, L2=500;  
    FILE *fp2;
    fp2=fopen("deployment_201506511.txt", "w");
    if(fp2==NULL) 
      {
        perror("Error opening file.");
      }
    
    FILE *fp1;
    fp1=fopen("name.gp", "w");
    char string[10000] = "plot[0:500][0:500] \"";
    strcat(string, "deployment_201506511.txt");
    strcat(string, "\" every ::1 using 1:2");
    fprintf(fp1,"%s", string);
    fclose(fp1); 
    
    generating_random_numbers(totalnodes, fp2);
    finding_neighbour_sensor_nodes(totalnodes);
    //printing_neighbour_list(n);


    //Finding primitive root of Finite Field GF(q) where q is large prime
    int k=10;
    long long int primitive_root_g, q;
    bool value = false;
    while(value!=true)
    {
      q = oddrand(qmin, qmax);
      value = isPrime(q, k);
      if(value==true)
        {
          primitive_root_g=finding_primitive_root(q);
        }

    }
    printf("Primitie Root Genarated : %lld, For Prime Number: %lld\n", primitive_root_g, q);
    creation_of_matrixG(primitive_root_g, totalnodes, q);
    printing_matrixG(matrix_G, 600, 100);

    matrix_D_generation(lambda_value, q);
    multiply_matrics_matrix_Dmatrix_G(lambda_value, totalnodes, q);
    creation_of_matrixK_multiplyAG(lambda_value, totalnodes, q);


    //Writing NEIGHBOUR LIST TO FILE
     FILE *fp3;
     fp3=fopen("keys.txt", "w");
     int i, j;
     fprintf(fp3, "Physical Neighout List of Sensor Nodes\n");
    for(i=0; i<totalnodes; i++)
    {
         fprintf(fp3, "NL%d : ", i);
         for(j=0; j<sensor_node[i].neighbour_size; j++)
          {
            fprintf(fp3, "%d ", sensor_node[i].phynbr[j]);
          }
          fprintf(fp3, "\n");
    }
    fprintf(fp3, "\n");
    fprintf(fp3, "Key Neighbours : Key(i,j) of Sensor Nodes\n");

    for(i=0; i<totalnodes; i++)
    {
         fprintf(fp3, "Key(i,j) %d : ", i);
         for(j=0; j<sensor_node[i].neighbour_size; j++)
          {
            fprintf(fp3, "%lld ", matrix_K[i][sensor_node[i].phynbr[j]]);
          }
          fprintf(fp3, "\n");
    }

    fclose(fp3);
    system("gnuplot -p 'name.gp'");     
    return 0;
}
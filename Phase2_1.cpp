#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <vector>
using namespace std;

typedef struct 
{
  int x; /* x-coordinate of sensor node location in target field */
  int y; /* y-coordinate of sensor node location in target field */ 
  int neighbour_size;
  vector<int> neighbours_list;  
} 
sensor;

sensor sensor_node[100000];

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
  for(int i=0; i<n; i++)
  {
        sensor_node[i].neighbour_size=0;  
        for(int j=0; j<n; j++)
              {
              if(i!=j)
                    {
                      distance = euclidean_distance(sensor_node[i].x, sensor_node[i].y, sensor_node[j].x, sensor_node[j].y);
                      if(distance<25.00)
                              {
                                sensor_node[i].neighbours_list.push_back(j);
                                sensor_node[i].neighbour_size++;       
                              }
                    }
              }
  }

}

void printing_neighbour_list(int n)
{
  for(int i=0; i<n; i++)
  {
       cout<<"Sensor Node: "<<i<<endl;
       for(int j=0; j<sensor_node[i].neighbours_list.size(); j++)
        {
          cout<<sensor_node[i].neighbours_list[j]<<" ";


        }
        cout<<endl;
  }


}
int main()  
{
    int L1 = 500, L2=500, n;  
    
    cout<<"Enter the number of sensor nodes"<<endl;
    cin>>n;
    
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
    
    generating_random_numbers(n, fp2);
    finding_neighbour_sensor_nodes(n);
    printing_neighbour_list(n);

    //average_neighbour_size = finding_physical_neighbours(n, atoi(argv[2]), atoi(argv[3]));
    //finding_key_neighbours(n, atoi(argv[2]), atoi(argv[3]));
 
    system("gnuplot -p 'name.gp'"); 
    
    return 0;
}
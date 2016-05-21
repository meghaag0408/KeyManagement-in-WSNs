****************************************************************************************************************************

NAME - Megha Agarwal
Roll No - 201506511
M.tech CSIS(PG1)
****************************************************************************************************************************

****************************************************************************************************************************
     Similation of the key management protocol in WSNs:A key-management scheme for distributed sensor networks.
____________________________________________________________________________________________________________________________

________________________________________________Task 1_____________________________________________________________
-> No. of sensor nodes are taken as an input from the user, say n.
-> n no. of unique sample points are generated in the range of (L1 X l2) i.e (500 X 500).
-> The no of sensor nodes, and all the points(locations) are stored in the file provided as an argument.
-> Gnuplot is called using System command and the graph is plotted by dynamically creating "name.gp" file at the run time.

___________________________________________________Task 2________________________________________________________

-> This task aims at finding the direct key establishment in the simulated environment.
-> All the physical neighbours are find out if the lie in the radius < 25.0 (i.e. euclidean distance is calculated from all
the remaining points).
-> Hence the key neighbours are also calculated using the physical neighbours.
-> Simulated Average Network Connectivity : Sigma(Key neighbors for all nodes) / Sigma(Physical Neighbours for all nodes). This
gives the network connectivity for the direct key establishment phase.


_________________________________________________Task 3_________________________________________________________

-> This task aims at finding the path key establishment in the simulated environment for the physical neighbours who cannot 
directly exchange a key, so will exchange the key using HOP - 1 or HOP -2.
-> Hence the one_hop_key_count and two_hop_key_count is updated.
-> Simulated Average Network Connectivity for one hop : Sigma(Key neighbors for all nodes + one_hop_key_count) / 
Sigma(Physical Neighbours for all nodes). This gives the network connectivity for the path key establishment phase using one hop.
-> Simulated Average Network Connectivity for two hop : Sigma(Key neighbors for all nodes + one_hop_key_count + two_hop_key_count)
/ Sigma(Physical Neighbours for all nodes). This gives the network connectivity for the path key establishment phase using two hop.

____________________________________________________________________________________________________________________________
Note: For Theoritical Network Connectivities, formulaes in the slides are used and finally the simulation is compared with 
theoritical value.
____________________________________________________________________________________________________________________________

==============================================================================================================================
 						Compilation and running 
==============================================================================================================================
-> We need to compile using -lm to use math library.
-> The file name needs to mention which will store the no of sensor nodes taken as an input from the user and all the sensor node
points generated during Task1.
-> We also need to mention the keyring size as arg2 and keypool size and arg3.

				$ gcc 201506511_assign_2.c -lm

				$ ./a.out sensor.data 100 40000


==============================================================================================================
 						    Sample Output
==============================================================================================================
Enter the no of sensor nodes
10000
Scaling communication range...
Average distance = 260.592885
Communication range of sensor nodes = 25.00

Computing physical neighbors...
Average neighborhood size = 74.107803

EG scheme
Distributing keys...

Computing key neighborhood in direct key establishment phase...
Computing key neighborhood in path key establishment phase...

Network Connectivities : 

Simulated Average Network Connectivity
0.221720
Theoritical Network Connectivity
0.221685
Simulated Average Network Connectivity for one hop
0.900693
Theoritical Average Network Connectivity for one hop
0.981410
Simulated Average Network Connectivity for two hop
0.999876
Theoritical Average Network Connectivity for two hop
1.000000



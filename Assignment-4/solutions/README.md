1. Running:

place the input in "/ds-m21/team_9/inputs/1/input.txt", the output will be stored at "/ds-m21/team_9/outputs/1/test_out".

remember to delete the output folder before running. 

algorithm:

Mapper: u -> U1,u  where U1 is the sorted camel case word of u. 

Reducer: since keys are sorted and one key will be passed to only one reducer that means all the words corresponding to one U1 will be present in the same reducer and we can collect and print them. 


2.	Running:

go to the folder /home/ds-m21-user9/HW-4/2, place input in /home/ds-m21-user9/HW-4/2/input.txt, run /home/ds-m21-user9/HW-4/2/run.sh, output will be at /home/ds-m21-user9/HW-4/2/output.txt


algorithm:

We are using bfs. Storing only the edges. If the edge if from u->v, we are storing it as v,u as we wanted to sort the file by v (for further processing in the reducer). 

Firstly the algorithm stores v,u,flg where flg = 1 if the node v is processed and 0 otherwise. For each v in the graph for which flg = 0, the mapper calculates its neighbours using the get request and scraping the element under "a" tag.

Now reducer takes all the edges incident at v (as it is sorted by v so all the edges incident at v will be in a single reducer) and assignes 0 to only one edge and 1 to rest of the others. If anyone edge was 1 before, then all the edges is assigned 1 (because that means v is visited). 


This runs in loop for k times. And we have all the edges. But to format our output (remove flags), we are running another simple map, reduce. 

Imp: For calculating the number of nodes and number of edges, we are appending a "\n" in temp files and calculating the number of lines after that. 

Corner case handled: 
	1. self loops are removed. 
	2. multiple edges are removed
	3.url is prepended with valid http so that it is valid for get request. 


3. Running:

go to the folder /home/ds-m21-user9/HW-4/3, place input in /home/ds-m21-user9/HW-4/3/input.txt, run /home/ds-m21-user9/HW-4/3/run.sh, output will be at /home/ds-m21-user9/HW-4/3/output.txt

algorithm: 

We are using colouring method. First the color of each node is same as the node number. In each iteration, the color of the node becomes the minimum of its color and its neighbour. So at the end of atmost n (n<=100) iterations (maximum depth can be n), all the nodes will be assigned correct color. For performing this task we are using one map reduce in loop for atmost 100 times.

Mapper : emits u,v, color_u,color_v and its reverse i.e. v,u,color_v,color_u. 

Reducer : for each u, calculates the minimum color and then emit the edge by reassigning the minimum color. 


The output will be the edges and the colours. So to format the output we are running another simple map reduce which keys the nodes by the color so that all the nodes with similar color are given to one reducer and reducer can print them seperated by spaces. 





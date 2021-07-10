#include<stdio.h>//Graph Done
#include<stdlib.h>
#include <time.h>

#define MAX_VERTICES 10000
#define MAX_EDGES 3000

typedef unsigned int vertex;

int main(){

    /*number of nodes in a graph*/
    srand ( time(NULL) );
    int numberOfVertices = rand() % MAX_VERTICES;

    /*number of maximum edges a vertex can have*/
    srand ( time(NULL) );
    int maxNumberOfEdges = rand() % MAX_EDGES;
    /*graphs is 2 dimensional array of pointers*/
    if( numberOfVertices == 0)
        numberOfVertices++;
    vertex ***graph;
    printf("Total Vertices = %d, Max # of Edges = %d\n",numberOfVertices, maxNumberOfEdges);

    /*generate a dynamic array of random size*/
    if ((graph = (vertex ***) malloc(sizeof(vertex **) * numberOfVertices)) == NULL){
        printf("Could not allocate memory for graph\n");
        exit(1);
    }

    /*generate space for edges*/
    int vertexCounter = 0;
    /*generate space for vertices*/
    int edgeCounter = 0;

    for (vertexCounter = 0; vertexCounter < numberOfVertices; vertexCounter++){
        if ((graph[vertexCounter] = (vertex **) malloc(sizeof(vertex *) * maxNumberOfEdges)) == NULL){
            printf("Could not allocate memory for edges\n");
            exit(1);
        }
        for (edgeCounter = 0; edgeCounter < maxNumberOfEdges; edgeCounter++){
            if ((graph[vertexCounter][edgeCounter] = (vertex *) malloc(sizeof(vertex))) == NULL){
                printf("Could not allocate memory for vertex\n");
                exit(1);
            }
        }
    }

    /*start linking the graph. All vetrices need not have same number of links*/
    vertexCounter = 0;edgeCounter = 0;
    for (vertexCounter = 0; vertexCounter < numberOfVertices; vertexCounter++){
        printf("%d:\t",vertexCounter);
        for (edgeCounter=0; edgeCounter < maxNumberOfEdges; edgeCounter++){
            if (rand()%2 == 1){ /*link the vertices*/
                int linkedVertex = rand() % numberOfVertices;
                graph[vertexCounter][edgeCounter] = graph[linkedVertex];
                printf("%d, ", linkedVertex);
            }
            else{ /*make the link NULL*/
                graph[vertexCounter][edgeCounter] = NULL;
            }
        }
        printf("\n");
    }
    printf("\n");

    int neighbourArray[10000];
    for(int i=0;i<10000;i++){
        neighbourArray[i]=-1;
    }
    /*for(int i=0;i<10000;i++){
        printf(" %d ",neighbourArray[i]);
    }*/
    for (vertexCounter = 0; vertexCounter < numberOfVertices; vertexCounter++){
        printf("%d:\t",vertexCounter);
        //findVertexNeighbours(vertexCounter,graph,maxNumberOfEdges);
        findVertexNeighboursAndStoreInArray(vertexCounter,graph,maxNumberOfEdges,neighbourArray);
        for(int i=0;i<10000;i++){
            if(neighbourArray[i]!= -1)
            printf(" %d ",neighbourArray[i]);
        }
        printf("\n");
    }

    return 1;
}
int findVertexNumber(int location,vertex*** graph){
    int i=0;
    for(i=0; i<100000;i++){
        if((int)graph[i]==location){
            break;
        }
    }
    return i;
}
void findVertexNeighbours(int ivertex,vertex*** graph,int maxNumberOfEdges){
   int edgeCounter=0;
        for (edgeCounter=0; edgeCounter < maxNumberOfEdges; edgeCounter++){
                if(graph[ivertex][edgeCounter]!=NULL){

                    printf("%d, ", findVertexNumber(graph[ivertex][edgeCounter],graph));
                }
        }
        printf("\n");
}
void findVertexNeighboursAndStoreInArray(int ivertex,vertex*** graph,int maxNumberOfEdges,int arr[10000]){
   int edgeCounter=0;
   int i=0;
        for (edgeCounter=0; edgeCounter < maxNumberOfEdges; edgeCounter++){
                if(graph[ivertex][edgeCounter]!=NULL){
                    arr[i]=findVertexNumber(graph[ivertex][edgeCounter],graph);
                    i++;
                }
        }
}

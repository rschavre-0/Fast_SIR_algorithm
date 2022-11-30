#include<stdio.h>
#include<stdlib.h>
#include <time.h>

#define MAX_VERTICES 10000
#define MAX_EDGES 3000

enum etype { transmit , recover } type;         //type=0 :: Infected and Transmitting ::: type=1 :: Recovered
const float T=0.5;//Transmission Probability
const float Y=0.2;//Recover Probability
typedef unsigned int vertex;//For Graph

/*******NODE FUNCTIONS AND STRUCTURES*******/
typedef struct node{//For List of S,I,R
	int id;
	struct node *link;
}NODE;
NODE *Sroot = NULL;//List of S nodes is stored in Sroot
NODE *Iroot = NULL;//List of I nodes
NODE *Rroot = NULL;//List of R nodes

/*Begin of insert node to list S or list I or list R*/
void insert(int item,NODE **root)
{
	//if( item != -1){
        NODE *tmp,*q;

        tmp = (NODE *)malloc(sizeof(NODE));
        tmp->id = item;
        /*List is empty*/
        if( (*root) == NULL )
        {
            tmp->link = (*root);
            (*root) = tmp;
        }
        else
        {
            q = (*root);
            while( q->link != NULL)
                q=q->link;
            tmp->link = q->link;
            q->link = tmp;
        }
	//}
}
/*End of insert*/

/*Begin of del*/
int del(int id,NODE **root)
{
	NODE *tmp,*prevtmp;
	if((*root) == NULL){
		printf("List is Empty\n");
		return -1;
		}
	else
	{
		prevtmp=(*root);
		tmp = (*root);
		while(prevtmp!=NULL){
            if(prevtmp->id==id){
                break;
            }
            prevtmp = prevtmp->link;
		}if(prevtmp==NULL){
            return -2;//NOT FOUND IN LIST
		}else{
            prevtmp->id=(*(root))->id;
            (*root)->id=id;
            int deleted=tmp->id;
            //printf("Deleted item is %d\n",(*root)->id);
            (*root) = (*root)->link;
            free(tmp);
            return deleted;
		}
	}
}
/*End of del*/

/*Begin of find*/
NODE* find(int nodeId,NODE *root){
    if(root == NULL){
		//printf("Cannot Find.List is Empty\n");
		return NULL;
    }
    else{
        NODE *prevtmp;
        prevtmp = root;
		while(prevtmp!=NULL){
            if(prevtmp->id==nodeId){
                break;//ID FOUND
            }
            prevtmp = prevtmp->link;
		}
		if(prevtmp==NULL){
            return NULL;//NOT FOUND IN LIST
		}else{//FOUND
            return prevtmp;
		}
    }
}
/*End of find*/

/*Begin of display*/
void display(NODE *root)
{
	NODE *ptr;
	ptr = root;
	if(root == NULL)
		printf("List is empty\n");
	else
	{
		//printf("Item\n");
		printf("\t");
		while(ptr != NULL)
		{
			printf("%d\t",ptr->id);
			ptr = ptr->link;
		}
	}
}
/*End of display*/
/*******GRAPH FUNCTIONS*******/
int findVertexNumber(int location,vertex*** graph){
    int i=0;
    for(i=0; i<100000;i++){
        if((int)graph[i]==location){
            break;
        }
    }
    return i;
}
void findVertexNeighboursAndStoreInArray(int ivertex,vertex*** graph,int maxNumberOfEdges,int arr[10000]){
   int edgeCounter=0;
   int i=0;
        for (edgeCounter=0; edgeCounter < maxNumberOfEdges; edgeCounter++){
                if(graph[ivertex][edgeCounter]!=NULL){
                    arr[i]=findVertexNumber((int)graph[ivertex][edgeCounter],graph);
                    i++;
                }
        }
}
void findVertexNeighbours(int ivertex,vertex*** graph,int maxNumberOfEdges){
   int edgeCounter=0;
        for (edgeCounter=0; edgeCounter < maxNumberOfEdges; edgeCounter++){
                if(graph[ivertex][edgeCounter]!=NULL){

                    printf("%d, ", findVertexNumber((int)graph[ivertex][edgeCounter],graph));
                }
        }
        printf("\n");
}

/*******QUEUE FUNCTIONS AND STRUCTURES*******/
typedef struct Event{
	int time;//int priority;
	int type;//0=transmit ; 1=recover
	int nodeId;//The event will occur on Node
	//int nodeStatus;//0=susceptible ; 1=not susceptible
}EVENT;
//EVENT *front = NULL;

typedef struct qnode
	{
	int priority;
	struct Event *event;
	struct qnode *link;
}QNODE;
QNODE *front = NULL;

/*Begin of insertToQueue*/
void insertToQueue(struct Event* item)
{
	QNODE *tmp,*q;

	tmp = (QNODE *)malloc(sizeof(QNODE));
	tmp->event = item;
	tmp->priority = item->time;
	/*Queue is empty or item to be added has priority more than first item*/
	if( front == NULL || item->time < front->priority )
	{
		tmp->link = front;
		front = tmp;
	}
	else
	{
		q = front;
		while( q->link != NULL && q->link->priority <= item->time )
			q=q->link;
		tmp->link = q->link;
		q->link = tmp;
	}
}
/*End of insertToQueue*/

/*Begin of delFromQueue*/
void delFromQueue()
{
	QNODE *tmp;
	if(front == NULL)
		printf("Queue Underflow\n");
	else
	{
		tmp = front;
		//printf("Deleted item is %d %d %d \n",tmp->event->time,tmp->event->type,tmp->event->nodeId);
		front = front->link;
		free(tmp);
	}
}
/*End of delFromQueue*/

/*Begin of displayQueue*/
void displayQueue()
{
	QNODE *ptr;
	ptr = front;
	if(front == NULL)
		printf("Queue is empty\n");
	else
	{
		printf("Queue is :\n");
		printf("Priority       Item\n");
		while(ptr != NULL)
		{
			printf("%5d %5d %d %d \n",ptr->priority,ptr->event->time,ptr->event->type,ptr->event->nodeId);
			ptr = ptr->link;
		}
	}
}
/*End of displayQueue*/
struct Event* createEventNode(int time,int type,int nodeId){//,int nodeStatus){
    struct Event *tmp;
        tmp = (struct Event*)malloc(sizeof(struct Event));
        tmp->nodeId=nodeId;
        tmp->time=time;
        tmp->type=type;
        //tmp->nodeStatus=nodeStatus;
    return tmp;
};

/*******PROBABILITY FUNCTION*******/
int toss(float inputProb){
    int favourable = inputProb*10;
    int outcome = rand()%10 + 1;
    if(outcome <= favourable){
        return 0;//Head has occured in toss with probability = inputProb
    }
    return 1;//Tail occured
}

/*******CREATE SIMULATION EVENTS AND ADD TO THE PRIORITY QUEUE*******/
/*
*BELOW CREATES QUEUE FOR ALL POSSIBLE EVENTS  WHEN A NODE IS INFECTED
USE OF RECURSION FOR FURTHER TRANSMISSION
*THIS QUEUE'S END EVENT WILL BE WHEN THERE ARE NO INFECTED NODES PRESENT
*THIS JUST CREATES EVENTS  EITHER TRANSMIT OR RECOVER
/////process_trans_SIR///
*/
void  createEventAndQueue(int timer, int type, int nodeId,vertex ***graph,int maxNumberOfEdges){//createEventAndAttachToQueue
    //displayQueue();
    if(type==transmit){//EVENT IS TRANSMIT
        if( find( nodeId, Sroot) != NULL){//NODE IS SUSCEPTIBLE
            insertToQueue(createEventNode(timer,type,nodeId));//ADD TRANSMIT EVENT OF NODE TO QUEUE
            int count_recovery_time=timer;
            //printf("time = %d count_recovery_time %d\n",timer,count_recovery_time);
                while(toss(Y)!=0){//CALCULATE NODE'S RECOVERY TIME USING Y AS PROBABILITY
                    count_recovery_time++;
                }
            count_recovery_time++;
            insertToQueue(createEventNode(count_recovery_time+timer,recover,nodeId));//ADD RECOVERY EVENT OF NODE TO QUEUE
            insert(del(nodeId,&Sroot),&Iroot);//DELETE NODE FROM S AND INSERT IN I LIST

            //FOR TRANSMISSION VIA NEIGHBOURS
            int *neighbourArray;
            neighbourArray=(int*)malloc(sizeof(int)*10000);
            int i=0;
            for(i=0;i<10000;i++){
                neighbourArray[i]=-1;
            }
            findVertexNeighboursAndStoreInArray(nodeId,graph,maxNumberOfEdges,neighbourArray);//STORE NEIGHBOURS OF THE NODE IN AN ARRAY
            i=0;
             while(neighbourArray[i]!=-1 ){
                int neighbourId=neighbourArray[i];
                if( find( neighbourId, Sroot) != NULL){//IF NEIGHBOUR VERTEX IS IN SUSCEPTIBILE LIST//EVENT TYPE IS TRANSMIT THUS IT BECOMES INFECTED
                    int count_infection_time_for_neighbour=timer;
                    while(toss(T)!=0){
                        count_infection_time_for_neighbour++;//CALCULATE NEIGHBOUR'S INFECTION TIME USING T AS PROBABILITY
                    }
                    count_infection_time_for_neighbour++;
                    if(count_infection_time_for_neighbour <= count_recovery_time){//TRANSMIT USING NEIGHBOUR AS NODE OF TRANSMISSION
                            createEventAndQueue(count_infection_time_for_neighbour,transmit,neighbourId,graph,maxNumberOfEdges);//
                    }
                }
                i++;
            }
            free(neighbourArray);//FREE THE NEIGHBOUR ARRAY AFTER THE NODE HAS TRANSMITTED TO ALL NEIGHBOURS WITH PROBABILITY= T = 0.5
        }
    }
}

int countListNodes(NODE *root){
    if(root == NULL){
		return 0;
    }
    else{
        NODE *prevtmp;
        int counter=0;
        prevtmp = root;
		while(prevtmp!=NULL){
            counter++;
            prevtmp = prevtmp->link;
		}
        return counter;
    }
}
int countQueueNodes(){
    QNODE *ptr;
	ptr = front;
	if(front == NULL)
		return 0;
	else
	{
		int counter=0;
		while(ptr != NULL)
		{
			counter++;
			ptr = ptr->link;
		}
		return counter;
	}
}


int main(){
    //START GRAPH MAKING
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
        //printf("%d:\t",vertexCounter);
        insert(vertexCounter,&Sroot);//ADD ALL VERTICES TO SUSCEPTIBLE LIST
        for (edgeCounter=0; edgeCounter < maxNumberOfEdges; edgeCounter++){
            if (rand()%2 == 1){ /*link the vertices*/
                int linkedVertex = rand() % numberOfVertices;
                graph[vertexCounter][edgeCounter] = graph[linkedVertex];
                //printf("%d, ", linkedVertex);
            }
            else{ /*make the link NULL*/
                graph[vertexCounter][edgeCounter] = NULL;
            }
        }
        //printf("\n");
    }
    printf("\n");//END OF GRAPH MAKING

    srand ( time(NULL) );
    int simulationTime=300;//MAXIMUM SIMULATION RUNNING TIME
    EVENT *currentEvent=NULL;//CURRENT EVENT OR MAX PRIORITY EVENT OR QUEUE'S FRONT ELEMENT
    int startNodeId = rand() % numberOfVertices;//RANDOM NODE FOR CREATING STARTING NODE FOR INFECTION
    int timer=0;
    createEventAndQueue(timer,transmit,startNodeId,graph,maxNumberOfEdges);        //CREATE SIMULATION EVENTS AND STORE THEM ON PRIORITY QUEUE


        do{//DISPLAY THE EVENTS SEQUENCE//Do Processing on Events
                currentEvent=createEventNode(front->event->time,front->event->type,front->event->nodeId);//Store the front event data of queue as current event
                if(currentEvent->type == recover){
                    insert(del(currentEvent->nodeId,&Iroot),&Rroot);//process_rec_SIR
                }
                //printf("day = %d node = %d  type = %d \n",currentEvent->time,currentEvent->nodeId,currentEvent->type);
                if(currentEvent->type==transmit){
                        printf("day = %d node = %d  type = transmit \n",currentEvent->time,currentEvent->nodeId);
                }
                else{//type==recover
                        printf("day = %d node = %d  type = recover \n",currentEvent->time,currentEvent->nodeId);
                }
                free(currentEvent);

                delFromQueue();//free the queue's first element
            }
        while(front!=NULL && front->priority <= simulationTime);
    printf("\nSusceptible Nodes :%d & List, is :\n",countListNodes(Sroot));
    display(Sroot);//Print List
    printf("\n");
    printf("Infected Nodes :%d & List, is :\n",countListNodes(Iroot));
    display(Iroot);
    printf("\n");
    printf("Recovered Nodes :%d & List, is :\n",countListNodes(Rroot));
    display(Rroot);
    //printf("\nTotal Nodes :%d\n",countListNodes(Sroot)+countListNodes(Iroot)+countListNodes(Rroot));
    getch();
    return 1;
}

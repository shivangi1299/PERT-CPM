#include <stdio.h>
#include <stdlib.h>
#include<time.h>

struct node{
    int vertex;
    struct node* next;
};

struct stack{
    int top;
    int capacity;
    int* array;
};

struct node* createNode(int);
struct stack* createStack(int);

struct Graph{
    int numVertices;
    struct node** adjLists;
};

struct Graph* createGraph(int vertices);
void addEdge(struct Graph* graph, int src, int dest);
void printGraph(struct Graph* graph);
void topological(struct Graph* graph, int *order, struct stack* Stack);
int isCyclic(struct Graph* graph);
int isCyclicUtil(struct Graph* graph, int, int*, int*);

int main(){
    clock_t start, end;
    printf("Enter number of modules- ");
    int n;
    scanf("%d",&n);
    printf("Enter the time required to execute each module\n");
    int *time=(int *)malloc(sizeof(int)*n);
    for(int i=0; i<n; i++){
        printf("Time for module %d- ", i+1);
        scanf("%d",&time[i]);
    }

    int big, pos=0;
    int *order=(int *)malloc(sizeof(int)*n);
    for(int i=0;i<n;i++){
        order[i]=i+1;
    }

    for(int i=0; i<n; i++){
        big=0;
        for(int j=0; j<n; j++){
            if(time[j]>big){
                big=time[j];
                pos=j;
            }
        }
        order[i]=pos+1;
        time[pos]=0;
    }

    struct stack* Stack= createStack(n);

    printf("\nEnter the dependencies one by one for each module pairwise\nSo if 1 depends on 2, type '1 2' and press enter to type the next pairwise dependency\nEnter 0 0 if all the dependencies are completed- ");
    struct Graph* graph= createGraph(n);
    int v1,v2;
    scanf("%d%d", &v1,&v2);
    while(v1!=0 && v2!=0){
        addEdge(graph, v1, v2);
        scanf("%d%d",&v1,&v2);
    }

    start = clock();
    if (isCyclic(graph)){
        printf("Graph is cyclic. Cannot execute.");
    }
   // printGraph(graph);
   else{
        topological(graph,order,Stack);
   }

   end = clock();
   printf("\nTime taken %f seconds",(float)(end-start)/CLOCKS_PER_SEC);
       return 0;
}

struct node* createNode(int v){
    struct node* newNode = malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

struct stack* createStack(int capacity){
    struct stack* newStack=(struct stack*)malloc(sizeof(struct stack));
    newStack->capacity=capacity;
    newStack->top=-1;
    newStack->array=(int*)malloc((newStack->capacity)*sizeof(int));
    return newStack;
};

void push (struct stack* Stack, int item){
    Stack->array[++Stack->top]=item;
}

struct Graph* createGraph(int vertices){
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->numVertices = vertices;

    graph->adjLists = malloc((vertices+1) * sizeof(struct node*));

    int i;
    for (i = 1; i < vertices+1; i++)
        graph->adjLists[i] = NULL;

    return graph;
}

void addEdge(struct Graph* graph, int src, int dest){
    // Add edge from dest to src
    struct node* newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

void printGraph(struct Graph* graph){
  int v;
  for (v = 1; v < graph->numVertices+1; v++)
  {
    struct node* temp = graph->adjLists[v];
    printf("\n Adjacency list of vertex %d\n ", v);
    while (temp)
     {
        printf("%d -> ", temp->vertex);
        temp = temp->next;
    }
    printf("\n");
   }
}

void topologicalSortUtil(int i, int *visited, struct Graph* graph,int *output,struct stack* Stack){
    visited[i]=1;
    struct node* temp=graph->adjLists[i];

    while(temp){
        if(visited[temp->vertex]==0){
            topologicalSortUtil(temp->vertex, visited, graph, output,Stack);
        }
        temp=temp->next;
    }
    push(Stack, i);
}

void topological(struct Graph* graph, int *order, struct stack* Stack){

    int* visited = (int*)calloc(graph->numVertices+1, sizeof(int));
    int* output = (int*)calloc(graph->numVertices, sizeof(int));

    int k=graph->numVertices-1;
    for(int i=0; i<graph->numVertices; i++){
        if(visited[order[i]]==0){
            topologicalSortUtil(order[i],visited,graph,output,Stack);
        }
    }

    printf("\nThe order of execution of the modules should be-\n");
    while(k>=0){
        printf("%d\n", Stack->array[Stack->top--]);
        --k;
    }
}

int isCyclic(struct Graph* graph) {
    int* visited = (int*)calloc(graph->numVertices+1, sizeof(int));
    int* recStack = (int*)calloc(graph->numVertices+1, sizeof(int));
    for (int node = 1; node <= graph->numVertices; node++) {
        if (!visited[node] && isCyclicUtil(graph, node, visited, recStack)) return 1;
    }
    return 0;
}

int isCyclicUtil(struct Graph* graph, int v, int* visited, int* recStack) {
    visited[v] = 1;
    recStack[v] = 1;
    for (struct node* neighbour = graph->adjLists[v]; neighbour; neighbour = neighbour->next) {
        if ((!visited[neighbour->vertex] && isCyclicUtil(graph, neighbour->vertex, visited, recStack)) || recStack[neighbour->vertex]) return 1;
    }
    recStack[v] = 0;
    return 0;
}

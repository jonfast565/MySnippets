/*
 *
 * Phylogenic Tree Algorithms *************************
 *
 * Author: Jon Fast
 * Description: Normal 2-approximation algorithms, etc.
 *              Also, contains useful input functions...
 * Last Modified: Nov 29, 2013
 *
 */

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>

// size defines
#define MAX_INPUT_SIZE 10000
#define MAX_FORMAT_SIZE 100

// testing defines
#define INPUT_RANDOM 1
#define DEBUG_INPUT 1
#define PRINT 1

// edge data
struct nNode;
typedef struct nEdge {
    struct nNode* node;
    struct nNode* from;
    int weight;
    bool discovered;
    bool back;
} GraphEdge;

// node data
typedef struct nNode {
    GraphEdge** edges;
    const char* data;
    char character;
    int edgeCount;
    int number;
    bool discovered;
} GraphNode;

// container structure
typedef struct nContainer {
    GraphNode** nodes;
    int nodeCount;
} GraphContainer;

typedef struct nStarContainer {
    GraphNode* center;
    GraphNode** nodes;
    int nodeCount;
} StarGraphContainer;

// input structure
typedef struct nInput {
    char** input;
    int lines;
    int line_alloc;
} Input;

// linked list
typedef struct nLink {
    int root;
    char* data;
    struct nLink* next;
} Link;

// binary leaf
typedef struct nLeaf {
    int root;
    const char* data;
    int numLeaves;
    struct nLeaf** leaves;
    int* weights;
} Leaf;

// distance matrix
typedef struct nMatrix {
    int numTaxa;
    int** dist;
    int** qScoring;
} DistanceMatrix;

// prototypes (helper)
char* readFileName();
int numChars(char*, char);
int strlenAlt(const char*);
typedef int (*comparator)(const char*, const char*);

// prototypes (input)
int getTestInput(char*, int, int, Input**);
int getInput(const char*, int, Input**);
void summarizeInput(Input*);
void destroyInput(Input*);

// prototypes (graph)
int createCliqueContainer(GraphContainer**, Input*);
int createStarContainer(StarGraphContainer**, Input*);
void resetContainer(GraphContainer*);
void summarizeContainer(GraphContainer*);
void summarizeStarContainer(StarGraphContainer*);
void destroyContainer(GraphContainer*);
void minimumSpanningTree(GraphContainer*, GraphNode**);
void depthFirstGraph(GraphNode*);
void calculateContainerScores(GraphContainer*, comparator);
void twoApproximation(GraphContainer*, Leaf**);
void buildTree(GraphNode*, Leaf**);
void printTree(Leaf*);
void printTreeInternal(Leaf*, int);
bool allNodesDiscovered(GraphContainer*);
bool invalidEdgeWeights(GraphContainer*);
void calculateNeighborJoin(StarGraphContainer*, comparator);

// prototypes (language)
char language[] = {'A', 'C', 'G', 'T', '\0'};
int validateLanguage(const char*, const char*);
int hammingDistance(const char*, const char*);

// main
int main(int argc, const char* argv[])
{
    // welcome message
    printf("--- Welcome to Jon's Neighbor-Join & 2-Approximation! ---\n\n");
    
    // predeclared filename
    char* filename;
    Input* ipt = NULL;
    
    if (DEBUG_INPUT)
        // set up the path
        filename = "/Users/jonfast/Desktop/phylogeny.txt";
    else
        // read the file name
        filename = readFileName();
    
    clock_t start, end;
    // experiment
    // i is the number of taxa
    // j is the number of base pairs
    for (int j = 0; j < 1; j++) {
        
    // setup and fill the input structures
    if (INPUT_RANDOM)
        getInput(filename, 1000, &ipt);
    else
        getTestInput(language, 300, j, &ipt);
    
    // summarize input if appropriate
    if (PRINT)
        summarizeInput(ipt);
    
    // free filename if input is standard
    if (!DEBUG_INPUT)
        free(filename);
        
    start = clock();
    
    // setup the graph container
    StarGraphContainer* containerStar = NULL;
    createStarContainer(&containerStar, ipt);
    
    // calculate the neighbor join tree
    calculateNeighborJoin(containerStar, &hammingDistance);
    
    // print a summary
    if (PRINT) {
        summarizeStarContainer(containerStar);
    }
    
    // stop the clock and check result
    end = clock();
    printf("%2.4f\n", (double) (end - start) / CLOCKS_PER_SEC);
    }
    
    start = clock();
    
    /*
    // now try the two approximation
    GraphContainer* container = NULL;
    createCliqueContainer(&container, ipt);
    Leaf* someLeaf = NULL;
    twoApproximation(container, &someLeaf);
    
    if (PRINT)
    printTree(someLeaf);
    
    end = clock();
    printf("2-Approximation Running Time: %2.4f\n", (double) (end - start) / CLOCKS_PER_SEC);
    
    // done, destroy all (non-freed) objects
    destroyInput(ipt);
    destroyContainer(container);
    
    // all done
    printf("Done!\n");
    return 0;
     */
}

// generate some random test data
int getTestInput(char* language, int length, int sequences, Input** ipt) {
    
    // error cases
    if (language == NULL) {
        perror("The language is invalid. Cannot generate random sequences.");
        return -1;
    }
    
    // initializers
    int i, j;
    char** sentences = (char**) calloc(sequences, sizeof(char*));
    
    // create and clear input structure
    (*ipt) = (Input*) malloc (sizeof(Input));
    memset(*ipt, 0, sizeof(Input));
    
    // set a random seed
    srand((unsigned) time(NULL));
    
    // create the sentence for each sequence and clear
    for (i = 0; i < sequences; i++) {
        *(sentences + i) = (char*) calloc(length, sizeof(char));
        memset(*(sentences + i), '\0', length * sizeof(char));
        
        // choose a random letter from the language and append
        for (j = 0; j < length; j++) {
            char* someLetter = &language[rand() % strlenAlt(language)];
            strcat(*(sentences + i), someLetter);
        }
        
        // end sequences at the length character (even after strcat)
        *(*(sentences + i) + length) = '\0';
    }
    
    // set up the input structure with the proper information
    (*ipt)->input = sentences;
    (*ipt)->line_alloc = length;
    (*ipt)->lines = sequences;
    
    // done
    return 0;
}

// read a file
int getInput(const char* filename, int line_alloc, Input** ipt)
{
    // attempt to get the file
    FILE* f = fopen(filename, "r");
    
    // file must be valid
    if (f == NULL) {
        perror("The file does not exist or is not open.");
        ipt = NULL;
        return -1;
    }
    
    // allocate input struct
    *ipt = (Input*) malloc(sizeof(Input));
    memset(*ipt, 0, sizeof(Input));
    (*ipt)->line_alloc = line_alloc;
    
    // line numbers
    (*ipt)->lines = 0;
    
    // set 2d array for later allocation
    (*ipt)->input = NULL;
    
    // get input per line
    char line[line_alloc];
    memset(&line, '\0', line_alloc * sizeof(char));
    
    // allocate root
    Link* root = (Link*) malloc(sizeof(Link));
    root->root = 1;
    
    // prev root tracker
    Link* prevroot = root;
    
    // get lines
    // int numTimes = 0;
    while (fgets(line, (int)line_alloc, f) != NULL) {
        
        // create a new link
        Link* current = (Link*) malloc(sizeof(Link));
        current->root = 0;
        
        // allocate space for the string data
        current->data = (char*) calloc(line_alloc, sizeof(char));
        memset(current->data, '\0', line_alloc * sizeof(char));
        
        // set the next link
        prevroot->next = current;
        prevroot = current;
        
        // check for a newline and remove
        char* ext = strchr(line, '\n');
        while (ext != NULL) {
            *ext = '\0';
            ext = strchr(ext + 1, '\n');
        }
        
        // concatenate and clear
        strcat(current->data, line);
        memset(&line, '\0', line_alloc * sizeof(char));
        
        // increment the number of lines
        (*ipt)->lines++;
    }
    
    // create an array for the lines
    (*ipt)->input = (char**) calloc((*ipt)->lines, sizeof(char*));
    
    // iterate over the linked list and add to the input
    Link* iterator = root;
    
    // counter is used to align with the input structure
    int counter = 0;
    while (iterator->next != NULL) {
        
        // error case
        if (counter == (*ipt)->lines) break;
        
        // if root, then go to the next
        if (iterator->root != 1) {
            
            // get the string and append to the input structure
            (*ipt)->input[counter] = iterator->data;
            counter++;
        }
        
        // go to the next node, and keep track
        // of previous structures for cleaning
        Link* prevIterator = iterator;
        iterator = iterator->next;
        free(prevIterator);
    }
    
    // for final items
    (*ipt)->input[counter] = iterator->data;
    free(iterator);
    
    // success!
    fclose(f);
    return 0;
}

// print out the input
void summarizeInput(Input* ipt) {
    
    // error cases
    if (ipt == NULL) return;
    
    // initializers
    char** inputLines = ipt->input;
    int i;
    
    // input summary
    printf("Input Summary: \n");
    
    // print each line
    for(i = 0; i < ipt->lines; i++) {
        printf("Input Line %d: %s\n", i + 1, inputLines[i]);
    }
    
    // formatting
    printf("\n");
}

// destroy input (free)
void destroyInput(Input* ipt) {
    
    // error cases
    if (ipt == NULL) return;
    
    // initializers
    int i;
    
    // destroy all data
    for (i = 0; i < ipt->lines; i++)
        if (ipt->input[i] != NULL)
            free(ipt->input[i]);
    
    // destroy the container
    free(ipt);
    
    // done
    printf("Input container deleted.\n");
}

// get the number of a target character in the input
int numChars(char* input, char target) {
    
    // initializers
    int i, total = 0;
    
    // go through the input and increment the total
    // if the target character is found
    for (i = 0; i < strlenAlt(input); i++)
        if (*(input + i) == target) total++;
    
    // done
    return total;
}

// read file name subroutine
char* readFileName() {
    
    // prompt for entering filename
    printf("Get sequences from file: ");
    
    // create and reset memory
    char filename[MAX_INPUT_SIZE];
    memset(&filename, '\0', MAX_INPUT_SIZE * sizeof(char));
    
    // scan in and remove ending new line character
    scanf("%s", &filename[0]);
    char* repl = strchr(filename, '\n');
    if (repl != NULL) *repl = '\0';
    
    // formatting
    printf("\n");
    
    // create a string to return
    char* path = (char*) calloc(MAX_INPUT_SIZE, sizeof(char));
    strcpy(path, &filename[0]);
    
    // done
    return path;
}

void summarizeStarContainer(StarGraphContainer* container) {
    // error case
    if (container == NULL) return;
    
    // container summary
    printf("Star Container Summary: \n");
    
    // initializers
    int i, j;
    
    // go through all the central edges
    printf("Center Node Ptr: %p\n", container->center);
    
    for (i = 0; i < container->center->edgeCount; i++) {
        printf("\t Edge %d: %p\n", i + 1, container->center->edges[i]);
        printf("\t\t To Node Address: %p\n", container->center->edges[i]->node);
        printf("\t\t Edge Weight: %d\n", container->center->edges[i]->weight);
        // printf("\t\t From Node: %p\n", container->center->edges[i]->from);
        printf("\t\t To Node Number: %d\n", container->center->edges[i]->node->number);
        printf("\t\t To Node Character: %c\n", container->center->edges[i]->node->character);
    }
    
    // go through all of the nodes in the container
    for (i = 0; i < container->nodeCount; i++) {
        
        // print out the node number, address,
        // data, and edge structure addr
        printf("Node #%d:\n", i + 1);
        printf("Address: %p\n", container->nodes[i]);
        printf("Data: %s\n", container->nodes[i]->data);
        // printf("Edge Array Address: %p\n", container->nodes[i]->edges);
        printf("Number of Edges: %d\n", container->nodes[i]->edgeCount);
        // printf("Discovered?: %s\n", container->nodes[i]->discovered ? "true" : "false");
        printf("Node Number: %d\n", container->nodes[i]->number);
        printf("Character: %c\n", container->nodes[i]->character);
        
        // print out each edge in the edge container
        for (j = 0; j < container->nodes[i]->edgeCount; j++) {
            
            // print the edge and node address corresponding along with the weight
            if (container->nodes[i]->edges != NULL) {
                printf("\t Edge %d: %p\n", j + 1, container->nodes[i]->edges[j]);
                printf("\t\t Node Address: %p\n", container->nodes[i]->edges[j]->node);
                printf("\t\t Edge Weight: %d\n", container->nodes[i]->edges[j]->weight);
                // printf("\t\t From Node: %p\n", container->nodes[i]->edges[j]->from);
                // printf("\t\t Discovered?: %s\n",
                // container->nodes[i]->edges[j]->discovered ? "true" : "false");
            }
        }
        
        // formatting
        printf("\n");
    }
}

// summarize all of the nodes in the graph
void summarizeContainer(GraphContainer* container) {
    
    // error case
    if (container == NULL) return;
    
    // container summary
    printf("Container Summary: \n");
    
    // initializers
    int i, j;
    
    // go through all of the nodes in the container
    for (i = 0; i < container->nodeCount; i++) {
        
        // print out the node number, address,
        // data, and edge structure addr
        printf("Node #%d:\n", i + 1);
        printf("Address: %p\n", container->nodes[i]);
        printf("Data: %s\n", container->nodes[i]->data);
        // printf("Edge Array Address: %p\n", container->nodes[i]->edges);
        printf("Number of Edges: %d\n", container->nodes[i]->edgeCount);
        // printf("Discovered?: %s\n", container->nodes[i]->discovered ? "true" : "false");
        
        // print out each edge in the edge container
        for (j = 0; j < container->nodes[i]->edgeCount; j++) {
            
            // print the edge and node address corresponding along with the weight
            if (container->nodes[i]->edges != NULL) {
                printf("\t Edge %d: %p\n", j + 1, container->nodes[i]->edges[j]);
                printf("\t\t Node Address: %p\n", container->nodes[i]->edges[j]->node);
                printf("\t\t Edge Weight: %d\n", container->nodes[i]->edges[j]->weight);
                // printf("\t\t From Node: %p\n", container->nodes[i]->edges[j]->from);
                // printf("\t\t Discovered?: %s\n",
                // container->nodes[i]->edges[j]->discovered ? "true" : "false");
            }
        }
        
        // formatting
        printf("\n");
    }
}

// create an array of nodes and pointers in a star formation
int createStarContainer(StarGraphContainer** container, Input* ipt) {
    
    // error case
    if (ipt == NULL) {
        perror("Could not create node array due to invalid input.");
        return -1;
    }
    
    // node initializer
    int nodeIdCounter = -1;
    int startChar = (int) 'A';
    
    // set up the container and initialize it
    // with one more central node
    (*container) = (StarGraphContainer*) malloc(sizeof(StarGraphContainer));
    (*container)->nodeCount = ipt->lines;
    
    // set up the graph node array (for nodes other than the center)
    GraphNode** nodes = (GraphNode**) calloc((*container)->nodeCount, sizeof(GraphNode*));
    
    // set up the center node
    (*container)->center = (GraphNode*) malloc(sizeof(GraphNode));
    (*container)->center->data = "CENTER";
    (*container)->center->discovered = false;
    (*container)->center->edgeCount = (*container)->nodeCount;
    (*container)->center->edges = (GraphEdge**) calloc((*container)->nodeCount, sizeof(GraphEdge*));
    
    // set up edges in a star formation
    int i;
    for(i = 0; i < (*container)->nodeCount; i++) {
        
        // initialize graph nodes on every input
        nodes[i] = (GraphNode*) malloc(sizeof(GraphNode));
        nodes[i]->data = ipt->input[i];
        nodes[i]->discovered = false;
        nodes[i]->edges = (GraphEdge**) calloc(1, sizeof(GraphEdge*));
        nodes[i]->edgeCount = 0;
        nodes[i]->number = ++nodeIdCounter;
        nodes[i]->character = (char) startChar + i;
    }
    
    // attach pointers from the center to other nodes
    for(i = 0; i < (*container)->nodeCount; i++) {
        (*container)->center->edges[i] = (GraphEdge*) malloc(sizeof(GraphEdge));
        (*container)->center->edges[i]->from = NULL;
        (*container)->center->edges[i]->node = nodes[i];
    }
    
    // containers
    (*container)->nodes = nodes;
    
    // done
    return 0;
}

void calculateNeighborJoin(StarGraphContainer* containerStar,
                           int (*comparator)(const char* s1, const char* s2)) {
    
    // calculating neighbor join
    if (PRINT)
        printf("Calculating the Neighbor Join Algorithm for this Input... \n");
    
    // forward a distance matrix
    DistanceMatrix *matrix = NULL;
    
    // get the center pointer, and leave it the same as
    // always throughout the computation (is 0 at start)
    GraphNode* center = containerStar->center;
    int computationNumber = center->edgeCount;
    int startChar = (int) 'A' + containerStar->nodeCount;
    bool firstRun = true;
    
    while(computationNumber > 2) {
        
        // the first run computes pairwise distances only!!!
        if (firstRun) {
            // create new distance matrix
            matrix = (DistanceMatrix*) malloc (sizeof(DistanceMatrix));
            
            // matrix size is the number of lines
            matrix->numTaxa = center->edgeCount;
            
            // create scoring matrices
            matrix->dist = (int**) calloc(matrix->numTaxa, sizeof(int*));
            matrix->qScoring = (int**) calloc(matrix->numTaxa, sizeof(int*));
            
            // go through the scoring matrix and allocate space for each distance
            int i, j;
            for (i = 0; i < matrix->numTaxa; i++) {
                matrix->dist[i] = (int*) calloc(matrix->numTaxa, sizeof(int));
                matrix->qScoring[i] = (int*) calloc(matrix->numTaxa, sizeof(int));
            }
            
            // compare, and initialize Q matrix
            for (i = 0; i < matrix->numTaxa; i++)
                for (j = 0; j < matrix->numTaxa; j++) {
                    matrix->dist[i][j] = (*comparator)(containerStar->nodes[i]->data, containerStar->nodes[j]->data);
                    matrix->qScoring[i][j] = 0;
                }
        }
        else {
            // do nothing here, since we want to recompute the new matrix after new distances
            // have been calculated at the end
        }
        
        // calculate the Q-matrix
        // component1 = (number of taxa - 2) * the distance from nodes i -> j (the joined nodes)
        // component2 = the sum of the distances between node i (leaf) and every other node
        // component3 = the sum of the distances between node j (leaf) and every other node
        // Q(i, j) = component1 - component2 - component3
        int i, j, k;
        for (i = 0; i < matrix->numTaxa; i++) {
            for (j = 0; j < matrix->numTaxa; j++) {
                
                // component 1 is easy!
                int component1 = (matrix->numTaxa - 2) * matrix->dist[i][j];
                
                // components 2 and 3 are a little bit more tricky
                int component2 = 0, component3 = 0;
                
                // component 2 sum
                for(k = 0; k < matrix->numTaxa; k++) {
                    if (k == i) continue;
                    else component2 += matrix->dist[i][k];
                }
                
                // component 3 sum
                for(k = 0; k < matrix->numTaxa; k++) {
                    if (k == j) continue;
                    else component3 += matrix->dist[j][k];
                }
                
                // final Q result
                matrix->qScoring[i][j] = component1 - component2 - component3;
            }
        }
        
        if (PRINT) {
            // debug print matrices
            printf("Distance, Q-Matrix: \n");
            for (i = 0; i < matrix->numTaxa; i++) {
                for (j = 0; j < matrix->numTaxa; j++) {
                    printf("(%4d, %4d) ", matrix->dist[i][j], matrix->qScoring[i][j]);
                }
                printf("\n");
            }
        }
        
        // least node is the cherry, so we want to save it
        int leastI = -1, leastJ = -1;
        int leastQ = INT_MAX;
        
        // iterate through q and find the minimum score and its index
        for (i = 0; i < matrix->numTaxa; i++)
            for (j = i + 1; j < matrix->numTaxa; j++)
                if (matrix->qScoring[i][j] <= leastQ) {
                    leastI = i;
                    leastJ = j;
                    leastQ = matrix->qScoring[i][j];
                }
        
        // debug print the node indices
        // printf("\nLeastI Node Index: %d\n", leastI);
        // printf("LeastJ Node Index: %d\n", leastJ);
        
        // break both node's linkages with the center, i and j
        // find each new child node in the storage array
        GraphNode* nodeF = NULL;
        GraphNode* nodeG = NULL;
        
        // iterate through all edges and find nodes i and j
        for (i = 0; i < center->edgeCount; i++) {
            if (center->edges[i] != NULL) {
                if (center->edges[i]->node->number == leastI) {
                    nodeF = center->edges[i]->node;
                    free(center->edges[i]);
                    center->edges[i] = NULL;
                }
            }
            if (center->edges[i] != NULL) {
                if (center->edges[i]->node->number == leastJ) {
                    nodeG = center->edges[i]->node;
                    free(center->edges[i]);
                    center->edges[i] = NULL;
                }
            }
        }
        
        // debug print each of the corresponding new nodes
        if (PRINT) {
            printf("\nSplit Node #1 Ptr: %p\n", nodeF);
            printf("Split Node #2 Ptr: %p\n", nodeG);
        }
        
        // create the new splitter node and add to the graph
        // increase the number of nodes
        GraphNode* uNode = (GraphNode*) malloc(sizeof(GraphNode));
        uNode->character = (char) startChar++;
        containerStar->nodes = (GraphNode**) realloc(containerStar->nodes,
                                                     (containerStar->nodeCount + 1) * sizeof(GraphNode*));
        containerStar->nodeCount += 1;
        
        // add the new node to the end of the container
        containerStar->nodes[containerStar->nodeCount - 1] = uNode;
        
        if (PRINT) {
            printf("\nCurrent Node View:\n[");
            for (i = 0; i < containerStar->nodeCount; i++) {
                printf("%p, ", containerStar->nodes[i]);
            }
            printf("]\n");
        }
        
        // give this blasted thing two new edges, and point them to f and g
        uNode->edges = (GraphEdge**) calloc(2, sizeof(GraphEdge*));
        uNode->edgeCount = 2;
        // set node as internal
        uNode->data = "INTERNAL";
        
        // edge 1
        uNode->edges[0] = (GraphEdge*) malloc(sizeof(GraphEdge));
        uNode->edges[0]->from = uNode;
        uNode->edges[0]->node = nodeF;
        nodeF->number = -1;
        
        // edge 2
        uNode->edges[1] = (GraphEdge*) malloc(sizeof(GraphEdge));
        uNode->edges[1]->from = uNode;
        uNode->edges[1]->node = nodeG;
        nodeG->number = -1;
        
        // recopy all non-edge pointers into a new edge array and set to
        // the central node
        int newEdgeCounter = 0;
        GraphEdge** newCentralEdges = (GraphEdge**) calloc(center->edgeCount - 1, sizeof(GraphEdge*));
        for (i = 0; i < center->edgeCount; i++) {
            if (center->edges[i] != NULL) {
                newCentralEdges[newEdgeCounter] = center->edges[i];
                newEdgeCounter++;
            }
        }
        
        // decrease the edgeCount by 1, since we are decreasing an edge
        center->edgeCount -= 1;
        center->edges = newCentralEdges;
        
        // create a new edge and point it to u
        newCentralEdges[newEdgeCounter] = (GraphEdge*) malloc(sizeof(GraphEdge));
        newCentralEdges[newEdgeCounter]->node = uNode;
        
        // renumber all nodes pointed to by the center
        // this is actually super important!!!
        for (i = 0; i < center->edgeCount; i++) {
            center->edges[i]->node->number = i;
        }
        
        if (PRINT) {
            // debug print all central edges
            printf("\nEdges From Center:[\n");
            for (i = 0; i < center->edgeCount; i++) {
                printf("Edge: %p, Node: %p\n", center->edges[i], center->edges[i]->node);
                if (center->edges[i]->node->edges != NULL && center->edges[i]->node->edgeCount > 0) {
                    printf("\t %d Edges Directly Below this Edge.\n", center->edges[i]->node->edgeCount);
                }
            }
            printf("]\n");
        }
        
        // calculate the distance between f and u, and g and u
        // f and u is the harder one to calculate
        int dist_fg = matrix->dist[leastI][leastJ];
        int component1 = 0.5 * dist_fg;
        int component2 = 1 / (2 * (matrix->numTaxa - 2));
        int component3 = 0, component4 = 0;
        
        // get the sum of the distances to node k
        for (int i = 0; i < matrix->numTaxa; i++) {
            for (j = 0; j < matrix->numTaxa; j++) {
                for(k = 0; k < matrix->numTaxa; k++) {
                    if (k == i) continue;
                    else component3 += matrix->dist[i][k];
                }
                
                for(k = 0; k < matrix->numTaxa; k++) {
                    if (k == j) continue;
                    else component4 += matrix->dist[j][k];
                }
            }
        }
        
        // dist between f and u can now be calculated
        int dist_fu = component1 + component2 * (component3 - component4);
        // dist between g and u can now be calculated via reflection
        int dist_gu = dist_fg - dist_fu;
        
        // set these distances up in the internal edges
        for (i = 0; i < uNode->edgeCount; i++) {
            if (uNode->edges[i]->node == nodeF) uNode->edges[i]->weight = dist_gu;
            else if (uNode->edges[i]->node == nodeG) uNode->edges[i]->weight = dist_fu;
        }
        
        if (PRINT) {
            // debug print
            printf("\nDistance from F to new node U is: %d: \n", dist_fu);
            printf("Distance from G to new node U is: %d: \n\n", dist_gu);
        }
        
        // get the distances from node u to any node k
        // is 0.5 * (distance from f to any k + distance from g to
        // any k - distance from f to g)
        // while iterating through nodes, all nodes abstracted away are children nodes
        // so we need to get nodes that haven't been already accounted for
        int validNodeCount = 0;
        for (i = 0; i < containerStar->nodeCount; i++)
            if (containerStar->nodes[i]->number > -1)
                validNodeCount++;
        
        // create a new distance matrix
        DistanceMatrix* tempMatrix = (DistanceMatrix*) malloc(sizeof(DistanceMatrix));
        tempMatrix->dist = (int**) calloc(validNodeCount, sizeof(int*));
        tempMatrix->qScoring = (int**) calloc(validNodeCount, sizeof(int*));
        
        // this includes internal nodes (ie. node u)
        tempMatrix->numTaxa = validNodeCount;
        
        // set up other dimensions
        for (i = 0; i < tempMatrix->numTaxa; i++) {
            tempMatrix->dist[i] = (int*) calloc(validNodeCount, sizeof(int));
            tempMatrix->qScoring[i] = (int*) calloc(validNodeCount, sizeof(int));
        }
        
        // go from the center star to u to get valid distances from u to everything else
        // using the comparator function
        // for every other node use the comparator function standardly
        for (i = 0; i < center->edgeCount; i++) {
            for (j = 0; j < center->edgeCount; j++) {
                if (center->edges[i]->node == uNode) {
                    tempMatrix->dist[i][j] = 0.5 *
                    ((*comparator)(nodeF->data, center->edges[i]->node->data) +
                     (*comparator)(nodeG->data, center->edges[i]->node->data) - dist_fg);
                }
                else if (center->edges[j]->node == uNode) {
                    tempMatrix->dist[i][j] = 0.5 *
                    ((*comparator)(nodeF->data, center->edges[j]->node->data) +
                     (*comparator)(nodeG->data, center->edges[j]->node->data) - dist_fg);
                }
                else {
                    tempMatrix->dist[i][j] = (*comparator)(center->edges[i]->node->data, center->edges[j]->node->data);
                }
            }
        }
        
        // reset the distance matrix
        free(matrix);
        matrix = tempMatrix;
        
        // decrease the number of nodes to be examined
        computationNumber--;
    }
}

// create an array of nodes and pointers in a clique
int createCliqueContainer(GraphContainer** container, Input* ipt) {
    
    // error case
    if (ipt == NULL) {
        perror("Could not create node array due to invalid input.");
        return -1;
    }
    
    // set up the container and initialize it
    *container = (GraphContainer*) malloc(sizeof(GraphContainer));
    (*container)->nodeCount = ipt->lines;
    
    // set up the graph node array
    GraphNode** nodes = (GraphNode**) calloc(ipt->lines, sizeof(GraphNode*));
    
    // go through each line and create a graph node
    int i, j;
    for(i = 0; i < ipt->lines; i++) {
        
        // initialize graph nodes on every input
        nodes[i] = (GraphNode*) malloc(sizeof(GraphNode));
        nodes[i]->data = ipt->input[i];
        nodes[i]->discovered = false;
        // nodes[i]->connected = (GraphEdge**) calloc(ipt->lines - 1, sizeof(GraphEdge*));
        
        // initialize an edge list for each node
        nodes[i]->edges = (GraphEdge**) calloc(ipt->lines - 1, sizeof(GraphEdge*));
        
        // create a bunch of edges
        for (j = 0; j < ipt->lines - 1; j++) {
            
            // make the edge
            nodes[i]->edges[j] = (GraphEdge*) malloc(sizeof(GraphEdge));
            
            // selected for future tree is false
            nodes[i]->edges[j]->discovered = false;
            nodes[i]->edges[j]->back = false;
            
            // list the number of edges in each structure
            nodes[i]->edgeCount = ipt->lines - 1;
            
            // list the 'from' vertex
            nodes[i]->edges[j]->from = nodes[i];
        }
    }
    
    // go through each lines again and reset pointers to every other node
    for(i = 0; i < ipt->lines; i++) {
        
        // counter for other nodes
        int counter = 0;
        for (j = 0; j < ipt->lines - 1; j++) {
            
            // skip same nodes
            if (counter == i) counter++;
            
            // initialize other edge pointers
            // default weight is negative
            nodes[i]->edges[j]->node = nodes[counter];
            nodes[i]->edges[j]->weight = -1;
            
            // go to the next unvisited edge
            counter++;
        }
    }
    
    // set up in the structure
    (*container)->nodes = nodes;
    
    // done
    return 0;
}

// reset container
void resetContainer(GraphContainer* container) {
    
    // for all nodes
    int i, j;
    
    // set discovered as false
    for (i = 0; i < container->nodeCount; i++) {
        container->nodes[i]->discovered = false;
        
        // for all edges, set discovered and back as false
        for (j = 0; j < container->nodes[i]->edgeCount; j++) {
            container->nodes[i]->edges[j]->discovered = false;
            container->nodes[i]->edges[j]->back = false;
        }
    }
}

// destroy container (free)
void destroyContainer(GraphContainer* container) {
    
    // error cases
    if (container == NULL) return;
    
    // intializers
    int i, j;
    
    // go through all nodes
    for (i = 0; i < container->nodeCount; i++) {
        
        // go through all edges
        for (j = 0; j < container->nodes[i]->edgeCount; j++) {
            free(container->nodes[i]->edges[j]);
        }
        
        // free the edge struct
        if (container->nodes[i]->edges != NULL)
            free(container->nodes[i]->edges);
        
        // and then the node struct in the node array
        free(container->nodes[i]);
    }
    
    // free the node array
    free(container->nodes);
    
    // free the container
    free(container);
    
    // done
    printf("Graph container deleted.\n");
}

bool invalidEdgeWeights(GraphContainer* container) {
    
    // initializers
    int i, j;
    
    // for all nodes
    for (i = 0; i < container->nodeCount; i++)
        // for all edges at that node
        for (j = 0; j < container->nodes[i]->edgeCount; j++)
            // check if weight is negative (initializer condition)
            if (container->nodes[i]->edges[j]->weight < 0)
                // if so, error out
                return true;
    // no issues
    return false;
}

bool allNodesDiscovered(GraphContainer* container) {
    
    // initializers
    int i;
    
    // for all nodes
    for(i = 0; i < container->nodeCount; i++)
        
        // if any node is not discovered
        if (container->nodes[i]->discovered == false)
            return false;
    
    // all nodes are visited
    return true;
}

// calculate the container scores using
void calculateContainerScores(GraphContainer* container,
                              int (*comparator)(const char* s1, const char* s2)) {
    // error cases
    if (container == NULL || comparator == NULL) return;
    
    // initializers, get all the nodes and edges, and
    // then iterate through them
    int i, j;
    for (i = 0; i < container->nodeCount; i++) {
        
        // for each node, get the current node, and all of
        // its edges along with its data
        GraphNode* currentNode = container->nodes[i];
        const char* nodeData = currentNode->data;
        
        // for each edge
        for (j = 0; j < currentNode->edgeCount; j++) {
            
            // get the edge and the next node + its data
            GraphEdge* currentEdge = currentNode->edges[j];
            GraphNode* nextNode = currentEdge->node;
            const char* nextNodeData = nextNode->data;
            
            // compare using the score comparator
            int score = (*comparator)(nodeData, nextNodeData);
            
            // set the weight of that edge based on the score
            currentEdge->weight = score;
        }
    }
}

// minimum spanning tree (not working)
void minimumSpanningTree(GraphContainer* container, GraphNode** rootNode) {
    
    // initialize container with all edges not discovered
    int i, j, k;
    for (i = 0; i < container->nodeCount; i++)
        container->nodes[i]->discovered = false;
    
    // randomly choose a starting node
    srand((unsigned int) time(NULL));
    
    // and set as the root node
    *rootNode = container->nodes[rand() % container->nodeCount];
    
    // mark the first node as visible
    (*rootNode)->discovered = true;
    
    // until all nodes are visited
    while (!allNodesDiscovered(container)) {
        
        // minimum weight initializers
        int minimumWeight = INT_MAX;
        GraphEdge* minimumEdge = NULL;
        
        // find a minimum edge to another
        // node from all previously discovered nodes
        for (i = 0; i < container->nodeCount; i++)
            // if a node is discovered (in the set)
            if (container->nodes[i]->discovered == true)
                // loop through all the edges of that node (in essence all discovered nodes)
                for (j = 0; j < container->nodes[i]->edgeCount; j++)
                    // if the current edge's weight is less than the minimum weight
                    // and the node the edge connects to is not discovered
                    if (container->nodes[i]->edges[j]->weight < minimumWeight &&
                        container->nodes[i]->edges[j]->node->discovered != true) {
                        minimumEdge = container->nodes[i]->edges[j];
                        minimumWeight = container->nodes[i]->edges[j]->weight;
                    }
        
        // if no other minimum edge, this is a strange error
        if (minimumEdge == NULL) break;
        
        // otherwise, set the node from that edge
        // outgoing to discovered (so its added to the discovered nodes)
        else {
            
            // mark the minimum node
            GraphNode* minimumNode = minimumEdge->node;
            minimumNode->discovered = true;
            
            // mark the minimum edge
            minimumEdge->discovered = true;
            
            // find the minimum edge going back to the last node (if any)
            // and mark as discovered
            for (k = 0; k < minimumNode->edgeCount; k++)
                if (minimumNode->edges[k]->node == minimumEdge->from) {
                    // minimumNode->edges[k]->discovered = true;
                }
        }
    }
}

// depth first graph
void depthFirstGraph(GraphNode* startNext) {
    
    // error cases
    if (startNext == NULL) return;
    
    // initializers
    int i;
    
    // label node as discovered
    startNext->discovered = true;
    
    // check all undiscovered nodes
    for (i = 0; i < startNext->edgeCount; i++) {
        if (startNext->edges[i]->discovered == false) {
            
            // get the potential edges
            GraphNode* potential = startNext->edges[i]->node;
            if (potential->discovered == false) {
                
                // if discovered, DFS it!
                startNext->edges[i]->discovered = true;
                depthFirstGraph(potential);
            }
            else {
                // else, is a back edge
                startNext->edges[i]->back = true;
            }
        }
    }
}

// two approximation edge selector for minimum spanning tree
void twoApproximation(GraphContainer* container, Leaf** treeRoot) {
    
    // select the relevant nodes to the tree building algorithm
    printf("Node selector for 2-approximation.\n");
    
    // reset container internals
    resetContainer(container);
    
    // find the minimum spanning tree
    printf("Finding the minimum spanning tree.\n");
    GraphNode* root = NULL;
    minimumSpanningTree(container, &root);
    
    // print the root node pointer
    // printf("Root node pointer: %p\n\n", root);
    
    // return a bad root, if the graph root is null
    if (root == NULL) {
        (*treeRoot) = NULL;
        return;
    }
    
    // get a leaf and set the root
    (*treeRoot) = (Leaf*) malloc(sizeof(Leaf));
    (*treeRoot)->root = 1;
    (*treeRoot)->data = root->data;
    
    // build the tree
    buildTree(root, treeRoot);
}

// create a binary tree from a graph (something wrong here)
void buildTree(GraphNode* nextNode, Leaf** nextLeaf) {
    
    // start at the root node and traverse
    // assume binary tree
    int i, nodeCounter = 0, initialSize = 2;
    
    // get a list of the incident edges
    GraphNode** incidentNode = (GraphNode**) calloc(initialSize, sizeof(GraphNode*));
    GraphEdge** incidentEdge = (GraphEdge**) calloc(initialSize, sizeof(GraphEdge*));
    
    // initialize and reset
    memset(incidentNode, 0, initialSize * sizeof(GraphNode*));
    memset(incidentEdge, 0, initialSize * sizeof(GraphEdge*));
    
    // get all connected edges that are selected
    for (i = 0; i < nextNode->edgeCount; i++) {
        
        // if an edge is discovered and not a back edge
        if (nextNode->edges[i]->discovered == true) {
            
            // if we need more room for more nodes
            if (nodeCounter + 1 >= initialSize) {
                // increase the size of the node array
                incidentNode = (GraphNode**) realloc(incidentNode, ++initialSize * sizeof(GraphNode*));
                incidentEdge = (GraphEdge**) realloc(incidentEdge, initialSize * sizeof(GraphEdge*));
            }
            // setting the node number to another node
            incidentNode[nodeCounter] = nextNode->edges[i]->node;
            incidentEdge[nodeCounter] = nextNode->edges[i];
            nodeCounter++;
        }
    }
    
    // print the number of nodes (debug)
    // printf("Number of Nodes: %d\n", nodeCounter);
    
    // create new leaves for each node discovered if
    if (nodeCounter > 0) {
        
        // allocate leaves
        (*nextLeaf)->leaves = (Leaf**) calloc(nodeCounter, sizeof(Leaf*));
        (*nextLeaf)->weights = (int*) calloc(nodeCounter, sizeof(int));
        (*nextLeaf)->numLeaves = nodeCounter;
        
        // for all items (nodeCounter is the number of nodes)
        for (i = 0; i < nodeCounter; i++) {
            
            // create a child node
            Leaf* child = (Leaf*) malloc(sizeof(Leaf));
            child->data = incidentNode[i]->data;
            child->root = 0;
            
            // append the child to the root
            (*nextLeaf)->leaves[i] = child;
            (*nextLeaf)->weights[i] = incidentEdge[i]->weight;
            
            // go down to the next level
            buildTree(incidentNode[i], &child);
        }
    } else {
        
        // set a default size branch, so it doesn't crap out on dereference
        (*nextLeaf)->leaves = (Leaf**) calloc(1, sizeof(Leaf*));
        (*nextLeaf)->weights = (int*) calloc(1, sizeof(int));
        (*nextLeaf)->numLeaves = nodeCounter;
        (*nextLeaf)->leaves[0] = (Leaf*) malloc(sizeof(Leaf));
    }
    
    // done
    free(incidentNode);
    free(incidentEdge);
    return;
}

// print wrapper
void printTree(Leaf* root) {
    printf("\nPhylogenic Tree: \n");
    printTreeInternal(root, 0);
}

// print binary tree
void printTreeInternal(Leaf* next, int indentLevel) {
    
    // error case
    if (next == NULL)
        return;
    else
    {
        // go to each other
        int i;
        for (i = 0; i < next->numLeaves; i++) {
            if (next->leaves != NULL)
                printTreeInternal(next->leaves[i], indentLevel++);
        }
        
        // print
        for (i = 0; i < indentLevel; i++)
            printf("\t\t");
        printf("Node Data: %s\n", next->data);
    }
}

// validate that the language of the input is correct
int validateLanguage(const char* language, const char* comparator) {
    
    // initializers
    int i, j, compared = 0;
    
    // compare language to each character
    for (i = 0; i < strlenAlt(comparator); i++) {
        for (j = 0; j < strlenAlt(language); j++) {
            
            // comparator is in language, set true else false
            if (comparator[i] == language[j])
                compared &= 0;
            else compared &= 1;
        }
    }
    
    // done
    return compared;
}

// find the hamming distance of two strings
int hammingDistance(const char* first, const char* second) {
    
    // initializers
    int distance = 0, i;
    
    // variability cases
    if (strlenAlt(first) != strlenAlt(second)) {
        if (strlenAlt(first) > strlenAlt(second))
            distance += (strlenAlt(first) - strlenAlt(second));
        else if (strlenAlt(second) > strlenAlt(first))
            distance += (strlenAlt(second) - strlenAlt(first));
    }
    
    // loop through and calculate the rest of the distances
    for (i = 0; i < strlenAlt(first) && i < strlenAlt(second); i++) {
        if (first[i] != second[i])
            distance += 1;
    }
    
    // done
    return distance;
}

// compute the strlenAlt (w/o the C library)
int strlenAlt(const char* string) {
    
    // initializers
    int length = 0;
    
    // loop through the string until the
    // null character is found
    while (string[length] != '\0')
        length++;
    
    // done
    return length;
}



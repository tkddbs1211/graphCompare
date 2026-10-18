#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_VERTICES 100

typedef struct AdjNode {
    int vertex;
    struct AdjNode* next;
} AdjNode;

typedef struct {
    int v_count;
    int e_count;
    AdjNode* adjList[MAX_VERTICES];
} AdjListGraph;

typedef struct {
    int v_count;
    int e_count;
    int matrix[MAX_VERTICES][MAX_VERTICES];
} AdjMatrixGraph;

typedef struct {
    int u;
    int v;
} Edge;

void initMatrixGraph(AdjMatrixGraph* g) {
    g->v_count = MAX_VERTICES;
    g->e_count = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            g->matrix[i][j] = 0;
        }
    }
}

void initListGraph(AdjListGraph* g) {
    g->v_count = MAX_VERTICES;
    g->e_count = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        g->adjList[i] = NULL;
    }
}

void generateGraphEdges(Edge* edges, int target_edges) {
    bool temp_matrix[MAX_VERTICES][MAX_VERTICES] = { false };
    int count = 0;

    while (count < target_edges) {
        int u = rand() % MAX_VERTICES;
        int v = rand() % MAX_VERTICES;

        if (u == v) continue; 
        if (u > v) { int temp = u; u = v; v = temp; } 

        if (!temp_matrix[u][v]) {
            temp_matrix[u][v] = true;
            edges[count].u = u;
            edges[count].v = v;
            count++;
        }
    }
}

void addEdgeMatrix(AdjMatrixGraph* g, int u, int v) {
    g->matrix[u][v] = 1;
    g->matrix[v][u] = 1;
    g->e_count++;
}

void addEdgeList(AdjListGraph* g, int u, int v) {
    AdjNode* node1 = (AdjNode*)malloc(sizeof(AdjNode));
    node1->vertex = v;
    node1->next = g->adjList[u];
    g->adjList[u] = node1;

    AdjNode* node2 = (AdjNode*)malloc(sizeof(AdjNode));
    node2->vertex = u;
    node2->next = g->adjList[v];
    g->adjList[v] = node2;

    g->e_count++;
}

void freeListGraph(AdjListGraph* g) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        AdjNode* curr = g->adjList[i];
        while (curr != NULL) {
            AdjNode* temp = curr;
            curr = curr->next;
            free(temp);
        }
        g->adjList[i] = NULL;
    }
}

size_t getMatrixMemory(AdjMatrixGraph* g) {
    size_t struct_size = sizeof(AdjMatrixGraph);
    return sizeof(g->matrix) + sizeof(int) * 2;
}

size_t getListMemory(AdjListGraph* g) {
    size_t total = sizeof(AdjListGraph);
    for (int i = 0; i < MAX_VERTICES; i++) {
        AdjNode* curr = g->adjList[i];
        while (curr != NULL) {
            total += sizeof(AdjNode);
            curr = curr->next;
        }
    }
    return total;
}

long long measureMatrixInsertion(AdjMatrixGraph* g, int u, int v) {
    return 1;
}

long long measureListInsertion(AdjListGraph* g, int u, int v) {
    long long comparisons = 0;
    AdjNode* curr = g->adjList[u];
    while (curr != NULL) {
        comparisons++;
        curr = curr->next;
    }
    comparisons++; 
    return comparisons;
}

long long measureMatrixDeletion(AdjMatrixGraph* g, int u, int v) {
    return 1;
}

long long measureListDeletion(AdjListGraph* g, int u, int v) {
    long long comparisons = 0;
    AdjNode* curr = g->adjList[u];
    while (curr != NULL) {
        comparisons++;
        if (curr->vertex == v) break;
        curr = curr->next;
    }
    return comparisons;
}

long long measureMatrixConnectivity(AdjMatrixGraph* g, int u, int v) {
    return 1;
}

long long measureListConnectivity(AdjListGraph* g, int u, int v) {
    long long comparisons = 0;
    AdjNode* curr = g->adjList[u];
    while (curr != NULL) {
        comparisons++;
        if (curr->vertex == v) break;
        curr = curr->next;
    }
    return comparisons;
}

long long measureMatrixAdjacentVertices(AdjMatrixGraph* g, int u) {
    long long comparisons = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        comparisons++;
        volatile int val = g->matrix[u][i];
    }
    return comparisons;
}

long long measureListAdjacentVertices(AdjListGraph* g, int u) {
    long long comparisons = 0;
    AdjNode* curr = g->adjList[u];
    while (curr != NULL) {
        comparisons++;
        curr = curr->next;
    }
    comparisons++; 
    return comparisons;
}

void runSimulation(int target_edges, const char* case_name) {
    srand(2026); 

    Edge* edges = (Edge*)malloc(sizeof(Edge) * target_edges);
    generateGraphEdges(edges, target_edges);

    AdjMatrixGraph matGraph;
    initMatrixGraph(&matGraph);
    for (int i = 0; i < target_edges; i++) {
        addEdgeMatrix(&matGraph, edges[i].u, edges[i].v);
    }

    AdjListGraph listGraph;
    initListGraph(&listGraph);
    for (int i = 0; i < target_edges; i++) {
        addEdgeList(&listGraph, edges[i].u, edges[i].v);
    }

    int test_u = 10;
    int test_v = 20;
    int ins_u = 0, ins_v = 99;
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (matGraph.matrix[0][i] == 0 && i != 0) {
            ins_v = i;
            break;
        }
    }

    size_t mem_mat = getMatrixMemory(&matGraph);
    size_t mem_list = getListMemory(&listGraph);

    printf("Case : %s - Adjacency Matrix\n", case_name);
    printf("Memory                         : %zu Bytes\n", mem_mat);
    printf("Edge insertion comparisons     : %lld\n", measureMatrixInsertion(&matGraph, ins_u, ins_v));
    printf("Edge deletion comparisons      : %lld\n", measureMatrixDeletion(&matGraph, edges[0].u, edges[0].v));
    printf("Connectivity check comparisons : %lld\n", measureMatrixConnectivity(&matGraph, test_u, test_v));
    printf("Adjacent vertices comparisons  : %lld\n\n", measureMatrixAdjacentVertices(&matGraph, test_u));

    printf("Case : %s - Adjacency List\n", case_name);
    printf("Memory                         : %zu Bytes\n", mem_list);
    printf("Edge insertion comparisons     : %lld\n", measureListInsertion(&listGraph, ins_u, ins_v));
    printf("Edge deletion comparisons      : %lld\n", measureListDeletion(&listGraph, edges[0].u, edges[0].v));
    printf("Connectivity check comparisons : %lld\n", measureListConnectivity(&listGraph, test_u, test_v));
    printf("Adjacent vertices comparisons  : %lld\n\n", measureListAdjacentVertices(&listGraph, test_u));

    free(edges);
    freeListGraph(&listGraph);
}

int main(void) {
    printf("========================================\n");
    printf("  GRAPH PERFORMANCE COMPARISON PROGRAM  \n");
    printf("========================================\n\n");

    runSimulation(100, "Sparse Graph");

    runSimulation(4000, "Dense Graph");

    return 0;
}
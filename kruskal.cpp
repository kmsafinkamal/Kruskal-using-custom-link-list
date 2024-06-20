#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

struct Edge {
    int src, dest, weight;
};

struct Graph {
    int V, E;
    vector<Edge*> edges;  // Stores pointers to Edge objects
    vector<list<int>> adjList;  
};


Graph* createGraph(int V, int E) {
    Graph* graph = new Graph;
    graph->V = V;
    graph->E = E;
    graph->edges.resize(E);
    graph->adjList.resize(V);
    return graph;
}


Edge* createEdge(int src, int dest, int weight) {
    Edge* edge = new Edge;  
    edge->src = src;
    edge->dest = dest;
    edge->weight = weight;
    return edge;
}

// Min-Heap implementation
class MinHeap {
public:
    void push(Edge* edge) {
        heap.push_back(edge);
        siftUp(heap.size() - 1);
    }

    void pop() {
        swap(heap[0], heap[heap.size() - 1]);
        heap.pop_back();
        siftDown(0);
    }

    Edge* top() {
        return heap[0];
    }

    bool empty() {
        return heap.empty();
    }

private:
    vector<Edge*> heap;

    void siftUp(int idx) {
        while (idx > 0 && heap[parent(idx)]->weight > heap[idx]->weight) {
            swap(heap[parent(idx)], heap[idx]);
            idx = parent(idx);
        }
    }

    void siftDown(int idx) {
        int minIndex = idx;
        int leftChild = left(idx);
        if (leftChild < heap.size() && heap[leftChild]->weight < heap[minIndex]->weight) {
            minIndex = leftChild;
        }
        int rightChild = right(idx);
        if (rightChild < heap.size() && heap[rightChild]->weight < heap[minIndex]->weight) {
            minIndex = rightChild;
        }
        if (idx != minIndex) {
            swap(heap[idx], heap[minIndex]);
            siftDown(minIndex);
        }
    }

    int parent(int idx) {
        return (idx - 1) / 2;
    }

    int left(int idx) {
        return 2 * idx + 1;
    }

    int right(int idx) {
        return 2 * idx + 2;
    }
};


class CustomAdjList {
public:
    CustomAdjList(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    int findComponet(int u) {
        if (parent[u] != u) {
            parent[u] = findComponet(parent[u]);
        }
        return parent[u];
    }

    void linkVertex(int u, int v) {
        int rootU = findComponet(u);
        int rootV = findComponet(v);
        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }

private:
    vector<int> parent, rank;
};

// Kruskal's algorithm
void KruskalMST(Graph* graph) {
    int V = graph->V;
    vector<Edge> result; 

    MinHeap minHeap;
    for (Edge* edge : graph->edges) {
        minHeap.push(edge);
    }

    CustomAdjList uf(V);  //to detect cycles


    while (result.size() < V - 1 && !minHeap.empty()) {
        Edge* edge = minHeap.top();
        minHeap.pop();

        int x = uf.findComponet(edge->src);
        int y = uf.findComponet(edge->dest);


        if (x != y) {
            result.push_back(*edge);
            uf.linkVertex(x, y);
        }
    }


    cout << "Following are the edges in the constructed MST\n";
    for (const auto& edge : result) {
        cout << edge.src << " -- " << edge.dest << "   " << edge.weight << endl;
    }
}

int main() {
    int V = 7;  // Number of vertices
    int E = 12;  // Number of edges

    // Array of edges for graph representation
    Edge edges[] = {
        //{0, 2, 10}, {0, 1, 2}, {1, 2, 3}
        //{0, 1, 10}, {0, 2, 6}, {0, 3, 5}, {1, 3, 15}, {2, 3, 4}
        {0, 1, 5}, {1, 2, 7}, {2, 6, 4}, {6, 5, 2}, {4, 5, 8},{0, 4, 2}, {0, 3, 3}, {1, 3, 6}, {2, 3, 4}, {3, 6, 8}, {3, 5, 7}, {3, 4, 7}
    };

    Graph* graph = createGraph(V, E);
 
    for (int i = 0; i < E; ++i) {
        graph->edges[i] = createEdge(edges[i].src, edges[i].dest, edges[i].weight);
    }

    KruskalMST(graph);

    return 0;
}



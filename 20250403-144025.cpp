#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_map>

using namespace std;

typedef pair<int, string> pii; // Pair to store (weight, vertex)

class Graph {
private:
    unordered_map<string, int> vertexMap; // Map to manage vertex names and their indices
    vector<vector<pii>> adj; // Adjacency list
    int V; // Number of vertices

public:
    Graph() : V(0) {}

    void addVertex(const string& vertex) {
        if (vertexMap.find(vertex) == vertexMap.end()) {
            vertexMap[vertex] = V++;
            adj.resize(V);
        }
    }

    void addEdge(const string& u, const string& v, int w) {
        addVertex(u);
        addVertex(v);
        int uIndex = vertexMap[u];
        int vIndex = vertexMap[v];
        adj[uIndex].emplace_back(w, v);
        adj[vIndex].emplace_back(w, u); // For undirected graph
    }

    void deleteEdge(const string& u, const string& v) {
        if (vertexMap.find(u) != vertexMap.end() && vertexMap.find(v) != vertexMap.end()) {
            int uIndex = vertexMap[u];
            int vIndex = vertexMap[v];
            adj[uIndex].erase(remove_if(adj[uIndex].begin(), adj[uIndex].end(),
                [v](const pii& edge) { return edge.second == v; }), adj[uIndex].end());
            adj[vIndex].erase(remove_if(adj[vIndex].begin(), adj[vIndex].end(),
                [u](const pii& edge) { return edge.second == u; }), adj[vIndex].end());
        }
    }

    void updateEdge(const string& u, const string& v, int w) {
        deleteEdge(u, v);
        addEdge(u, v, w);
    }

    void deleteNode(const string& vertex) {
        if (vertexMap.find(vertex) != vertexMap.end()) {
            int index = vertexMap[vertex];
            adj.erase(adj.begin() + index);
            vertexMap.erase(vertex);

            // Remove edges to this vertex from other vertices
            for (auto& edges : adj) {
                edges.erase(remove_if(edges.begin(), edges.end(),
                    [index, this](const pii& edge) { return vertexMap[edge.second] == index; }), edges.end());
            }

            // Update vertexMap for remaining vertices
            unordered_map<string, int> newMap;
            int newIndex = 0;
            for (const auto& pair : vertexMap) {
                newMap[pair.first] = newIndex++;
            }
            vertexMap = newMap;
        }
    }

    void viewGraph() {
        for (const auto& pair : vertexMap) {
            string vertex = pair.first;
            int u = pair.second;
            cout << "Vertex " << vertex << ": ";
            for (auto& edge : adj[u]) {
                cout << " -> (Weight: " << edge.first << ", Vertex: " << edge.second << ")";
            }
            cout << endl;
        }
    }

    void viewNodes() {
        cout << "Current Nodes in the Graph:\n";
        for (const auto& pair : vertexMap) {
            cout << pair.first << endl;
        }
    }

    vector<int> dijkstra(const string& src) {
        if (vertexMap.find(src) == vertexMap.end()) return {};

        vector<int> dist(V, numeric_limits<int>::max());
        vector<int> parent(V, -1); // For path reconstruction
        priority_queue<pii, vector<pii>, greater<pii>> pq;

        int srcIndex = vertexMap[src];
        dist[srcIndex] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            int uIndex = vertexMap[pq.top().second];
            pq.pop();

            for (auto& edge : adj[uIndex]) {
                int weight = edge.first;
                string v = edge.second;
                int vIndex = vertexMap[v];

                if (dist[uIndex] + weight < dist[vIndex]) {
                    dist[vIndex] = dist[uIndex] + weight;
                    parent[vIndex] = uIndex; // Keep track of parent for path reconstruction
                    pq.push({dist[vIndex], v});
                }
            }
        }

        // Print shortest paths
        for (const auto& pair : vertexMap) {
            string vertex = pair.first;
            int i = pair.second;
            if (dist[i] < numeric_limits<int>::max()) {
                cout << "Shortest path to vertex " << vertex << " is " << dist[i] << " via ";
                printPath(parent, i);
                cout << endl;
            } else {
                cout << "Vertex " << vertex << " is unreachable." << endl;
            }
        }

        return dist;
    }

    void printPath(vector<int>& parent, int j) {
        if (parent[j] == -1) {
            cout << j;
            return;
        }
        printPath(parent, parent[j]);
        cout << " -> " << j;
    }

    void bfs(const string& start) {
        if (vertexMap.find(start) == vertexMap.end()) return;

        vector<bool> visited(V, false);
        queue<int> q;
        int startIndex = vertexMap[start];
        q.push(startIndex);
        visited[startIndex] = true;

        cout << "BFS Traversal: ";
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cout << u << " ";

            for (auto& edge : adj[u]) {
                int vIndex = vertexMap[edge.second];
                if (!visited[vIndex]) {
                    visited[vIndex] = true;
                    q.push(vIndex);
                }
            }
        }
        cout << endl;
    }

    void dfsUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";

        for (auto& edge : adj[v]) {
            int vIndex = vertexMap[edge.second];
            if (!visited[vIndex]) {
                dfsUtil(vIndex, visited);
            }
        }
    }

    void dfs(const string& start) {
        if (vertexMap.find(start) == vertexMap.end()) return;

        vector<bool> visited(V, false);
        int startIndex = vertexMap[start];
        cout << "DFS Traversal: ";
        dfsUtil(startIndex, visited);
        cout << endl;
    }

    int longestPath(const string& start) {
        if (vertexMap.find(start) == vertexMap.end()) return 0;

        vector<bool> visited(V, false);
        int maxLength = 0;
        longestPathUtil(vertexMap[start], visited, 0, maxLength);
        return maxLength;
    }

    void longestPathUtil(int u, vector<bool>& visited, int pathLength, int& maxLength) {
        visited[u] = true;
        maxLength = max(maxLength, pathLength);

        for (auto& edge : adj[u]) {
            int vIndex = vertexMap[edge.second];
            if (!visited[vIndex]) {
                longestPathUtil(vIndex, visited, pathLength + edge.first, maxLength);
            }
        }
        visited[u] = false; // Backtrack
    }
};

int main() {
    Graph g;
    int choice;
    string u, v;
    int w;

    // Example edges representing locations in Gondar
    g.addEdge("Fasil Castle", "Debre Birhan Selassie Church", 10);
    g.addEdge("Fasil Castle", "Gondar Castle", 5);
    g.addEdge("Debre Birhan Selassie Church", "Gondar Castle", 2);
    g.addEdge("Gondar Castle", "Bahir Dar Road", 1);
    g.addEdge("Bahir Dar Road", "Lake Tana", 3);
    g.addEdge("Lake Tana", "Simien Mountains", 9);
    g.addEdge("Lake Tana", "Gondar Castle", 2);
    g.addEdge("Simien Mountains", "Fasil Castle", 4);
    
    do {
        cout << "\nMenu:\n";
        cout << "1. Add Edge\n";
        cout << "2. Delete Edge\n";
        cout << "3. Update Edge\n";
        cout << "4. Add Node\n";
        cout << "5. Delete Node\n";
        cout << "6. View Nodes\n";
        cout << "7. View Graph\n";
        cout << "8. Perform BFS\n";
        cout << "9. Perform DFS\n";
        cout << "10. Shortest Path (Dijkstra)\n";
        cout << "11. Longest Path\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter edge (u v w): ";
                cin >> u >> v >> w; // Add edge u-v with weight w
                g.addEdge(u, v, w);
                break;
            case 2:
                cout << "Enter edge to delete (u v): ";
                cin >> u >> v; // Delete edge u-v
                g.deleteEdge(u, v);
                break;
            case 3:
                cout << "Enter edge to update (u v w): ";
                cin >> u >> v >> w; // Update edge u-v with weight w
                g.updateEdge(u, v, w);
                break;
            case 4:
                cout << "Enter node to add: ";
                cin >> u; // Add node u
                g.addVertex(u);
                break;
            case 5:
                cout << "Enter node to delete: ";
                cin >> u; // Delete node u
                g.deleteNode(u);
                break;
            case 6:
                g.viewNodes(); // View all nodes
                break;
            case 7:
                g.viewGraph(); // View graph structure
                break;
            case 8:
                cout << "Enter start vertex for BFS: ";
                cin >> u;
                g.bfs(u);
                break;
            case 9:
                cout << "Enter start vertex for DFS: ";
                cin >> u;
                g.dfs(u);
                break;
            case 10:
                cout << "Enter source vertex for Dijkstra: ";
                cin >> u;
                g.dijkstra(u);
                break;
            case 11:
                cout << "Enter start vertex for Longest Path: ";
                cin >> u;
                cout << "Longest Path Length: " << g.longestPath(u) << endl;
                break;
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}

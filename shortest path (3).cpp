#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <limits>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Graph {
private:
    unordered_map<int, list<pair<int, int>>> adj; // Adjacency list: node -> (neighbor, weight)

public:
    // Add a node
    void addNode(int node) {
        if (adj.find(node) == adj.end()) {
            adj[node]; // Create an entry if it doesn't exist
            cout << "Node " << node << " added.\n";
        } else {
            cout << "Node " << node << " already exists.\n";
        }
    }

    // Add an edge
    void addEdge(int u, int v, int weight) {
        if (adj.find(u) != adj.end() && adj.find(v) != adj.end()) {
            adj[u].emplace_back(v, weight);
            adj[v].emplace_back(u, weight); // For undirected graph
            cout << "Edge added between " << u << " and " << v << " with weight " << weight << ".\n";
        } else {
            cout << "One or both nodes do not exist.\n";
        }
    }

    // Update an edge
    void updateEdge(int u, int v, int newWeight) {
        if (adj.find(u) != adj.end() && adj.find(v) != adj.end()) {
            for (auto& edge : adj[u]) {
                if (edge.first == v) {
                    edge.second = newWeight;
                    cout << "Edge updated between " << u << " and " << v << " to weight " << newWeight << ".\n";
                    return;
                }
            }
            cout << "Edge not found.\n";
        } else {
            cout << "One or both nodes do not exist.\n";
        }
    }

    // Delete an edge
    void deleteEdge(int u, int v) {
        if (adj.find(u) != adj.end() && adj.find(v) != adj.end()) {
            adj[u].remove_if([v](const pair<int, int>& edge) { return edge.first == v; });
            adj[v].remove_if([u](const pair<int, int>& edge) { return edge.first == u; });
            cout << "Edge deleted between " << u << " and " << v << ".\n";
        } else {
            cout << "One or both nodes do not exist.\n";
        }
    }

    // Delete a node
    void deleteNode(int node) {
        if (adj.find(node) != adj.end()) {
            adj.erase(node);
            for (auto& [key, value] : adj) {
                value.remove_if([node](const pair<int, int>& edge) { return edge.first == node; });
            }
            cout << "Node " << node << " deleted.\n";
        } else {
            cout << "Node " << node << " does not exist.\n";
        }
    }

    // Display the graph
    void display() {
        cout << "Graph:\n";
        for (const auto& [node, edges] : adj) {
            cout << node << " -> ";
            for (const auto& edge : edges) {
                cout << "(" << edge.first << ", " << edge.second << ") ";
            }
            cout << endl;
        }
    }

    // BFS traversal
    void BFS(int start) {
        if (adj.find(start) == adj.end()) {
            cout << "Start node does not exist.\n";
            return;
        }

        unordered_map<int, bool> visited;
        queue<int> q;
        q.push(start);
        visited[start] = true;

        cout << "BFS Traversal: ";
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            cout << node << " ";

            for (const auto& edge : adj[node]) {
                if (!visited[edge.first]) {
                    q.push(edge.first);
                    visited[edge.first] = true;
                }
            }
        }
        cout << endl;
    }

    // DFS traversal
    void DFSUtil(int node, unordered_map<int, bool>& visited) {
        visited[node] = true;
        cout << node << " ";

        for (const auto& edge : adj[node]) {
            if (!visited[edge.first]) {
                DFSUtil(edge.first, visited);
            }
        }
    }

    void DFS(int start) {
        if (adj.find(start) == adj.end()) {
            cout << "Start node does not exist.\n";
            return;
        }

        unordered_map<int, bool> visited;
        cout << "DFS Traversal: ";
        DFSUtil(start, visited);
        cout << endl;
    }

    // Dijkstra's algorithm for shortest path
    void dijkstra(int start) {
        if (adj.find(start) == adj.end()) {
            cout << "Start node does not exist.\n";
            return;
        }

        unordered_map<int, int> dist;
        for (const auto& [node, _] : adj) {
            dist[node] = numeric_limits<int>::max(); // Initialize distances
        }
        dist[start] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.emplace(0, start); // (distance, node)

        while (!pq.empty()) {
            int d = pq.top().first;
            int node = pq.top().second;
            pq.pop();

            if (d > dist[node]) continue; // Skip if we've found a better path

            for (const auto& edge : adj[node]) {
                int neighbor = edge.first;
                int weight = edge.second;

                if (dist[node] + weight < dist[neighbor]) {
                    dist[neighbor] = dist[node] + weight;
                    pq.emplace(dist[neighbor], neighbor);
                }
            }
        }

        cout << "Shortest paths from node " << start << ":\n";
        for (const auto& [node, distance] : dist) {
            cout << "To " << node << ": " << (distance == numeric_limits<int>::max() ? "Infinity" : to_string(distance)) << endl;
        }
    }

    // Longest path (using DFS)
    void longestPathUtil(int node, unordered_map<int, bool>& visited, int length, int& maxLength) {
        visited[node] = true;
        maxLength = max(maxLength, length);

        for (const auto& edge : adj[node]) {
            if (!visited[edge.first]) {
                longestPathUtil(edge.first, visited, length + edge.second, maxLength);
            }
        }

        visited[node] = false; // Backtrack
    }

    void longestPath(int start) {
        if (adj.find(start) == adj.end()) {
            cout << "Start node does not exist.\n";
            return;
        }

        unordered_map<int, bool> visited;
        int maxLength = 0;
        longestPathUtil(start, visited, 0, maxLength);
        cout << "Longest path length from node " << start << ": " << maxLength << endl;
    }
};

void menu(Graph& g) {
    int choice, u, v, weight, start;

    while (true) {
        cout << "\nWellcome to Shortst Path Navigation Menu:\n";
        cout << "1. Add Node\n";
        cout << "2. Add Edge\n";
        cout << "3. Update Edge\n";
        cout << "4. Delete Node\n";
        cout << "5. Delete Edge\n";
        cout << "6. Display Graph\n";
        cout << "7. BFS Traversal\n";
        cout << "8. DFS Traversal\n";
        cout << "9. Shortest Path (Dijkstra)\n";
        cout << "10. Longest Path\n";
        cout << "11. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter node value: ";
                cin >> u;
                g.addNode(u);
                break;
            case 2:
                cout << "Enter edge (u v weight): ";
                cin >> u >> v >> weight;
                g.addEdge(u, v, weight);
                break;
            case 3:
                cout << "Enter edge to update (u v newWeight): ";
                cin >> u >> v >> weight;
                g.updateEdge(u, v, weight);
                break;
            case 4:
                cout << "Enter node to delete: ";
                cin >> u;
                g.deleteNode(u);
                break;
            case 5:
                cout << "Enter edge to delete (u v): ";
                cin >> u >> v;
                g.deleteEdge(u, v);
                break;
            case 6:
                g.display();
                break;
            case 7:
                cout << "Enter starting node for BFS: ";
                cin >> start;
                g.BFS(start);
                break;
            case 8:
                cout << "Enter starting node for DFS: ";
                cin >> start;
                g.DFS(start);
                break;
            case 9:
                cout << "Enter starting node for Dijkstra: ";
                cin >> start;
                g.dijkstra(start);
                break;
            case 10:
                cout << "Enter starting node for longest path: ";
                cin >> start;
                g.longestPath(start);
                break;
            case 11:
                cout << "Exiting...\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

int main() {
    Graph g;
    menu(g);
    return 0;
}

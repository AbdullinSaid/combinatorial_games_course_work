#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <map>
#include <algorithm>

using namespace std;

void buildGraph(int n,
                 int m,
                 map<vector<int>, vector<vector<int>>>& graph,
                 map<vector<int>, vector<vector<int>>>& reversedGraph,
                 map<vector<int>, int>& degree) {
    int maxi = int(pow((n + 1), m));
    for (int i = 0; i < maxi; ++i) {
        vector<int> current;
        int tmp = i;
        for (int j = 0; j < m; ++j) {
            current.push_back(tmp % (n + 1));
            tmp /= (n + 1);
        }
        graph[current].size(); //Just to insert graph in map
        reversedGraph[current].size();

        /*for (int ind1 = 0; ind1 < m; ++ind1) {
            for (int ind2 = ind1 + 1; ind2 < m; ++ind2) {
                for (int k = 1; k <= min(current[ind1], current[ind2]); ++k) {
                    vector<int> child = current;
                    child[ind1] -= k;
                    child[ind2] -= k;
                    graph[current].push_back(child);
                    degree[current]++;
                    reversedGraph[child].push_back(current);
                }
            }
        }*/

        for (int k = 1; k <= *min_element(current.begin(), current.end()); ++k) {
            vector<int> child(m);
            for (int j = 0; j < m; ++j) {
                child[j] = current[j] - k;
            }
            graph[current].push_back(child);
            degree[current]++;
            reversedGraph[child].push_back(current);
        }

        /*for (int ind1 = 0; ind1 < m; ++ind1) {
            for (int ind2 = ind1 + 1; ind2 < m; ++ind2) {
                for (int ind3 = ind2 + 1; ind3 < m; ++ind3) {
                    for (int k = 1; k <= min(current[ind1], min(current[ind2], current[ind3])); ++k) {
                        vector<int> child = current;
                        child[ind1] -= k;
                        child[ind2] -= k;
                        child[ind3] -= k;
                        graph[current].push_back(child);
                        degree[current]++;
                        reversedGraph[child].push_back(current);
                    }
                }
            }
        }*/

        for (int ind = 0; ind < m; ++ind) {
            for (int k = 1; k <= current[ind]; ++k) {
                vector<int> child = current;
                child[ind] -= k;
                graph[current].push_back(child);
                degree[current]++;
                reversedGraph[child].push_back(current);
            }
        }
    }
}

bool isSorted(const vector<int>& a) {
    for (size_t i = 1; i < a.size(); ++i) {
        if (a[i] < a[i - 1]) {
            return false;
        }
    }
    return true;
}

int main() {
    int n;
    cin >> n;

    map<vector<int>, int> answer;
    map<vector<int>, vector<vector<int>>> graph;
    map<vector<int>, vector<vector<int>>> reversedGraph;
    map<vector<int>, int> degree;
    buildGraph(n, 4, graph, reversedGraph, degree);

    queue<vector<int> > zeroDegree;
    for (const auto &v: graph) {
        if (v.second.empty()) {
            zeroDegree.push(v.first);
            answer[v.first] = 0;
        }
    }
    int currentAnswer = 1;
    while (!zeroDegree.empty()) {
        queue<vector<int> > newZeroDegree;

        while (!zeroDegree.empty()) {
            auto v = zeroDegree.front();
            zeroDegree.pop();
            for (const auto &vertex: reversedGraph[v]) {
                if (answer[vertex] == 0) {
                    answer[vertex] = currentAnswer;
                    for (const auto &vertexToCutEdge: reversedGraph[vertex]) {
                        degree[vertexToCutEdge]--;
                        if (degree[vertexToCutEdge] == 0) {
                            newZeroDegree.push(vertexToCutEdge);
                            answer[vertexToCutEdge] = currentAnswer + 1;
                        }
                    }
                }
            }
        }
        zeroDegree = newZeroDegree;
        currentAnswer += 2;
    }

    for (const auto& ans : answer) {
        if (ans.second % 2 == 0 && isSorted(ans.first)) {
            int counter = 0;
            for (auto i: ans.first) {
                counter ^= i;
                cout << i << ' ';
            }
            cout << endl;
        }
    }

    return 0;
}

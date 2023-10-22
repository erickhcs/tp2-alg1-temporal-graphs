#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <set>

using namespace std;

struct Connection {
  int destVillage, year, time, cost;
};

struct Village {
  int dFromOrigin, year;
  bool processed, visited;
};

int find_min (vector<Village> villages) {
  auto lowestCost = numeric_limits<unsigned>::max();
  int lowestCostNode;

  for (int i = 0; i < villages.size(); i++) {
    Village currentVillage = villages[i];

    bool isNotProcessed = !currentVillage.processed;
    bool isVisited = currentVillage.visited;
    
    if (isVisited && isNotProcessed && currentVillage.dFromOrigin < lowestCost)
    {
      lowestCost = currentVillage.dFromOrigin;
      lowestCostNode = i;
    }
  }

  if (lowestCost == numeric_limits<unsigned>::max()) {
    return -1;
  }

  return lowestCostNode;
}

int main () {
  int villagesNum, connectionsNum;
  int originVillage, destVillage, year, cost, time;

  vector<vector<Connection>> graph;
  Connection newConnection;

  vector<Village> villages;

  scanf("%d %d", &villagesNum, &connectionsNum);

  villages.resize(villagesNum);
  graph.resize(villagesNum);

  for (int i = 0; i < connectionsNum; i++) {
    scanf("%d %d %d %d %d", &originVillage, &destVillage, &year, &time, &cost);

    newConnection.destVillage = destVillage - 1;

    newConnection.year = year;
    newConnection.time = time;
    newConnection.cost = cost;

    graph[originVillage - 1].push_back(newConnection);

    newConnection.destVillage = originVillage - 1;

    graph[destVillage - 1].push_back(newConnection);
  }

  // Dijkstra
  set<int> priorityQueue;

  for (int i = 0; i < villages.size(); i++) {
    villages[i].processed = false;
    villages[i].visited = false;
  }

  villages[0].dFromOrigin = 0;
  villages[0].visited = true;

  priorityQueue.insert(0);

  int u = find_min(villages);
   
  while (u != -1) {
    vector<Connection> neighbourConnections = graph[u];

    for (int i = 0; i < neighbourConnections.size(); i++) {
      int currentVillage = neighbourConnections[i].destVillage;
      int dNeighbour = neighbourConnections[i].time;

      if (!villages[currentVillage].visited || villages[currentVillage].dFromOrigin > villages[u].dFromOrigin + dNeighbour) {
        villages[currentVillage].dFromOrigin = villages[u].dFromOrigin + dNeighbour;
        villages[currentVillage].visited = true;
        villages[currentVillage].year = neighbourConnections[i].year;
      }
    }

    villages[u].processed = true;

    u = find_min(villages);
  }

  // for (int i = 0; i < graph.size(); i++) {
  //   for(int j = 0; j < graph[i].size(); j++) {
  //     cout << graph[i][j].destVillage << "\n";
  //     cout << graph[i][j].year << "\n";
  //     cout << graph[i][j].time << "\n";
  //     cout << graph[i][j].cost << "\n";
  //     cout << "\n" << "\n";
  //   }
  // }

  int firstYearOfCompleteConnections = 0;

  for (int i = 0; i < villages.size(); i++) {
    if (villages[i].year > firstYearOfCompleteConnections) {
      firstYearOfCompleteConnections = villages[i].year;
    }

    cout << villages[i].dFromOrigin << "\n";
  }

  cout << firstYearOfCompleteConnections << "\n";

  return 0;
}

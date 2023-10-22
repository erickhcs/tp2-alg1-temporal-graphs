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
  int dFromOrigin;
  bool processed;
  bool visited;
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

  vector<vector<Connection>> connections;
  Connection newConnection;

  vector<Village> villages;

  scanf("%d %d", &villagesNum, &connectionsNum);

  connections.resize(connectionsNum);
  villages.resize(villagesNum);

  for (int i = 0; i < connectionsNum; i++) {
    scanf("%d %d %d %d %d", &originVillage, &destVillage, &year, &time, &cost);

    newConnection.destVillage = destVillage - 1;

    newConnection.year = year;
    newConnection.time = time;
    newConnection.cost = cost;

    connections[originVillage - 1].push_back(newConnection);
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

  // cout << "\n\n";
  // while (!pq.empty()) {
  //     std::cout << pq.top() << " ";
  //     pq.pop();
  // }
  //  cout << "\n\n";

  int u = find_min(villages);
   
  while (u != -1) {
    vector<Connection> neighbourConnections = connections[u];

    for (int i = 0; i < neighbourConnections.size(); i++) {
      int currentVillage = neighbourConnections[i].destVillage;
      int dNeighbour = neighbourConnections[i].time;

      if (!villages[currentVillage].visited || villages[currentVillage].dFromOrigin > villages[u].dFromOrigin + dNeighbour) {
        villages[currentVillage].dFromOrigin = villages[u].dFromOrigin + dNeighbour;
        villages[currentVillage].visited = true;
      }
    }

    villages[u].processed = true;

    u = find_min(villages);
  }

  for (int i = 0; i < connections.size(); i++) {
    for(int j = 0; j < connections[i].size(); j++) {
      cout << connections[i][j].destVillage << "\n";
      cout << connections[i][j].year << "\n";
      cout << connections[i][j].time << "\n";
      cout << connections[i][j].cost << "\n";
      cout << "\n" << "\n";
    }
  }

  for (int i = 0; i < villages.size(); i++) {
    cout << villages[i].dFromOrigin << "\n";
  }

  return 0;
}

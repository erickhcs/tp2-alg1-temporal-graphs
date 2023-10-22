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
  bool processed, visited, primYearVisited, primCostVisited;
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
    villages[i].primYearVisited = false;
    villages[i].primCostVisited = false;
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

  // Prim Year
  int lowestYear = 0;
  auto compareYear = [](Connection a, Connection b) { return a.year > b.year; };
    
  priority_queue<int, vector<Connection>, decltype(compareYear)> primYearQueue(compareYear);

  villages[0].primYearVisited = true;

  for (Connection connection: graph[0]) {
    primYearQueue.push(connection);
  }

  while (!primYearQueue.empty()) {
    Connection connectionLowestYear = primYearQueue.top();
    primYearQueue.pop();

    if (villages[connectionLowestYear.destVillage].primYearVisited == true) continue;

    villages[connectionLowestYear.destVillage].primYearVisited = true;
    
    if (connectionLowestYear.year > lowestYear) {
      lowestYear = connectionLowestYear.year;
    }

    for (Connection connection: graph[connectionLowestYear.destVillage]) {
      primYearQueue.push(connection);
    }
  }

   // Prim Cost
  int sumCost = 0;
  auto compareCost = [](Connection a, Connection b) { return a.cost > b.cost; };
    
  priority_queue<int, vector<Connection>, decltype(compareCost)> primCostQueue(compareCost);

  villages[0].primCostVisited = true;

  for (Connection connection: graph[0]) {
    primCostQueue.push(connection);
  }

  while (!primCostQueue.empty()) {
    Connection connectionLowestCost = primCostQueue.top();
    primCostQueue.pop();

    if (villages[connectionLowestCost.destVillage].primCostVisited == true) continue;

    sumCost += connectionLowestCost.cost;

    villages[connectionLowestCost.destVillage].primCostVisited = true;

    for (Connection connection: graph[connectionLowestCost.destVillage]) {
      primCostQueue.push(connection);
    }
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
  cout << lowestYear << "\n";
  cout << sumCost << "\n";

  return 0;
}

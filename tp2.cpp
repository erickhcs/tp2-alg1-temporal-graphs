#include <iostream>
#include <vector>
#include <limits.h>
#include <queue>

using namespace std;

struct Connection {
  int destVillage, year, time, cost;
};

struct Village {
  long int dFromOrigin;
  int year, villageIndex;
  bool primYearVisited, primCostVisited;
};

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
  auto compareDistance = [](Village a, Village b) { return a.dFromOrigin > b.dFromOrigin; };
    
  priority_queue<int, vector<Village>, decltype(compareDistance)> djQueue(compareDistance);

  for (int i = 0; i < villages.size(); i++) {
    villages[i].dFromOrigin = LONG_MAX;
    villages[i].villageIndex = i;
    villages[i].primYearVisited = false;
    villages[i].primCostVisited = false;
  }

  villages[0].dFromOrigin = 0;
  djQueue.push(villages[0]);

  while (!djQueue.empty()) {
    Village currentVillage = djQueue.top();
    int v = currentVillage.villageIndex;
    long int w = currentVillage.dFromOrigin;

    djQueue.pop();

    if (w != villages[v].dFromOrigin) continue;

    for (int i = 0; i < graph[v].size(); i++) {
      Connection currentConnection = graph[v][i];

      int u = currentConnection.destVillage;
      int cost = currentConnection.time;
      int year = currentConnection.year;

      if (villages[u].dFromOrigin > villages[v].dFromOrigin + cost) {
        villages[u].dFromOrigin = villages[v].dFromOrigin + cost;
        villages[u].year = year;
        djQueue.push(villages[u]);
      }
    }
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
      if (villages[connection.destVillage].primYearVisited == false) {
        primYearQueue.push(connection);
      }
    }
  }

   // Prim Cost
  long int sumCost = 0;
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
      if (villages[connection.destVillage].primCostVisited == false) {
        primCostQueue.push(connection);
      }
    }
  }

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

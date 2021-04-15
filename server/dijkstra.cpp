#include "dijkstra.h"
#include "heap.h"
#include <iostream>

using namespace std;

typedef pair<int, int> PII;
//e.g. PII x; u = x.first, v = x.second.

void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PIL>& reached){


    BinaryHeap <PII, long long> events;

    // at time 0, the startVertex burns, we use -1 to indicate there is
    // no "predecessor" of the startVertex
    events.insert(PII(-1, startVertex), 0);
    //                 u      v         c
    // So, 
    //     
    //     cost = x.key;

    // while there is an active fire
    while (events.size() > 0) {

        //storing an HeapItem with minimum cost.
        HeapItem <PII, long long>lowest_cost = events.min();

        events.popMin();

        if(reached.find(lowest_cost.item.second) == reached.end()){

            //Here, u = lowest_cost.item.first, v = lowest_cost.item.second, cost = lowest_cost.key
            reached[lowest_cost.item.second];
            reached[lowest_cost.item.second].first = lowest_cost.item.first;
            reached[lowest_cost.item.second].second = lowest_cost.key;

            //for all neigbours of vertex v.
            for(auto w = graph.neighbours(lowest_cost.item.second); w != graph.endIterator(lowest_cost.item.second); w++){
                
                pair<int, int> temp;

                temp.first = lowest_cost.item.second;
                temp.second = (*w);

                //start burning edge(v, w)              
                events.insert(temp, lowest_cost.key + graph.getCost(lowest_cost.item.second, (*w)));
            }
        }
    }
}

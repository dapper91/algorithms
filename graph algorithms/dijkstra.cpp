#include <vector>
#include <forward_list>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <tuple>
#include <functional>


template <typename T>
using Graph = std::vector<std::forward_list<T>>;


template <typename T1, typename T2>
void dijkstra(const Graph<std::pair<T1, T2>>& g, T2 start, std::vector<T1>& dist_array, std::vector<T2>& prev_array)
{
    typedef std::pair<T1, T2> PairType;

    std::priority_queue<PairType, std::vector<PairType>, std::greater<PairType>> queue;

    dist_array[start] = 0;
    queue.push(std::make_pair(0, start));

    while(!queue.empty()) {
        T1 dist; T2 vertex;
        std::tie(dist, vertex) = queue.top(); queue.pop();

        if (dist > dist_array[vertex]) {
            continue;
        }

        for (auto& pair : g[vertex]) {
            T1 cost; T2 to;
            std::tie(cost, to) = pair;

            if (dist + cost < dist_array[to]) {
                dist_array[to] = dist + cost;
                prev_array[to] = vertex;
                queue.push(std::make_pair(dist_array[to], to));
            }
        }
    }
}



int main()
{
    Graph<std::pair<int, size_t>> g = {
        {{1,2},{1,3}},
        {{1,0},{1,2},{1,3}},
        {{1,0},{1,1},{1,3}},
        {{1,0},{1,1},{1,2}}
    };

    size_t start = 0;

    int INF = std::numeric_limits<int>::max();

    std::vector<int> dist(g.size(), INF);
    std::vector<size_t> prev(g.size(), 0);

    dijkstra(g, start, dist, prev);

    for (size_t v = 0; v < g.size(); ++v) {
        std::cout << v << '\t' << dist[v] << '\t' << prev[v] << std::endl;
    }

    return 0;
}
#include <cstddef>
#include <vector>
#include <forward_list>
#include <iostream>
#include <queue>


template <typename T>
using Graph = std::vector<std::forward_list<T>>;


template<typename T>
void bfs(const Graph<T>& g, T start)
{
    std::vector<char> color(g.size(), 0);
    std::queue<T> queue;

    queue.push(start);
    color[start] = 1;

    while (!queue.empty()) {
        T cur_vertex = queue.front(); queue.pop();

        std::cout << cur_vertex << std::endl;

        for (const T& vertex: g[cur_vertex]) {
            if (color[vertex] == 0) {
                queue.push(vertex);
                color[vertex] = 1;
            }
        }
    }
}


int main()
{
    Graph<size_t> g = {
        {2,3},
        {0,2,3},
        {0,1,3},
        {0,1,2}
    };

    size_t start = 0;

    bfs(g, start);

    return 0;
}
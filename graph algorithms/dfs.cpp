#include <cstddef>
#include <vector>
#include <forward_list>
#include <iostream>


template <typename T>
using Graph = std::vector<std::forward_list<T>>;


template<typename T>
void dfs(const Graph<T>& g, T start, std::vector<char>& color)
{
    color[start] = 1;

    std::cout << start << std::endl;

    for (const T& vertex: g[start]) {
        if (color[vertex] == 0) {
            dfs(g, vertex, color);
        }
    }
}


template<typename T>
void dfs(const Graph<T>& g, T start)
{
    std::vector<char> color(g.size(), 0);
    dfs(g, start, color);
}


int main()
{
    Graph<size_t> g = {
        {1,2,3},
        {0,2,3},
        {0,1,3},
        {0,1,2}
    };

    size_t start = 0;

    dfs(g, start);

    return 0;
}
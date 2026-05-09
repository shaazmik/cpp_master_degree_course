//@compile g++ part6/11_06.cpp -std=c++20 -Wall -Wextra -Wpedantic -isystem boost_1_85_0
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <vector>

#include <boost/graph/adjacency_matrix.hpp>

using EdgeProp = boost::property < boost::edge_weight_t, int >;
using Graph    = boost::adjacency_matrix < boost::undirectedS, boost::no_property, EdgeProp >;
using Vertex   = boost::graph_traits < Graph >::vertex_descriptor;

int main()
{
    constexpr std::size_t vertex_count = 10;

    std::random_device                 rd;
    std::default_random_engine         engine(rd());
    std::uniform_int_distribution<int> dist(1, 10);

    Graph g(vertex_count);

    for (Vertex i = 0; i < vertex_count; ++i)
        for (Vertex j = i + 1; j < vertex_count; ++j)
            boost::add_edge(i, j, EdgeProp(dist(engine)), g);

    auto const weight_map = boost::get(boost::edge_weight, g);

    std::cout << "Adjacency matrix:\n";

    for (Vertex i = 0; i < vertex_count; ++i)
    {
        for (Vertex j = 0; j < vertex_count; ++j)
        {
            if (i == j)
                std::cout << std::setw(3) << 0;
            else
                std::cout << std::setw(3) << weight_map[boost::edge(i, j, g).first];
        }
        std::cout << '\n';
    }

    std::vector < Vertex > path(vertex_count - 1);
    std::iota(path.begin(), path.end(), Vertex{1});

    int                    best_cost = std::numeric_limits < int >::max();
    std::vector < Vertex > best_path = path;

    do
    {
        int    cost = 0;
        Vertex prev = 0;

        for (Vertex v : path)
        {
            cost += weight_map[boost::edge(prev, v, g).first];
            prev  = v;
        }

        cost += weight_map[boost::edge(prev, Vertex{0}, g).first];

        if (cost < best_cost)
        {
            best_cost = cost;
            best_path = path;
        }
    }
    while (std::next_permutation(path.begin(), path.end()));

    std::cout << "\nOptimal path: 0";

    for (Vertex v : best_path)
        std::cout << " -> " << v;

    std::cout << " -> 0\nTotal cost: " << best_cost << '\n';

    return 0;
}

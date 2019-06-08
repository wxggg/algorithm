#include <iostream>
#include <map>
#include <vector>
#include "include/graph.hh"

using namespace std;

void test_mst() {
    wxg::graph<char, int> g;

    for (auto &&v : {'A', 'B', 'C', 'D', 'E', 'F', 'G'}) g.add_node(v);

    g.add_edge_bidirection('A', 'B', 7);
    g.add_edge_bidirection('A', 'D', 5);

    g.add_edge_bidirection('B', 'C', 8);
    g.add_edge_bidirection('B', 'D', 9);
    g.add_edge_bidirection('B', 'E', 7);

    g.add_edge_bidirection('D', 'E', 15);
    g.add_edge_bidirection('D', 'F', 6);

    g.add_edge_bidirection('C', 'E', 5);

    g.add_edge_bidirection('E', 'F', 8);
    g.add_edge_bidirection('E', 'G', 9);

    g.add_edge_bidirection('F', 'G', 11);

    // g.bfs([&g](char x) { g.print_node(x); });
    // cout << endl;

    // g.dfs([&g](char x) { g.print_node(x); });
    // cout << endl;

    cout << g.prim() << endl;
    cout << g.kruskal() << endl;
}

void test_topo() {
    wxg::graph<int, int> g;

    for (auto &&v : {1, 2, 3, 4, 5}) g.add_node(v);

    g.add_edge(1, 2, 1);
    g.add_edge(1, 3, 1);

    g.add_edge(2, 3, 1);

    g.add_edge(3, 4, 1);
    g.add_edge(3, 5, 1);

    g.add_edge(4, 5, 1);

    vector<int> res;

    auto istopo = g.toposort(1, [&res](int v) { res.push_back(v); });

    if (istopo) {
        for (const auto &v : res) cout << v << ",";
        cout << endl;
    }
}

// Activity On Edge Network
void test_aoe() {
    wxg::graph<char, int> g;

    for (auto &&v : {'1', '2', '3', '4', '5', '6', '7', '8', '9'})
        g.add_node(v);

    g.add_edge('1', '2', 6);
    g.add_edge('1', '3', 4);
    g.add_edge('1', '4', 5);

    g.add_edge('2', '5', 1);
    g.add_edge('3', '5', 1);

    g.add_edge('4', '6', 2);

    g.add_edge('5', '7', 9);
    g.add_edge('5', '8', 7);
    g.add_edge('6', '8', 4);

    g.add_edge('7', '9', 2);
    g.add_edge('8', '9', 4);

    auto res = g.critical_path('1', '9', [](char x) { cout << x << ","; });
    cout << endl;
    cout << "res=" << res << endl;
}

void test_path() {
    wxg::graph<char, int> g;

    for (auto &&v : {'A', 'B', 'C', 'D', 'E'}) g.add_node(v);

    g.add_edge('A', 'B', 10);
    g.add_edge('A', 'D', 30);
    g.add_edge('A', 'E', 100);

    g.add_edge('B', 'C', 50);
    g.add_edge('C', 'E', 10);

    g.add_edge('D', 'C', 20);
    g.add_edge('D', 'E', 60);

    auto res = g.dijkstra('A', 'E');
    cout << "res=" << res << endl;

    std::vector<std::vector<int>> distance;
    std::map<char, int> index;

    g.floyd(distance, index);

    cout << distance[index['A']][index['E']] << endl;
}

int main(int argc, char const *argv[]) {
    // test_mst();

    // test_topo();

    // test_aoe();

    test_path();
    return 0;
}

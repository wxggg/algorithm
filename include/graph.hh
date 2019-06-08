#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

namespace wxg {

template <class Node, class Edge>
struct graph_edge {
    Edge weight;
    Node u;
    Node v;

    graph_edge(Node u, Node v, Edge e) : weight(e), u(u), v(v) {}
};

template <class Node, class Edge>
class graph {
   private:
    typedef Node node_t;

    typedef graph_edge<Node, Edge> edge_t;

    std::map<node_t, std::set<edge_t *>> adjacent;
    std::map<node_t, std::set<edge_t *>> adjacent_reversed;

   public:
    graph() {}
    ~graph() {}

    bool empty() const { return adjacent.empty(); }

    node_t begin() const { return (*adjacent.begin()).first; }

    size_t size() const { return adjacent.size(); }

    void add_node(const Node &v) { adjacent[v] = {}; }

    void add_edge(Node u, Node v, Edge e) {
        edge_t *edge = new edge_t(u, v, e);
        adjacent[u].insert(edge);
        adjacent_reversed[v].insert(edge);
    }

    void add_edge_bidirection(Node u, Node v, Edge e) {
        add_edge(u, v, e);
        add_edge(v, u, e);
    }

    Edge prim() {
        if (empty()) return (Edge)0;

        Edge mst = (Edge)0;

        std::set<node_t> used;
        used.insert(begin());

        edge_t *edge;
        while (used.size() < size()) {
            edge = nullptr;
            for (const auto &v : used) {
                for (const auto &e : adjacent[v])
                    if (!used.count(e->v))
                        if (!edge || e->weight < edge->weight) edge = e;
            }
            if (edge) {
                used.insert(edge->v);
                mst += edge->weight;
            }
        }
        return mst;
    }

    Edge kruskal() {
        if (empty()) return (Edge)0;

        Edge mst = (Edge)0;

        // value reparesent which sub graph node belongs to
        std::map<node_t, int> nodes;
        std::map<int, std::set<node_t>> nodeset;

        auto compare = [](edge_t *e1, edge_t *e2) {
            return e1->weight < e2->weight;
        };
        auto edges = std::multiset<edge_t *, decltype(compare)>(compare);

        for (const auto &kv : adjacent) {
            static int i = 0;
            nodes[kv.first] = i;
            nodeset[i++].insert(kv.first);
            for (const auto &e : kv.second) edges.insert(e);
        }

        for (const auto &e : edges) {
            int i = nodes[e->u], j = nodes[e->v];
            if (i != j) {
                if (i > j) std::swap(i, j);

                for (const auto &v : nodeset[j]) {
                    nodes[v] = i;
                    nodeset[i].insert(v);
                }
                mst += e->weight;

                if (nodeset[i].size() == size()) return mst;
            }
        }
        return mst;
    }

    template <class Func>
    bool toposort(node_t start, Func &&func) {
        std::map<node_t, int> indegree;
        for (const auto &kv : adjacent)
            for (const auto &e : kv.second) indegree[e->v]++;

        std::queue<node_t> que;

        for (const auto &kv : adjacent) {
            auto v = kv.first;
            if (indegree[v] == 0) {
                que.push(v);
                indegree[v] = -1;
            }
        }

        while (!que.empty()) {
            auto x = que.front();
            func(x);
            que.pop();
            for (const auto &e : adjacent[x]) {
                if (indegree[e->v] == -1) return false;
                if (--indegree[e->v] == 0) {
                    que.push(e->v);
                    indegree[e->v] = -1;
                }
            }
        }
        return true;
    }

    template <class Func>
    int critical_path(node_t source, node_t target, Func &&func) {
        std::vector<node_t> vec;
        std::map<node_t, size_t> latest, earlist;

        auto f = [this, &vec, &earlist](node_t v) {
            vec.push_back(v);
            for (const auto &e : adjacent[v])
                if (earlist[v] + e->weight > earlist[e->v])
                    earlist[e->v] = earlist[v] + e->weight;
        };
        if (!toposort(source, f)) return -1;

        for (const auto &kv : adjacent) latest[kv.first] = earlist[target];

        for (int i = vec.size() - 1; i >= 0; i--) {
            auto v = vec[i];
            for (const auto &e : adjacent_reversed[v])
                if (latest[v] - e->weight < latest[e->u])
                    latest[e->u] = latest[v] - e->weight;
        }

        for (const auto &v : vec)
            if (earlist[v] == latest[v]) func(v);

        return earlist[target];
    }

    Edge dijkstra(node_t source, node_t target) {
        std::map<node_t, Edge> distance;

        auto f = [&distance, this](node_t v) {
            for (const auto &e : adjacent[v])
                if (distance.count(e->v) == 0 ||
                    distance[v] + e->weight < distance[e->v])
                    distance[e->v] = distance[v] + e->weight;
        };

        bfs(source, f);

        return distance[target];
    }

    void floyd(std::vector<std::vector<Edge>> &distance,
               std::map<node_t, int> &index) {
        int n = size();
        std::vector<node_t> nodes;

        nodes.resize(n);
        distance.resize(n);
        for (int i = 0; i < n; i++) distance[i].resize(n);

        for (const auto &kv : adjacent) {
            static int i = 0;
            index[kv.first] = i;
            nodes[i] = kv.first;
            i++;
        }

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) distance[i][j] = (Edge)(-1);

        for (int i = 0; i < n; i++) {
            auto u = nodes[i];
            for (const auto &e : adjacent[u])
                distance[index[u]][index[e->v]] = e->weight;
        }

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++) {
                    if (distance[i][k] >= 0 && distance[k][j] >= 0)
                        if (distance[i][j] == (Edge)-1 ||
                            distance[i][k] + distance[k][j] < distance[i][j])
                            distance[i][j] = distance[i][k] + distance[k][j];
                }
    }

    void print_edge(edge_t *edge) {
        std::cout << edge->u << "_" << edge->weight << "_" << edge->v << ", ";
    }

    void print_node(node_t v) {
        if (!adjacent.count(v)) return;
        std::cout << "[" << v << ":";
        for (const auto &e : adjacent[v]) print_edge(e);
        std::cout << "]";
    }

    template <class Func>
    void bfs(node_t start, Func &&func) {
        if (adjacent.empty()) return;

        std::set<node_t> visited;
        std::queue<node_t> que;

        que.push(start);
        visited.insert(start);

        while (!que.empty()) {
            auto x = que.front();
            func(x);
            que.pop();
            for (const auto &e : adjacent[x])
                if (!visited.count(e->v)) {
                    visited.insert(e->v);
                    que.push(e->v);
                }
        }
    }

    template <class Func>
    void bfs(Func &&func) {
        bfs(begin(), func);
    }

    template <class Func>
    void dfs(node_t start, Func &&func) {
        if (adjacent.empty()) return;

        std::set<node_t> visited;
        std::stack<node_t> stk;

        stk.push(begin());
        visited.insert(begin());

        while (!stk.empty()) {
            auto x = stk.top();
            func(x);
            stk.pop();
            for (const auto &e : adjacent[x])
                if (!visited.count(e->v)) {
                    visited.insert(e->v);
                    stk.push(e->v);
                }
        }
    }
    template <class Func>
    void dfs(Func &&func) {
        dfs(begin(), func);
    }
};

}  // namespace wxg
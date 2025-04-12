#pragma once
#include "structure.h"



// 并查集
class Un {
public:
    Un(int n): fa(n) {
        iota(fa.begin(), fa.end(), 0);
    }

    int find(int x) {
        if (x != fa[x]) {
            fa[x] = find(fa[x]);
        }
        return fa[x];
    }

    void merge(int x, int y) {
        x = find(x), y = find(y);
        fa[x] = y;
    }

    int operator[](int i) {
        return find(i);
    }

    std::vector<int> fa;
};

// 求关键边 tarjan算法
class TarjanSCC {
   private:
    // 联通的节点
    std::vector<std::vector<int>> edges;
    // 对应的边id
    std::vector<std::vector<int>> edgesId;
    // 最大连通分量的序号
    std::vector<int> low;
    // 节点在dfs中的执行序号
    std::vector<int> dfn;
    // 最后的关键边
    std::vector<int> ans;
    // 节点个数
    int n;
    // dfs执行序号
    int ts;

   private:
    void getCuttingEdge_(int u, int parentEdgeId) {
        low[u] = dfn[u] = ++ts;
        for (size_t i = 0; i < edges[u].size(); ++i) {
            int v = edges[u][i];
            int id = edgesId[u][i];
            if (dfn[v] == -1) {
                getCuttingEdge_(v, id);
                low[u] = std::min(low[u], low[v]);
                if (low[v] > dfn[u]) {
                    ans.push_back(id);
                }
            } else if (id != parentEdgeId) {
                low[u] = std::min(low[u], dfn[v]);
            }
        }
    }

   public:
    TarjanSCC(int n_, const std::vector<std::vector<int>>& edges_, int mask)
        : low(n_, -1), dfn(n_, -1), n(n_), ts(-1) {
        for (size_t i = 0; i < edges_.size(); ++i) {
            auto&& a = edges_[i];
            if (a[0] & mask) {
                edges[a[1]].push_back(a[2]);
                edges[a[2]].push_back(a[1]);
                edgesId[a[1]].push_back(i);
                edgesId[a[2]].push_back(i);
            }
        }
    }

    std::vector<int> getCuttingEdge() {
        getCuttingEdge_(0, -1);
        return ans;
    }
};

// 求逆元 辗转相除法
std::pair<int, int> gcd_dfs(int a, int b) {
    int c = a % b, d = -a / b;
    if (c == 1) {
        return std::make_pair(1, d);
    }
    auto [e, f] = gcd_dfs(b, c);

    return std::make_pair(f, e + f * d);
}
int seek_origin(int a, int mod) {
    int ans = gcd_dfs(mod, a).second;
    return ans < 0 ? ans + mod : ans;
}




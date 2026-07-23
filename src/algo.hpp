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



template<int MOD = 1000000007>
class ModInt {
    int num;
public:
    ModInt(long long n = 0) { n %= MOD; num = n < 0 ? n + MOD : n; }
    int get() const { return num; }

    ModInt& operator += (const ModInt& other) { return this->operator=(num + other.num); }
    ModInt operator + (const ModInt& other) const { return ModInt(*this) += other; }

    ModInt& operator -= (const ModInt& other) { return this->operator=(num - other.num); }
    ModInt operator - (const ModInt& other) const { return ModInt(*this) -= other; }

    ModInt& operator *= (const ModInt& other) { return this->operator=((long long)num * other.num); }
    ModInt operator * (const ModInt& other) const { return ModInt(*this) *= other; }

    ModInt pow(int exp) const {
        ModInt ans(1), base(*this);
        while (exp) {
            if (exp & 1) {
                ans *= base;
            }
            base *= base;
            exp >>= 1;
        }
        return ans;
    }

    ModInt& operator /= (const ModInt& other) { return *this *= other.pow(MOD - 2); }
    ModInt operator / (const ModInt& other) const { return ModInt(*this) /= other; }
};


// 线段树不同的合并策略
template<typename T>
struct Max {
    static constexpr T identity = std::numeric_limits<T>::lowest();
    T operator()(const T& a, const T& b) const {return std::max(a, b);}
};

template<typename T>
struct Min {
    static constexpr T identity = std::numeric_limits<T>::max();
    T operator()(const T& a, const T& b) const {return std::min(a, b);}
};

template<typename T>
struct Sum {
    static constexpr T identity = T{};
    T operator()(const T& a, const T& b) const {return a + b;}
};

// 线段树
template<
    typename T,
    template<typename> class Merge
>
class SegmentTree {
    std::vector<T> seg;
    int n;

    void update(int l, int r, int i, int pos, const T& val) {
        if (l == r) {
            seg[i] = val;
            return;
        }
        int mid = (l + r) / 2;

        if (pos <= mid){
            update(l, mid, i * 2, pos, val);
        }
        else{
            update(mid + 1, r, i * 2 + 1, pos, val);
        }

        seg[i] = Merge<T>{}(seg[i * 2], seg[i * 2 + 1]);
    }

    T query(int l, int r, int i, int sl, int sr) {
        if (sl <= l && r <= sr)
            return seg[i];

        int mid = (l + r) / 2;

        if (sr <= mid){
            return query(l, mid, i * 2, sl, sr);
        }
        else if (sl > mid){
            return query(mid + 1, r, i * 2 + 1, sl, sr);
        }

        auto ra = query(mid + 1, r, i * 2 + 1, mid + 1, sr);
        auto la = query(l, mid, i * 2, sl, mid);

        return Merge<T>{}(la, ra);
    }

    void build(int l, int r, int i, const std::vector<T>& data) {
        if (l == r) {
            seg[i] = data[l];
            return;
        }

        int mid = (l + r) / 2;
        build(l, mid, i * 2, data);
        build(mid + 1, r, i * 2 + 1, data);

        seg[i] = Merge<T>{}(seg[i * 2], seg[i * 2 + 1]);
    }

public:
    SegmentTree(int n) :n(n), seg(n * 4, Merge<T>::identity) {};
    SegmentTree(const std::vector<T>& data) : n(data.size()), seg(4 * data.size(), Merge<T>::identity) { build(data); }

    void update(int pos, const T& val) {
        update(0, n - 1, 1, pos, val);
    }
    
    T query(int l, int r) {
        return query(0, n - 1, 1, l, r);
    }

    void build(const std::vector<T>& data) {
        build(0, n - 1, 1, data);
    }
};
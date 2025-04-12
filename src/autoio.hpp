#pragma once
#include "structure.h"
#include "parse.hpp"

// c++方便的输出vector和map:重载的应用
namespace py_pr {
template <typename T>
inline std::ostream& out_put(std::ostream& o, const T& x) {
    return o << x;
}
inline std::ostream& out_put(std::ostream& o, const std::string& x) {
    return o << "\"" << x << "\"";
}
inline std::ostream& out_put(std::ostream& o, const char*& x) {
    return o << "\"" << x << "\"";
}
inline std::ostream& out_put(std::ostream& o, const char& x) {
    return o << "\'" << x << "\'";
}
template <typename T1, typename T2>
inline std::ostream& out_put(std::ostream& o, const std::pair<T1, T2>& x) {
    out_put(o, x.first);
    o << ": ";
    out_put(o, x.second);
    return o;
}
}

template <typename T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& x) {
    o << "[";
    for (auto it = x.begin(); it != x.end(); ++it) {
        if (it != x.begin())
        o << ", ";
        py_pr::out_put(o, *it);
    }
    o << "]";
    return o;
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& o, const std::map<T1, T2>& x) {
    o << "{";
    for (auto it = x.begin(); it != x.end(); ++it) {
        if (it != x.begin())
        o << ", ";
        py_pr::out_put(o, *it);
    }
    o << "}";
    return o;
}

std::vector<std::string> split(const std::string& str, const std::string& pattern) {
    std::regex re(pattern);
    std::sregex_token_iterator it(str.begin(), str.end(), re, -1);
    std::sregex_token_iterator end;
    return {it, end};
}



template<typename Ret, typename Cls, typename... Args>
void run_case(Ret (Cls::*func)(Args...), const std::vector<std::string>& inputs) {
    auto args = parse_tuple_helper<Args...>(inputs);
    Cls s;
    if constexpr (std::is_same_v<Ret, void>) {
        std::apply([&](auto&&... unpacked) {
            (s.*func)(unpacked...);
        }, args);
    } 
    else {
        auto call = [&](auto&&... unpacked) -> Ret {
            return (s.*func)(unpacked...);
        };
        Ret res = std::apply(call, args);
        py_pr::out_put(cout, res);
    }
}

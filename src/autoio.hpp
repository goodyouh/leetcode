#pragma once
#include "structure.h"
#include "parse.hpp"

// c++方便的输出vector和map:重载的应用
namespace py_pr {
    template <typename T>
    inline std::ostream& out_put(std::ostream& o, const T& x)
    {
        return o << x;
    }
    inline std::ostream& out_put(std::ostream& o, const std::string& x)
    {
        return o << "\"" << x << "\"";
    }
    inline std::ostream& out_put(std::ostream& o, const char*& x)
    {
        return o << "\"" << x << "\"";
    }
    inline std::ostream& out_put(std::ostream& o, const char& x)
    {
        return o << "\'" << x << "\'";
    }
    inline std::ostream& out_put(std::ostream& o, const bool& x)
    {
        if (x) {
            return o << "true";
        }
        else {
            return o << "false";
        }
    }

    template <typename T1, typename T2>
    inline std::ostream& out_put(std::ostream& o, const std::pair<T1, T2>& x)
    {
        out_put(o, x.first);
        o << ":";
        out_put(o, x.second);
        return o;
    }

    template <typename T>
    std::ostream& out_put(std::ostream& o, const std::vector<T>& x)
    {
        if (x.empty()) {
            return o << "[]";
        }
        o << "[";
        auto it = x.begin();
        while (it + 1 != x.end()) {
            py_pr::out_put(o, *it) << ", ";
            ++it;
        }
        return py_pr::out_put(o, *it) << "]";
    }

    template <typename T1, typename T2>
    std::ostream& out_put(std::ostream& o, const std::map<T1, T2>& x)
    {
        if (x.empty()) {
            return o << "{}";
        }
        o << "{";
        auto it = x.begin();
        while (it + 1 != x.end()) {
            py_pr::out_put(o, *it) << ", ";
            ++it;
        }
        return py_pr::out_put(o, *it) << "}";
    }

    std::ostream& out_put(std::ostream& o, ListNode* phead)
    {
        if (phead == nullptr) {
            return o << "[]";
        }
        o << "[";
        auto pnode = phead;
        while (pnode->next) {
            py_pr::out_put(o, pnode->val) << ", ";
            pnode = pnode->next;
        }
        return py_pr::out_put(o, pnode->val) << "]";
    }
}



std::vector<std::string> split(const std::string& str, const std::string& pattern)
{
    std::regex re(pattern);
    std::sregex_token_iterator it(str.begin(), str.end(), re, -1);
    std::sregex_token_iterator end;
    return {it, end};
}







template<typename... Args, std::size_t... Is>
static std::tuple<std::remove_reference_t<Args>...>
parse_tuple(const std::vector<std::string>& tokens, std::index_sequence<Is...>)
{
    return std::make_tuple(parsevalue_wrapper<Args>(tokens[Is])...);
}

template<typename Ret, typename Cls, typename... Args>
void run_case(Ret (Cls::*func)(Args...), const std::vector<std::string>& inputs) {
    auto args = parse_tuple<Args...>(inputs, std::index_sequence_for<Args...>{});
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
        py_pr::out_put(std::cout, res) << std::endl;
    }
}


#include <fstream>
template<typename Ret, typename Cls, typename... Args>
void run_case(Ret (Cls::*func)(Args...), const std::string& filename) {
    std::vector<std::string> inputs;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return;
    }
    
    // 逐行读取
    std::string line;
    while (std::getline(file, line)) {
        // 去除首尾空白字符（包括换行符）
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        if (!line.empty()) {
            inputs.push_back(line);
        }
    }
    file.close();
    
    // 调用另一个run_case函数
    run_case(func, inputs);
}

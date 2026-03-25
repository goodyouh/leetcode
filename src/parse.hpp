#pragma once
#include "structure.h"



template<typename T>
struct parsevalue {
    parsevalue() {};

    T operator()(const std::string& str)
    {
// 默认实现，需要为每种类型特化
        std::istringstream iss(str);
        T value;
        if (!(iss >> value)) {
            throw std::invalid_argument("Cannot parse value");
        }
        return value;
    }
};

template<>
struct parsevalue<std::string> {
    std::string operator()(const std::string& str)
    {
        if (str.size() >= 2 && str.front() == '"' && str.back() == '"') {
            return str.substr(1, str.length() - 2);
        }
        return str;
    }
};

template<typename T>
struct parsevalue<std::vector<T>> {
    std::vector<T> operator()(const std::string& str)
    {
        std::vector<T> result;
    
        if (str.empty() || str.front() != '[' || str.back() != ']') {
            return result;
        }
        
        std::string inner = str.substr(1, str.size() - 2);
        int level = 0;
        size_t start = 0;
        
        for (size_t i = 0; i <= inner.size(); ++i) {
            bool at_end = (i == inner.size());
            char c = at_end ? '\0' : inner[i];
            
            if (c == '[') level++;
            if (c == ']') level--;
            
            if (at_end || (c == ',' && level == 0)) {
                if (i > start) {
                    std::string elem = inner.substr(start, i - start);
                    result.push_back(parsevalue<T>{}(elem));
                }
                start = i + 1;
            }
        }
        
        return result;
    }
};

template<>
struct parsevalue<TreeNode*> {
    TreeNode* operator()(const std::string& str)
    {
        auto nodes = parsevalue<std::vector<std::string>>{}(str);
        if (nodes.empty() || nodes[0] == "null") return nullptr;
        TreeNode* root = new TreeNode(std::stoi(nodes[0]));
        std::queue<TreeNode*> q;
        q.push(root);
        size_t i = 1;
        while (!q.empty() && i < nodes.size()) {
            TreeNode* current = q.front();
            q.pop();
            if (i < nodes.size() && nodes[i] != "null") {
                current->left = new TreeNode(std::stoi(nodes[i]));
                q.push(current->left);
            }
            i++;
            if (i < nodes.size() && nodes[i] != "null") {
                current->right = new TreeNode(std::stoi(nodes[i]));
                q.push(current->right);
            }
            i++;
        }
        return root;
    }
};

template<>
struct parsevalue<ListNode*> {
    ListNode* operator()(const std::string& str)
    {
        auto nodes = parsevalue<std::vector<int>>{}(str);
        ListNode dummy(0);
        ListNode* current = &dummy;
        for (int val : nodes) {
            current->next = new ListNode(val);
            current = current->next;
        }
        return dummy.next;
    }
};





template<typename T>
static std::remove_reference_t<T> parsevalue_wrapper(const std::string& str)
{
    return parsevalue<std::remove_reference_t<T>>{}(str);
}

template<typename... Args, std::size_t... Is>
static std::tuple<std::remove_reference_t<Args>...>
parse_tuple_helper_impl(const std::vector<std::string>& tokens, std::index_sequence<Is...>)
{
    return std::make_tuple(parsevalue_wrapper<Args>(tokens[Is])...);
}

template<typename... Args>
std::tuple<std::remove_reference_t<Args>...> parse_tuple_helper(const std::vector<std::string>& tokens)
{
    return parse_tuple_helper_impl<Args...>(tokens, std::index_sequence_for<Args...>{});
}


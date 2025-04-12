#pragma once
#include "structure.h"


template<typename T>
static T parsevalue(const std::string& str);

template<>
static int parsevalue<int>(const std::string& str) {
    return std::stoi(str);
}

template<>
static bool parsevalue<bool>(const std::string& str) {
    if (str == "true" || str == "1") return true;
    if (str == "false" || str == "0") return false;
    throw std::invalid_argument("Invalid bool string");
}

template<>
static std::string parsevalue<std::string>(const std::string& str) {
    return str;
}

template<>
static std::vector<std::string> parsevalue<std::vector<std::string>>(const std::string& str) {
    std::vector<std::string> nodes;
    std::string s = str;
    s.erase(remove(s.begin(), s.end(), '['), s.end());
    s.erase(remove(s.begin(), s.end(), ']'), s.end());
    std::istringstream iss(s);
    std::string token;
    while (getline(iss, token, ',')) {
        nodes.push_back(token);
    }
    return nodes;
}

template<>
static std::vector<int> parsevalue<std::vector<int>>(const std::string& str) {
    std::vector<int> result;
    std::string s = str;
    s.erase(remove(s.begin(), s.end(), '['), s.end());
    s.erase(remove(s.begin(), s.end(), ']'), s.end());
    std::istringstream iss(s);
    std::string token;
    while (getline(iss, token, ',')) {
        if (!token.empty()) result.push_back(std::stoi(token));
    }
    return result;
}

template<>
static TreeNode* parsevalue<TreeNode*>(const std::string& str) {
    auto nodes = parsevalue<std::vector<std::string>>(str);
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

template<>
static ListNode* parsevalue<ListNode*>(const std::string& str) {
    auto values = parsevalue<std::vector<int>>(str);
    ListNode dummy(0);
    ListNode* current = &dummy;
    for (int val : values) {
        current->next = new ListNode(val);
        current = current->next;
    }
    return dummy.next;
}


template<typename T>
static std::remove_reference_t<T> parsevalue_wrapper(const std::string& str) {
    return parsevalue<std::remove_reference_t<T>>(str);
}

template<typename... Args, std::size_t... Is>
static std::tuple<std::remove_reference_t<Args>...>
parse_tuple_helper_impl(const std::vector<std::string>& tokens, std::index_sequence<Is...>) {
    return std::make_tuple(parsevalue_wrapper<Args>(tokens[Is])...);
}

template<typename... Args>
std::tuple<std::remove_reference_t<Args>...> parse_tuple_helper(const std::vector<std::string>& tokens) {
    return parse_tuple_helper_impl<Args...>(tokens, std::index_sequence_for<Args...>{});
}


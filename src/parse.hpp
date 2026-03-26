#pragma once
#include "structure.h"



template<typename T>
struct Parse {
    Parse() {};

    static T parsevalue(const std::string& str)
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
struct Parse<std::string> {
    static std::string parsevalue(const std::string& str)
    {
        if (str.size() >= 2 && str.front() == '"' && str.back() == '"') {
            return str.substr(1, str.length() - 2);
        }
        return str;
    }
};

template<typename T>
struct Parse<std::vector<T>> {
    static std::vector<T> parsevalue(const std::string& str)
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
                    result.push_back(Parse<T>::parsevalue(elem));
                }
                start = i + 1;
            }
        }
        
        return result;
    }
};

template<>
struct Parse<TreeNode*> {
    static TreeNode* parsevalue(const std::string& str)
    {
        auto nodes = Parse<std::vector<std::string>>::parsevalue(str);
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
struct Parse<ListNode*> {
    static ListNode* parsevalue(const std::string& str)
    {
        auto nodes = Parse<std::vector<int>>::parsevalue(str);
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
std::remove_reference_t<T> parsevalue_wrapper(const std::string& str)
{
    return Parse<std::remove_reference_t<T>>::parsevalue(str);
}


#pragma once
#include "structure.h"
#include <charconv>
#include <system_error>

template<typename T>
[[noreturn]]
void throw_err(std::string_view sv) {
    std::ostringstream oss;
    oss << "Cannot parse type: " << typeid(T).name() 
        << ", value: \"" << sv << "\"";
    throw std::invalid_argument(oss.str());
}


template<typename T>
struct Parse {
    static T parsevalue(std::string_view sv)
    {
        // 默认实现，需要为每种类型特化
        T value;
        auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
        if (ec != std::errc()) {
            throw_err<T>(sv);
        }
        return value;
    }
};

template<>
struct Parse<bool> {
    static bool parsevalue(std::string_view sv)
    {
        if (sv == "true" || sv == "1")
            return true;
        if (sv == "false" || sv == "0")
            return false;
        throw_err<bool>(sv);
    }
};

template<>
struct Parse<char> {
    static char parsevalue(std::string_view sv)
    {
        if (sv.size() != 1) {
            throw_err<char>(sv);
        }
        return sv[0];
    }
};

template<>
struct Parse<std::string_view> {
    static std::string_view parsevalue(std::string_view sv)
    {
        return sv;
    }
};

template<>
struct Parse<std::string> {
    static std::string parsevalue(std::string_view sv)
    {
        if (sv.size() >= 2 && sv.front() == '"' && sv.back() == '"') {
            return std::string(sv.substr(1, sv.length() - 2));
        }
        return std::string(sv);
    }
};

template<typename T>
struct Parse<std::vector<T>> {
    static std::vector<T> parsevalue(std::string_view sv)
    {
        std::vector<T> result;
    
        if (sv.empty() || sv.front() != '[' || sv.back() != ']') {
            return result;
        }
        
        auto inner = sv.substr(1, sv.size() - 2);
        int level = 0;
        size_t start = 0;
        
        for (size_t i = 0; i <= inner.size(); ++i) {
            bool at_end = (i == inner.size());
            char c = at_end ? '\0' : inner[i];
            
            if (c == '[') level++;
            if (c == ']') level--;
            
            if (at_end || (c == ',' && level == 0)) {
                if (i > start) {
                    result.push_back(Parse<T>::parsevalue(inner.substr(start, i - start)));
                }
                start = i + 1;
            }
        }
        
        return result;
    }
};

template<>
struct Parse<TreeNode*> {
    static TreeNode* parsevalue(std::string_view sv)
    {
        auto nodes = Parse<std::vector<std::string_view>>::parsevalue(sv);
        if (nodes.empty() || nodes[0] == "null") return nullptr;
        auto root = new TreeNode(Parse<int>::parsevalue(nodes[0]));
        std::queue<TreeNode*> q;
        q.push(root);
        size_t i = 1;
        while (!q.empty() && i < nodes.size()) {
            TreeNode* current = q.front();
            q.pop();
            if (i < nodes.size() && nodes[i] != "null") {
                current->left = new TreeNode(Parse<int>::parsevalue(nodes[i]));
                q.push(current->left);
            }
            i++;
            if (i < nodes.size() && nodes[i] != "null") {
                current->right = new TreeNode(Parse<int>::parsevalue(nodes[i]));
                q.push(current->right);
            }
            i++;
        }
        return root;
    }
};

template<>
struct Parse<ListNode*> {
    static ListNode* parsevalue(std::string_view sv)
    {
        auto nodes = Parse<std::vector<int>>::parsevalue(sv);
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


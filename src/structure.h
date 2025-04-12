#pragma once
#include <math.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

// STL
#include <map>            //图
#include <unordered_map>  //
#include <stack>  //栈
#include <deque>  //双队列
#include <queue>  //队列
#include <set>            //集合
#include <unordered_set>  //
#include <list>    //列表
#include <vector>  //向量
#include <algorithm>   //算法
#include <functional>  //仿函数
#include <iterator>    //迭代器
#include <numeric>     //数值
#include <utility>     //模板
#include <regex>
#include <bitset>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

struct ListNode {
    int val;
    ListNode* next;
    ListNode(): val(0), next(nullptr) {}
    ListNode(int x): val(x), next(nullptr) {}
    ListNode(int x, ListNode* next): val(x), next(next) {}
};

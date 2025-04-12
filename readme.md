# leedcode 本地编码框架
## 使用说明
```
class Solution {
    public:
        bool canPartition(vector<int> nums) {
            ...
        }
    };


int main(){
    run_case(&Solution::canPartition, {"[1,5,11,5]"});
}
```
传入string数组作为参数

## 问题说明
- 当前不支持函数参数类型是引用
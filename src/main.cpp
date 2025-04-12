#include "autoio.hpp"

using namespace std;

class Solution {
    public:
        bool canPartition(vector<int>& nums) {
            int s = reduce(nums.begin(), nums.end());
            if (s & 1) {
                return false;
            }
            s >>= 1;
            bitset<10001> f; // sum(nums[i]) / 2 <= 10000
            f[0] = 1;
            for (int x : nums) {
                f |= f << x;
                if (f[s]) return true;
            }
            return f[s]; // 判断 f 中第 s 位是否为 1
        }
    };


int main(){
    run_case(&Solution::canPartition, {"[1,5,11,5]"});
}


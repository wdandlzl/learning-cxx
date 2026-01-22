#include "../exercise.h"
#include <vector>

using udim = unsigned int;

/// @brief 计算连续存储张量的步长
/// @param shape 张量的形状
/// @return 张量每维度的访问步长
std::vector<udim> strides(std::vector<udim> const &shape) {
    std::vector<udim> strides(shape.size());
    udim current_stride = 1; // 最后一维的步长固定为1
    
    // 修复：拆分auto声明，分别推导两个迭代器（解决const/非const类型不一致问题）
    auto shape_it = shape.rbegin();
    auto stride_it = strides.rbegin();
    for (; shape_it != shape.rend() && stride_it != strides.rend(); ++shape_it, ++stride_it) {
        *stride_it = current_stride;       // 给当前维度赋值步长
        current_stride *= *shape_it;       // 计算前一维的步长（当前步长 × 当前维度长度）
    }
    return strides;
}

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    ASSERT((strides({2, 3, 4}) == std::vector<udim>{12, 4, 1}), "Make this assertion pass.");
    ASSERT((strides({3, 4, 5}) == std::vector<udim>{20, 5, 1}), "Make this assertion pass.");
    ASSERT((strides({1, 3, 224, 224}) == std::vector<udim>{150528, 50176, 224, 1}), "Make this assertion pass.");
    ASSERT((strides({7, 1, 1, 1, 5}) == std::vector<udim>{5, 5, 5, 5, 1}), "Make this assertion pass.");
    return 0;
}
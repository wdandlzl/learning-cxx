#include "../exercise.h"
#include <numeric>
// 可选：若使用std::multiplies需包含，用lambda则无需
// #include <functional>

// READ: `std::accumulate` <https://zh.cppreference.com/w/cpp/algorithm/accumulate>

int main(int argc, char **argv) {
    using DataType = float;
    int shape[]{1, 3, 224, 224};
    // TODO: 调用 `std::accumulate` 计算张量占用的字节数
    int size = std::accumulate(
        shape,                  // 输入序列起始（数组首地址）
        shape + 4,              // 输入序列结束（数组尾地址，shape共4个元素）
        1,                      // 累加初始值（乘积初始值为1）
        [](int acc, int dim) {  // 二元操作：计算各维度乘积（总元素数）
            return acc * dim;
        }
    ) * sizeof(DataType);      // 总元素数 × 单个元素字节数（float占4字节）

    ASSERT(size == 602112, "4x1x3x224x224 = 602112");
    return 0;
}
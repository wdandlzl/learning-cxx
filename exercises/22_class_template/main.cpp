#include "../exercise.h"
#include <cstring>
// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        // TODO: 填入正确的 shape 并计算 size
        // 复制shape数组，并计算总元素数（4个维度的乘积）
        for (int d = 0; d < 4; ++d) {
            shape[d] = shape_[d];
            size *= shape[d];
        }
        data = new T[size];
        std::memcpy(data, data_, size * sizeof(T));
    }
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。
    Tensor4D &operator+=(Tensor4D const &others) {
        // TODO: 实现单向广播的加法
        // 1. 校验广播合法性：others的每个维度要么等于1，要么等于当前张量的对应维度
        for (int d = 0; d < 4; ++d) {
            ASSERT(others.shape[d] == 1 || others.shape[d] == shape[d], 
                   "Broadcast shape mismatch: dimension " << d);
        }

        // 2. 计算当前张量的总元素数
        unsigned int total_elements = 1;
        for (int d = 0; d < 4; ++d) {
            total_elements *= shape[d];
        }

        // 3. 遍历当前张量的每个元素，执行广播加法
        for (unsigned int linear_idx = 0; linear_idx < total_elements; ++linear_idx) {
            // 3.1 将线性索引转换为4D坐标 (dim0, dim1, dim2, dim3)
            unsigned int coord[4];
            unsigned int temp = linear_idx;
            coord[3] = temp % shape[3]; temp /= shape[3];
            coord[2] = temp % shape[2]; temp /= shape[2];
            coord[1] = temp % shape[1]; temp /= shape[1];
            coord[0] = temp % shape[0];

            // 3.2 计算others的对应4D坐标（广播维度取0）
            unsigned int o_coord[4];
            for (int d = 0; d < 4; ++d) {
                o_coord[d] = (others.shape[d] == 1) ? 0 : coord[d];
            }

            // 3.3 将others的4D坐标转换为线性索引
            unsigned int o_linear_idx = 0;
            o_linear_idx = o_linear_idx * others.shape[0] + o_coord[0];
            o_linear_idx = o_linear_idx * others.shape[1] + o_coord[1];
            o_linear_idx = o_linear_idx * others.shape[2] + o_coord[2];
            o_linear_idx = o_linear_idx * others.shape[3] + o_coord[3];

            // 3.4 执行广播加法
            data[linear_idx] += others.data[o_linear_idx];
        }

        return *this;
    }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
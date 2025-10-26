#include <iostream>
#include <vector>
#include <chrono>

int main() {
    constexpr int N = 10'000'000;
    std::vector<int> data(N, 1);
    volatile long long sum = 0;

    // Sequential access
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i)
        sum += data[i];
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> seqTime = t2 - t1;

    // Strided access
    sum = 0;
    int stride = 16; // skip 16 elements = one cache line
    t1 = std::chrono::high_resolution_clock::now();
    for (int offset = 0; offset < stride; ++offset)
        for (int i = offset; i < N; i += stride)
            sum += data[i];
    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> stridedTime = t2 - t1;

    std::cout << "Sequential: " << seqTime.count() << " s\n";
    std::cout << "Strided: " << stridedTime.count() << " s\n";
}
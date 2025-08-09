#include <iostream>
#include <chrono>

int main()
{
    using namespace std::chrono;

    // Start measuring time
    auto start = high_resolution_clock::now();

    // One iteration (your code here)
    for (int i = 0; i < 1; ++i)
    {
        // Do something simple
        volatile int x = i * i; // volatile to prevent compiler optimization
    }

    // Stop measuring time
    auto end = high_resolution_clock::now();

    // Calculate elapsed time
    auto duration = duration_cast<nanoseconds>(end - start).count();
    std::cout << "Time for 1 iteration: " << duration << " ns\n";

    return 0;
}

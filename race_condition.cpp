#include <iostream>
#include <thread>
#include <vector>
#include <string>
using namespace std;

static int counter = 0; // GLOBAL state which needs to be consistent

void worker(int loop_len)
{
    for (int i = 1; i <= loop_len; i++)
    {
        // CRITICAL SECTION STARTS
        counter++;
        // CRITICAL SECTION
    }
    // NO MUTUAL EXCLUSION is guaranteed
}

void runThreads(int loop_len)
{
    vector<thread> threads(2);
    threads[0] = thread(worker, loop_len);
    threads[1] = thread(worker, loop_len);
    for (auto &thread : threads)
    {
        thread.join();
    }
    cout << "Expected: " << (2 * loop_len) << endl;
    cout << "Received: " << (counter) << endl;
}
int main(int argc, char *argv[])
{
    int loopLen = stoi(argv[1]);
    runThreads(loopLen);

    return 0;
}

#include "shared_array.h"
#include <iostream>
#include <thread>
#include <chrono>

void first_process(shared_array& arr) {
    size_t index = 0;
    while (true) {
        arr[index] += 1;
        std::cout << "First process updated index " << index << " to " << arr[index] << std::endl;
        index = (index + 1) % arr.get_size();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    shared_array arr("array-name", 100);
    first_process(arr);
    return 0;
}


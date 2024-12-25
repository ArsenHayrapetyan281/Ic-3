#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <iostream>
#include <cstring>

class shared_array {
private:
    int* data;
    size_t size;
    std::string name;
    sem_t* sem;

public:
    shared_array(const std::string& array_name, size_t array_size)
        : size(array_size), name(array_name) {
        int fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
        if (fd == -1) {
            perror("shm_open failed");
            exit(1);
        }
        ftruncate(fd, size * sizeof(int));
        data = (int*) mmap(NULL, size * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED) {
            perror("mmap failed");
            exit(1);
        }
        sem = sem_open((name + "_sem").c_str(), O_CREAT, 0666, 1);
        if (sem == SEM_FAILED) {
            perror("sem_open failed");
            exit(1);
        }
    }

    ~shared_array() {
        munmap(data, size * sizeof(int));
        close(shm_open(name.c_str(), O_RDWR, 0666));
        shm_unlink(name.c_str());
        sem_close(sem);
        sem_unlink((name + "_sem").c_str());
    }

    int& operator[](size_t i) {
        if (i >= size) {
            throw std::out_of_range("Index out of range");
        }
        sem_wait(sem);
        int& element = data[i];
        sem_post(sem);
        return element;
    }

    void clear() {
        sem_wait(sem);
        std::memset(data, 0, size * sizeof(int));
        sem_post(sem);
    }

    size_t get_size() const {
        return size;
    }
};

#endif


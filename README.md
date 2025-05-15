# C++ Concurrency Simulations 👩🏻‍💻☕
Welcome! This repository contains a collection of C++ programs developed to explore and simulate different concurrency scenarios using both threads (pthread and <thread>) and processes (fork()), along with mechanisms like mutexes and shared memory.

These programs were created with academic and experimental purposes in mind. Each simulation showcases how different concurrency models behave, especially when dealing with shared data and potential race conditions. They're great for understanding the challenges and tools available when writing concurrent code in C++.

## 🧵 Basic Thread Examples

- hilos.cpp <br>
Launches three threads, each printing its own message with a specific delay and number of repetitions. It’s a minimal and friendly example to get started with std::thread

- hilos-arreglo.cpp<br>
Similar to hilos.cpp, but this time the threads are managed using an array. Demonstrates how to work with multiple threads in a more scalable way.

## 🌀 Concurrency Without Protection

- programa-sinmutex.cpp<br>
Spawns 15 threads that each print messages multiple times, with random delays and no synchronization. This version intentionally omits mutexes to highlight how console outputs can become interleaved when threads aren’t synchronized.

## 🔐 Mutex-Protected Concurrency

- programa-conmutex.cpp <br>
A refined version of the above program using a std::mutex to protect the output section. It prevents message mixing and ensures cleaner, thread-safe console output.

## 🔄 Advanced Concurrency Simulators

- concurrencia-hilos.cpp <br>
Simulates two types of threads: one that increments a shared variable and another that reads it. A mutex is used to avoid race conditions. Each thread sleeps for a random duration before executing its operation.

- concurrencia-procesos.cpp <br>
Simulates concurrency using processes (fork()), where each process has its own copy of a variable. Demonstrates the lack of shared memory and how this affects data sharing between processes.

- concurrencia-procesos-memoriacomp.cpp <br>
An improved version of the previous program that uses shared memory (mmap) to allow processes to increment and read a common variable. This version provides a realistic approach to inter-process communication and shared state.


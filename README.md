# 🚀 Parallel For Framework (C++ with pthreads + lambdas)

This project implements a **simple, flexible multithreaded loop execution framework** in C++ using **POSIX threads (`pthread`)** and modern **lambda functions**. It allows easy parallelization of both 1D and 2D loop constructs.

---

## 🔧 Features

- ✅ `parallel_for` for **1D** and **2D** ranges
- ✅ Pass any lambda with `int` or `int, int` as parameters
- ✅ Supports arbitrary thread count
- ✅ Built-in execution time measurement (in microseconds)
- ✅ Graceful error handling and validation
- ✅ Lambda demonstrations with capture semantics

---

## 🧠 How It Works

The framework defines:

- `parallel_for(int low, int high, function<void(int)> lambda, int nthreads)`
- `parallel_for(int low1, int high1, int low2, int high2, function<void(int, int)> lambda, int nthreads)`

Each call splits the loop range across threads and executes your lambda function in parallel.

---

// Lambda with captured variables
auto lambda1 = [x, &y]() {
    y = 5;
    std::cout << "====== Welcome to Assignment-" << y << " of the CSE231(A) ======\n";
};
demonstration(lambda1);  // Demonstration of basic lambda usage

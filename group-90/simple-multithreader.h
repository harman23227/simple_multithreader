#include <pthread.h>
#include <iostream>
#include <functional>
#include <time.h>
#include <stdlib.h>
#include <cstring>

using namespace std;

int user_main(int argc, char **argv);

typedef struct {
    int low;
    int high;
    std::function<void(int)> lambda;
} thread_args;

typedef struct {
    int low1;
    int high1;
    int low2;
    int high2;
    std::function<void(int, int)> lambda;
} thread_args2;

void demonstration(std::function<void()> && lambda) {
    lambda();
}

void *thread_func_1(void *ptr) {
    if (ptr == NULL) {
        std::cerr << "Error: NULL argument passed to thread_func_1\n";
        return NULL;
    }

    thread_args *t = static_cast<thread_args *>(ptr);

    if (t == NULL) {
        std::cerr << "Error: Invalid thread_args\n";
        return NULL;
    }

    if (t->low >= t->high) {
        std::cerr << "Error: Invalid range [low, high]\n";
        return NULL;
    }

    if (!t->lambda) {
        std::cerr << "Error: Lambda function is not callable\n";
        return NULL;
    }

    for (int i = t->low; i < t->high; i++) {
        t->lambda(i);
    }

    pthread_exit(NULL);
}

void* thread_func_2(void* ptr) {
    if (ptr == NULL) {
        std::cerr << "Error: NULL argument passed to thread_func_2\n";
        return NULL;
    }

    thread_args2* args = static_cast<thread_args2*>(ptr);

    if (args == NULL) {
        std::cerr << "Error: Invalid thread_args2\n";
        return NULL;
    }

    if (args->low1 >= args->high1 || args->low2 >= args->high2) {
        std::cerr << "Error: Invalid range [low1, high1] or [low2, high2]\n";
        return NULL;
    }

    if (!args->lambda) {
        std::cerr << "Error: Lambda function is not callable\n";
        return NULL;
    }

    for (int i = args->low1; i < args->high1; i++) {
        for (int j = args->low2; j < args->high2; j++) {
            args->lambda(i, j);
        }
    }

    pthread_exit(NULL);
}

void parallel_for(int low, int high, function<void(int)>&& lambda, int nthread) {
    if (low >= high) {
        std::cerr << "Error: Invalid range [low, high]\n";
        return;
    }

    if (nthread <= 0) {
        std::cerr << "Error: Number of threads must be greater than 0\n";
        return;
    }

    struct timespec start, end;
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        perror("clock_gettime failed");
        return;
    }

    pthread_t tid[nthread];
    thread_args args[nthread];
    int chunk = (high - low) / nthread;
    int remainder = (high - low) % nthread;

    for (int i = 0; i < nthread; i++) {
        args[i].low = low + i * chunk + min(i, remainder);
        args[i].high = (i == nthread - 1) ? high : args[i].low + chunk + (i < remainder ? 1 : 0);
        args[i].lambda = lambda;
        if (pthread_create(&tid[i], NULL, thread_func_1, (void *)&args[i]) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nthread; i++) {
        if (pthread_join(tid[i], NULL) != 0) {
            perror("pthread_join failed");
            exit(EXIT_FAILURE);
        }
    }

    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) {
        perror("clock_gettime failed");
        return;
    }

    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    long duration = seconds * 1000000 + nanoseconds / 1000;

    printf("Total Execution 1D parallel_for: %ld microseconds\n", duration);
}

void parallel_for(int low1, int high1, int low2, int high2, function<void(int, int)>&& lambda, int nthread) {
    if (low1 >= high1 || low2 >= high2) {
        std::cerr << "Error: Invalid range [low1, high1] or [low2, high2]\n";
        return;
    }

    if (nthread <= 0) {
        std::cerr << "Error: Number of threads must be greater than 0\n";
        return;
    }

    struct timespec start, end;
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        perror("clock_gettime failed");
        return;
    }

    pthread_t tid[nthread];
    thread_args2 args[nthread];
    int chunk = (high1 - low1) / nthread;
    int remainder = (high1 - low1) % nthread;

    for (int i = 0; i < nthread; i++) {
        args[i].low1 = low1 + i * chunk + min(i, remainder);
        args[i].high1 = (i == nthread - 1) ? high1 : args[i].low1 + chunk + (i < remainder ? 1 : 0);
        args[i].low2 = low2;
        args[i].high2 = high2;
        args[i].lambda = lambda;
        if (pthread_create(&tid[i], NULL, thread_func_2, (void *)&args[i]) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nthread; i++) {
        if (pthread_join(tid[i], NULL) != 0) {
            perror("pthread_join failed");
            exit(EXIT_FAILURE);
        }
    }

    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) {
        perror("clock_gettime failed");
        return;
    }

    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    long duration = seconds * 1000000 + nanoseconds / 1000;

    printf("Total Execution Time 2D parallel_for: %ld microseconds\n", duration);
}

int main(int argc, char **argv) {
  /* 
   * Declaration of a sample C++ lambda function
   * that captures variable 'x' by value and 'y'
   * by reference. Global variables are by default
   * captured by reference and are not to be supplied
   * in the capture list. Only local variables must be 
   * explicity captured if they are used inside lambda.
   */
  int x=5,y=1;
  // Declaring a lambda expression that accepts void type parameter
  auto /*name*/ lambda1 = /*capture list*/[/*by value*/ x, /*by reference*/ &y](void) {
    /* Any changes to 'x' will throw compilation error as x is captured by value */
    y = 5;
    std::cout<<"====== Welcome to Assignment-"<<y<<" of the CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  // Executing the lambda function
  demonstration(lambda1); // the value of x is still 5, but the value of y is now 5

  int rc = user_main(argc, argv);
 
  auto /*name*/ lambda2 = [/*nothing captured*/]() {
    std::cout<<"====== Hope you enjoyed CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  demonstration(lambda2);
  return rc;
}
#define main user_main

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#define MILL 1000000000LL
#define TIMES 20

void fill(std::vector<long long>& vec1,long long AMOUNT) {
    for (long long i = 0; i < AMOUNT; i++) {
        vec1[i] = i;
    }
}

long long sum_seq(std::vector<long long> &vec1) {
    long long sum1 = 0;
    for (long long i = 0; i < MILL; i++) {
        sum1 += vec1[i];
    }
    return sum1;

}

void setup_seq(long long amountWork,int times, std::vector<long long> &vec1) {
    long long result = 0;
    long long promedio = 0;
    
    for (int i = 0; i < times; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        result = sum_seq(vec1);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double>(end - start);
        promedio += duration.count();
        std::cout << "Resultado secuencial " << i << " -> " << result << " con tiempo de " << duration.count() << "s" << std::endl;
    }
    std::cout << "El promedio secuencial fue " << promedio / TIMES << "s" << std::endl;
}

void sum_par(std::vector<long long> &vec1,std::vector<long long> &threadResults, int threadID, long long start, long long end) {
    long long sum1 = 0;
    for (long long i = start; i < end; i++) {
        sum1 += vec1[i];
    }
    threadResults[threadID] = sum1;
    //std::cout << "Result from thread " << threadID << " -> " << sum1 << std::endl;
}

void setup_par(int cantThreads, long long amountWork, int times, std::vector<long long>& vec1) {
    std::vector<std::thread> threads;
    std::vector<long long> threadResults(cantThreads);
    std::vector<double> timesThreads(times);

    long long result = 0;
    double promedio = 0;

    // Resultado paralelo
    for (int m = 0;m < times;m++) {
        long long work = (long long)(amountWork / cantThreads);

        auto start = std::chrono::steady_clock::now();
        for (long long i = 0; i < cantThreads; i++) {
            threads.emplace_back(sum_par, std::ref(vec1), std::ref(threadResults), i, i * work, work + i * work);
        }
        for (auto& thread : threads) {
            thread.join();
        }
        
        for (long long i = 0;i < threadResults.size();i++) {
            result += threadResults[i];

        }
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration<double>(end - start);
        timesThreads[m] = duration.count();
        
        //std::cout << "Resultado paralelo es  " << result << " y tardo " << duration.count() << "ms" << std::endl;

        threads.clear();
        result = 0;
    }

    for (int i = 0;i < timesThreads.size();i++) {
        promedio += timesThreads[i];
    }
    std::cout << "El promedio paralelo para cantThreads=" << cantThreads << " es " << promedio / timesThreads.size() << std::endl;
}

int main() {
    long long CANT_THREAD = 2;
    std::cout << "LA CANTIDAD DE THREADS DISPONIBLE " << CANT_THREAD << std::endl;
    std::vector<long long> vec1(MILL);
    
    fill(vec1, MILL);

    // Pruebas paralelo
    setup_par(1, MILL, TIMES, vec1);
    setup_par(2, MILL, TIMES, vec1);
    setup_par(4, MILL, TIMES, vec1);
    setup_par(8, MILL, TIMES, vec1);
    setup_par(16, MILL, TIMES, vec1);
    setup_par(32, MILL, TIMES, vec1);

    setup_seq(MILL, TIMES, vec1);
    
   


}


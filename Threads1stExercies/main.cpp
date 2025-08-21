#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <random>
#include <algorithm>  
#include <utility>    

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distrib(1, 1000);

class Summation {
    int total;

public:
    Summation() : total(0) {}

    void doSum() {
        for (int i = 0; i < 100; i++) {
            total += distrib(gen);
        }
    }

    int Total() const {
        return total;
    }
};

int main() {
    const int TOTAL_THREADS = 10;

    std::vector<std::shared_ptr<std::thread>> threads(TOTAL_THREADS);
    std::vector<std::shared_ptr<Summation>> summations(TOTAL_THREADS);

    for (int i = 0; i < TOTAL_THREADS; i++) {
        summations[i] = std::make_shared<Summation>();
        threads[i] = std::make_shared<std::thread>([&, i](){ summations[i]->doSum(); });
    }

    for (int i = 0; i < TOTAL_THREADS; i++) {
        threads[i]->join();
    }

   
    std::vector<std::pair<int,int>> results; 
    results.reserve(TOTAL_THREADS);

    for (int i = 0; i < TOTAL_THREADS; i++) {
        std::cout << "El thread #" << i + 1 << " sumo: " << summations[i]->Total() << std::endl;
        results.emplace_back(summations[i]->Total(), i);
    }

    std::sort(results.begin(), results.end(),
              [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
                  return a.first > b.first; 
              });

    int maxSum = results.front().first;
    int maxId  = results.front().second;

    std::cout << "El thread con mayor puntuacion fue el #" << maxId + 1
              << " y sumo: " << maxSum << std::endl;

    return 0;
}

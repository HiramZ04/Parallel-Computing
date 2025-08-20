#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <random>

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
        threads[i] = std::make_shared<std::thread>([&, i](){summations[i]->doSum();});
    }

    for (int i = 0; i < TOTAL_THREADS; i++) {
        threads[i]->join();
    }

    int maxSum = 0;
    int maxId;

    for (int i = 0; i < TOTAL_THREADS; i++) {
        std::cout << "El thread #" << i + 1 << " sumo: " << summations[i]->Total() << std::endl;

        if (summations[i]->Total() > maxSum) {
            maxSum = summations[i]->Total();
            maxId = i;
        }
    }

    std::cout << "El thread con mayor puntuacion fue el #" << maxId + 1 << " y sumo: " << summations[maxId]->Total() << std::endl;

    return 0;
}
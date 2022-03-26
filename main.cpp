#include <iostream>

#include "include/blab/environment.h"
#include "include/blab/meter.h"
#include "include/blab/report.h"
#include "include/blab/experiment.h"

int subject1(int in) {
    auto e = blab::static_environment::instance();

    auto &swaps = e->create_meter<blab::quantity_meter>("Swaps");
    auto &comparisons = e->create_meter<blab::quantity_meter>("Comparisons");
    auto &extime = e->create_meter<blab::execution_time_meter>("Ex. time");

    extime.start();

    ++swaps;
    comparisons += 5;
    
    extime.stop();

    return 0;
}

int main() {
    blab::experiment<int, int> experiment([](int a) -> bool {
                return a == 0;
            }, 0);

    experiment.add_subject("subject 1", subject1);

    std::cout << experiment.perform() << std::endl;

    return 0;
}

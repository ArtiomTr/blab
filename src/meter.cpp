#include "../include/blab/meter.h"

#include "../include/blab/not_implemented.h"

namespace blab {
    meter::~meter() = default;

    meter::meter() = default;

    std::unique_ptr<metric> meter::result() const {
        throw not_implemented("result");
    }

    quantity_meter::quantity_meter() : quantity(0) {

    }

    void quantity_meter::operator+=(uint32_t value) {
        if (value == 0) {
            return;
        }

        uint32_t new_value = quantity + value;

        if (new_value <= quantity) {
            throw std::out_of_range("Quantity meter value exceeded it's limit (2^32-1)");
        }

        quantity = new_value;
    }

    void quantity_meter::operator++() {
        *this += 1;
    }

    std::unique_ptr<metric> quantity_meter::result() const {
        return std::unique_ptr<metric>(new uint32_metric(quantity));
    }

    execution_time_meter::execution_time_meter() : tp_begin(nullptr), tp_end(nullptr) {

    }

    void execution_time_meter::start() {
        using namespace std::chrono;

        if (tp_begin != nullptr) {
            throw std::logic_error("Execution time meter failure: timer already started.");
        }

        tp_begin = std::make_unique<time_point<steady_clock>>(steady_clock::now());
    }

    void execution_time_meter::stop() {
        using namespace std::chrono;
        using std::logic_error;

        if (tp_begin == nullptr) {
            throw logic_error("Execution time meter failure: trying to stop timer that not started.");
        }

        if (tp_end != nullptr) {
            throw logic_error("Execution time meter failure: trying to stop timer already stopped timer.");
        }

        tp_end = std::make_unique<time_point<steady_clock>>(steady_clock::now());
    }

    std::unique_ptr<metric> execution_time_meter::result() const {
        using std::logic_error;

        if (tp_begin == nullptr) {
            throw logic_error("Execution time meter failure: trying to access result of non started timer.");
        }

        if (tp_end == nullptr) {
            throw logic_error("Execution time meter failure: trying to access result while timer is not stopped yet.");
        }

        std::chrono::duration<int64_t, std::nano> duration = *tp_end - *tp_begin;
        return std::unique_ptr<metric>(new time_metric(duration));
    }
}

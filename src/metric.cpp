#include "../include/blab/metric.h"

#include <sstream>
#include <array>

#include "../include/blab/not_implemented.h"

namespace blab {
    using std::string;

    string metric::to_string() const {
        throw not_implemented("to_string");
    }

    uint32_metric::uint32_metric(uint32_t initial_value) : value(initial_value) {
    }

    string uint32_metric::to_string() const {
        using std::stringstream;

        stringstream string_builder{};
        string_builder << value;

        return string_builder.str();
    }

    time_metric::time_metric(std::chrono::nanoseconds initial_duration) : duration(initial_duration) {

    }

    string time_metric::to_string() const {
        using namespace std::chrono;
        using std::stringstream;

        auto display_duration = duration;

        auto hour_count = duration_cast<hours>(display_duration);
        display_duration -= hour_count;
        auto minute_count = duration_cast<minutes>(display_duration);
        display_duration -= minute_count;
        auto second_count = duration_cast<seconds>(display_duration);
        display_duration -= second_count;
        auto millisecond_count = duration_cast<milliseconds>(display_duration);
        display_duration -= millisecond_count;
        auto microsecond_count = duration_cast<microseconds>(display_duration);
        display_duration -= microsecond_count;
        auto nanosecond_count = duration_cast<nanoseconds>(display_duration);

        stringstream string_builder{};

        std::array<std::pair<int64_t, string>, 6> values = {{
                                                                         {hour_count.count(), "h"},
                                                                         {minute_count.count(), "m"},
                                                                         {second_count.count(), "s"},
                                                                         {millisecond_count.count(), "ms"},
                                                                         {microsecond_count.count(), "us"},
                                                                         {nanosecond_count.count(), "ns"}
                                                                 }};
        bool begin = false;

        for (const auto &[value, prefix]: values) {
            if (value != 0) {
                begin = true;
            }

            if (begin || value != 0) {
                string_builder << value << prefix << " ";
            }
        }

        return string_builder.str();
    }

    bool_metric::bool_metric(bool result) : result(result) {

    }

    string bool_metric::to_string() const {
        return result ? "true" : "false";
    }
}
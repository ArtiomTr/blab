#include "../include/blab/environment.h"

namespace blab {
    void environment::add_meter(const std::string &tag, std::unique_ptr<meter> new_meter) {
        named_meter m(tag, std::move(new_meter));
        meters.push_back(std::move(m));
    }

    std::vector<std::pair<std::string, std::unique_ptr<metric>>> environment::get_metrics() {
        std::vector<std::pair<std::string, std::unique_ptr<metric>>> metrics;
        
        for (const auto &[name, meter]: meters) {
            metrics.emplace_back(name, meter->result());
        }
        
        return metrics;
    }

    void environment::clear() {
        meters.clear();
    }
    
    environment *static_environment::env = nullptr;

    void static_environment_cleanup() {
        delete static_environment::env;
        static_environment::env = nullptr;
    }

    environment *static_environment::instance() {
        if (env == nullptr) {
            env = new environment();
            atexit(static_environment_cleanup);
        }

        return env;
    }
}

#ifndef BLAB_EXPERIMENT_H
#define BLAB_EXPERIMENT_H

#include <vector>
#include <functional>

#include "report.h"
#include "environment.h"

namespace blab {
    template<class Subject>
    class experiment {
    private:
        std::vector<std::pair<std::string, Subject>> subjects;
        std::function<bool(Subject)> tester;
    public:
        explicit experiment(std::function<bool(Subject)> tester) : subjects({}), tester(tester) {
        }

        void add_subject(const std::string &name, Subject subject) {
            subjects.push_back({name, subject});
        }

        report perform() {
            report result_report{};

            blab::static_environment::instance()->clear();
            for (const auto &[name, subject]: subjects) {
                bool result = tester(subject);

                auto metrics = blab::static_environment::instance()->get_metrics();

                std::unique_ptr<metric> success_metric(new bool_metric(result));

                metrics.push_back({"Success?", std::move(success_metric)});

                result_report.add_row(name, metrics);
            }

            return result_report;
        }
    };
}

#endif

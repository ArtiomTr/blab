#include "../include/blab/report.h"

#include <sstream>
#include <iomanip>

namespace blab {
    using std::string;
    using std::vector;
    using std::size_t;

    void add_metric(const string &metric_name,
                    const string &metric_value,
                    vector<string> &new_row,
                    vector<string> &titles,
                    vector<size_t> &column_width) {
        auto it = std::find(titles.begin(), titles.end(), metric_name);

        if (it != titles.end()) {
            size_t index = it - titles.begin();
            new_row[index] = metric_value;
            column_width[index] = std::max(column_width[index], metric_value.length());
        } else {
            titles.push_back(metric_name);
            column_width.push_back(std::max(metric_name.length(), metric_value.length()));
            new_row.push_back(metric_value);
        }
    }

    void report::add_row(const string &name, const std::vector<named_metric> &metrics) {
        vector<string> new_row;
        new_row.resize(titles.size());
        std::fill(new_row.begin(), new_row.end(), "-");

        add_metric("Name", name, new_row, titles, column_width);
        for (const auto &[metric_name, metric_value]: metrics) {
            add_metric(metric_name, metric_value->to_string(), new_row, titles, column_width);
        }

        rows.push_back(new_row);
    }

    std::ostream &operator<<(std::ostream &output, const report &r) {
        using std::left;
        using std::setfill;
        using std::setw;

        output << left << setfill(' ');

        for (size_t i = 0; i < r.titles.size(); ++i) {
            output << " | " << setw((int) r.column_width[i]) << r.titles[i];
        }
        output << '\n';

        output << setfill('-');
        for(size_t width : r.column_width) {
            output << " | " << setw((int) width) << "";
        }
        output << '\n';

        output << setfill(' ');

        for (auto const &value: r.rows) {
            for (size_t i = 0; i < r.column_width.size(); ++i) {
                output << " | " << setw((int) r.column_width[i]) << (i >= value.size() ? "-" : value[i]);
            }
            output << '\n';
        }

        return output;
    }

    string report::to_csv() const {
        std::stringstream csv;

        for (const auto & title : titles) {
            csv << title << ';';
        }
        csv << '\n';

        for (auto const &value: rows) {
            for (size_t i = 0; i < titles.size(); ++i) {
                csv << (i >= value.size() ? "-" : value[i]) << ';';
            }
            csv << '\n';
        }

        return csv.str();
    }
}

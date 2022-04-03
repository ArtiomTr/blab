# blab 🥼

Homemade benchmarking laboratory for C++. Made for learning

## Main concepts

Benchmarking is a process, of evaluating relative performance of program. Subjects (programs/subprograms) are placed in
a test environment in which various indicators are measured. All subjects are being tested on same input data. They
should produce output data, that could be easily validated via `checker` function.

## Process

1. Input dataset is being generated. The input dataset **must** be copiable C++ object.
2. Subjects are added to the experiment.
3. Evaluating all subjects. Each subject creates `meters`, before beginning evaluation. More information about meters
   is [here](#meters).
4. While computing output, subject can trigger different `meters`.
5. After processing is complete, [test environment](#environment) is cleared, [metrics](#metrics) are collected.

## Experiment

To create new experiment, you should pass `InputType`, `OutputType`, `checker` and `dataset`:

```cpp
  blab::experiment<InputType, OutputType> experiment(checker, dataset);
```

For instance, if you would like to test sorting algorithms, that's how experiment should be initialized:

```cpp
#include <vector>

#include "blab/experiment.h"

using namespace std;

// Function, which validates output of sorting function.
bool isArraySorted(vector<int> list) {
  // Base condition - an empty list is interpreted as sorted.
  if(list.size == 0) {
    return true;
  }

  for(size_t i = 0; i < list.size() - 1; ++i) {
    if(list[i] < list[i + 1]) {
      return false;
    }
  }
  
  return true;
}

int main() {
  // Input dataset: list, that will be sorted.
  vector<int> input{4, 3, 5, 1, 2};
  blab::experiment<vector<int>, vector<int>> experiment(isArraySorted, input);
}
```

## Subjects

After experiment is created, you can easily add subjects to it:

```cpp
experiment.add_subject("Subject name", subject);
```

The subject is function, that receives `InputType` as argument, and returns `OutputType`. Returned output will be
validated via `checker` function.

## Meters

Meter is object, that inherits from `blab::meter` base class. It should override `result` function, that
returns [metric](#metrics). Meter should be `friend` to `blab::experiment` class, and should have private constructor, to
prevent invalid usage.

Example of creating meter:

```cpp
#include "blab/environment.h"
#include "blab/meter.h"

int fn() {
    blab::environment *env = blab::static_environment::instance();
    
    blab::quantity_meter &meter = env->create_meter<blab::quantity_meter>("Count");
}
```

**Builtin meters:**

| Name                 | Description | 
| -------------------- | ----------- | 
| blab::quantity_meter | Meter for counting operations (read/write for example). Available operations: preincrement (++), addition (+= unsigned). Throws error when count is bigger, than max value available (2^32-1). | 
| blab::time_meter     | Meter for measuring execution time. Available operations: start(), stop(). Throws error if trying to access result when timer is not started or stopped. |

## Metrics

Metric is object, that inherits from `blab::metric` base class. It holds measured value, and should override `to_string`
function, that returns `std::string`.

**Builtin metrics:**

| Name                | Description                                                                |
| ------------------- | -------------------------------------------------------------------------- |
| blab::uint32_metric | Saves uint32_t value. Prints single number                                 |
| blab::time_metric   | Saves std::chrono::nanoseconds value. Prints time in human-readable format |
| blab::bool_metric   | Saves bool value. Prints "true" / "false" string, depending on value       |

## Environment

Testing environment is object, which stores all meters. `blab::static_environment` is a singleton instance of 
environment, which is used for the most of the tests.

## Report

After performing `blab::experiment`, the report object is returned. Report could be printed in markdown-like format to
any `std::ostream` via `<<` operator, or, into CSV format via `to_csv` function.

```cpp
blab::report report = experiment.perform();

// Markdown-like format
std::cout << report << std::endl;

std::ofstream output("out.csv");

// CSV format
std::cout << report.to_csv() << std::endl;
```

## License

MIT © [Artiom Tretjakovas](https://github.com/ArtiomTr)

#include "../include/blab/not_implemented.h"

namespace blab {
    not_implemented::not_implemented(const std::string &method_name) :
            logic_error("Method \"" + method_name + "\" is not implemented yet.") {
    }
}

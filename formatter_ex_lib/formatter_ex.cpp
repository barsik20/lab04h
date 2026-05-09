#include "formatter_ex.h"
#include "formatter.h"

std::string formatter_ex(const std::string& message) {
    return formatter("[EX] " + message);
}

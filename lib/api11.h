#ifndef API11_H
#define API11_H

#include "LCard_global.h"
#include <string>

namespace api11 {
LCARD_EXPORT void start(int argc, const char *argv[], void (*callback)(int, const std::string &, double *data));
LCARD_EXPORT void stop(void (*callback)(int, const std::string&, double *data));
LCARD_EXPORT void f_abort_handler(int sig);
};

#endif // API11_H

#ifndef PYBOOSTCONVERTER_HPP_SL
#define PYBOOSTCONVERTER_HPP_SL

#include <string>

#include "PyBoostCvMatConverter.hpp"
#include "PyBoostListConverter.hpp"
#include "PyBoostTupleConverter.hpp"

namespace pbcvt {

static void export_cpptuple_conv() {
    create_tuple_converter<int, float>();
    create_tuple_converter<int, double, std::string>();
}

static void initPyBindings() {
    Py_Initialize();
    export_cpptuple_conv();
}

} // end namespace pbcvt

#endif
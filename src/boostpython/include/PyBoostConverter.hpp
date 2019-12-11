#ifndef PYBOOSTCONVERTER_HPP_SL
#define PYBOOSTCONVERTER_HPP_SL

#include <variant>
#include <string>

#include "PyBoostCvMatConverter.hpp"
#include "PyBoostListConverter.hpp"
#include "PyBoostTupleConverter.hpp"
#include "PyBoostDictConverter.hpp"

#include <boost/python/init.hpp>
#include <boost/python/class.hpp>
#include <boost/python/class_fwd.hpp>

namespace pbcvt {

static void export_cpptuple_conv() {
    create_tuple_converter<int, float>();
    create_tuple_converter<int, double, std::string>();
    create_tuple_converter<int, double, cv::Mat>();
}

static void initPyBindings() {

    py::type_info info = py::type_id<cv::Mat>(); 
    const py::converter::registration* reg = py::converter::registry::query(info); 
    if (reg != NULL && reg->m_to_python != NULL) {
        return;
    }
    cout << "Initialising Python bindings!" << endl;

    Py_Initialize();


    // Bindings for CV::Mat
    py::to_python_converter<cv::Mat, matToNDArrayBoostConverter>();
    py::converter::registry::push_back(
        &matFromNDArrayBoostConverter::convertible,
        &matFromNDArrayBoostConverter::construct,
        py::type_id<cv::Mat>());

    export_cpptuple_conv();

    import_array();
}

} // end namespace pbcvt

#endif
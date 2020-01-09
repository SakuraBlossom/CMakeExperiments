#ifndef PYBOOSTCONVERTER_HPP_DICT_SL
#define PYBOOSTCONVERTER_HPP_DICT_SL

#include <cstdio>
#include <vector>
#include <memory>

#include "PyBoostConfig.h"

namespace pbcvt {

namespace py = boost::python;
using namespace std;

//===================   LIST CONVERTER FUNCTIONS     ===============================================

template<typename T>
inline std::vector< T > py_list_to_std_vector(PyObject *list) {

    py::handle<> handle(py::borrowed(list));
    py::object iterable(handle);
    return std::vector<T>( py::stl_input_iterator<T>(iterable),
                             py::stl_input_iterator<T>());
}

template <class T>
inline PyObject* std_vector_to_py_list(std::vector<T> vector) {
    py::list list;
    for (auto iter = vector.begin(); iter != vector.end(); ++iter) {
        list.append(*iter);
    }
    return py::incref(list.ptr());
}

} // end namespace pbcvt

#endif

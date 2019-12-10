#ifndef PYBOOSTCONVERTER_HPP_LIST_SL
#define PYBOOSTCONVERTER_HPP_LIST_SL

#include <cstdio>
#include <vector>
#include <memory>

#include <Python.h>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/python/stl_iterator.hpp>

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
    typename std::vector<T>::iterator iter;
    py::list list;
    for (iter = vector.begin(); iter != vector.end(); ++iter) {
        list.append(*iter);
    }
    return py::incref(list.ptr());
}

template <class T>
inline PyObject* std_vector_to_py_list(std::vector<T> vector, PyObject* (*func)(const T&)) {
    typename std::vector<T>::iterator iter;
    py::list list;
    for (iter = vector.begin(); iter != vector.end(); ++iter) {
        py::handle<> handle(py::borrowed(func(*iter)));
        py::object iterable(handle);
        list.append(iterable);
    }
    return py::incref(list.ptr());
}

} // end namespace pbcvt

#endif

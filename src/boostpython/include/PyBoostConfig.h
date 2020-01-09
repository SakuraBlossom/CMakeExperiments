#ifndef PYBOOST_CONFIG_SL
#define PYBOOST_CONFIG_SL
#include <iostream>

#include <numpy/ndarrayobject.h>

#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/python/stl_iterator.hpp>

#include <boost/python/init.hpp>
#include <boost/python/class.hpp>
#include <boost/python/class_fwd.hpp>

#include <boost/python/scope.hpp>
#include <boost/python/object/add_to_namespace.hpp>

#define NUMPY_IMPORT_ARRAY_RETVAL

namespace py = boost::python;
using namespace std;

#if 0
typedef struct PyModuleDef_Base {
  PyObject_HEAD
  PyObject* (*m_init)(void);
  Py_ssize_t m_index;
  PyObject* m_copy;
};

typedef struct PyModuleDef{
  PyModuleDef_Base m_base;
  const char* m_name;
  const char* m_doc;
  Py_ssize_t m_size;
  PyMethodDef *m_methods;
  struct PyModuleDef_Slot* m_slots;
  traverseproc m_traverse;
  inquiry m_clear;
  freefunc m_free;
};

namespace boost { namespace python { namespace detail {
    extern "C" PyObject* init_module(PyModuleDef& moduledef, void(*init_function)());

}}}

#   define _BOOST_PYTHON_MODULE_INIT3(name) \
  PyObject* BOOST_PP_CAT(PyInit_, name)()  \
  { \
    static PyModuleDef_Base initial_m_base = { \
        PyObject_HEAD_INIT(NULL) \
        0, /* m_init */ \
        0, /* m_index */ \
        0 /* m_copy */ };  \
    static PyMethodDef initial_methods[] = { { 0, 0, 0, 0 } }; \
 \
    static struct PyModuleDef moduledef = { \
        initial_m_base, \
        BOOST_PP_STRINGIZE(name), \
        0, /* m_doc */ \
        -1, /* m_size */ \
        initial_methods, \
        0,  /* m_reload */ \
        0, /* m_traverse */ \
        0, /* m_clear */ \
        0,  /* m_free */ \
    }; \
 \
    std::cout << "Initialising Phase 1" << std::endl;   \
    return boost::python::detail::init_module( \
        BOOST_PP_STRINGIZE(name),&BOOST_PP_CAT(init_module_,name)); \
  }

#   define BOOST_PYTHON_MODULE3(name)                               \
extern "C" __attribute__ ((__visibility__("default"))) _BOOST_PYTHON_MODULE_INIT3(name)

#  if PY_VERSION_HEX >= 0x03000000

#warning "PYTHON 3 TARGET"

#else

#warning "PYTHON 2 TARGET"

#endif
#endif

#endif

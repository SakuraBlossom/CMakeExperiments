#ifndef CVBOOSTCONVERTER_HPP_SL
#define CVBOOSTCONVERTER_HPP_SL

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include <cstdio>
#include <vector>
#include <memory>
#include <string>

#include <Python.h>
#include <numpy/ndarrayobject.h>
#include <opencv2/core/core.hpp>

#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/python/stl_iterator.hpp>

namespace pbcvt {

namespace py = boost::python;
using namespace cv;
using namespace std;


static PyObject* opencv_error = 0;

//===================    MACROS    =================================================================
#define ERRWRAP2(expr) \
try \
{ \
    PyAllowThreads allowThreads; \
    expr; \
} \
catch (const cv::Exception &e) \
{ \
    PyErr_SetString(opencv_error, e.what()); \
    return 0; \
}

//===================   ERROR HANDLING     =========================================================

static int failmsg(const char *fmt, ...) {
	char str[1000];

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(str, sizeof(str), fmt, ap);
	va_end(ap);

	PyErr_SetString(PyExc_TypeError, str);
	return 0;
}

//===================   THREADING     ==============================================================
class PyAllowThreads {
public:
	PyAllowThreads() :
			_state(PyEval_SaveThread()) {
	}
	~PyAllowThreads() {
		PyEval_RestoreThread(_state);
	}
private:
	PyThreadState* _state;
};

class PyEnsureGIL {
public:
	PyEnsureGIL() :
			_state(PyGILState_Ensure()) {
	}
	~PyEnsureGIL() {
		PyGILState_Release(_state);
	}
private:
	PyGILState_STATE _state;
};

enum {
	ARG_NONE = 0, ARG_MAT = 1, ARG_SCALAR = 2
};

static size_t REFCOUNT_OFFSET = (size_t)&(((PyObject*)0)->ob_refcnt) +
    (0x12345678 != *(const size_t*)"\x78\x56\x34\x12\0\0\0\0\0")*sizeof(int);

static inline PyObject* pyObjectFromRefcount(const int* refcount) {
    return (PyObject*)((size_t)refcount - REFCOUNT_OFFSET);
}

static inline int* refcountFromPyObject(const PyObject* obj) {
    return (int*)((size_t)obj + REFCOUNT_OFFSET);
}

//===================   NUMPY ALLOCATOR FOR OPENCV     =============================================

class NumpyAllocator;

//===================   STANDALONE CONVERTER FUNCTIONS     =========================================

PyObject* fromMatToNDArray(const Mat& m);
Mat fromNDArrayToMat(PyObject* o);


//===================   LIST CONVERTER FUNCTIONS     ===============================================

template<typename T>
inline std::vector< T > py_list_to_std_vector(PyObject *list) {

    py::handle<> handle(py::borrowed(list));
    py::object iterable(handle);
    return std::vector< T >( py::stl_input_iterator< T >(iterable),
                             py::stl_input_iterator< T >());
}

template <class T>
inline PyObject* std_vector_to_py_list(std::vector<T> vector) {
    typename std::vector<T>::iterator iter;
    py::list list;
    for (iter = vector.begin(); iter != vector.end(); ++iter) {
        list.append(*iter);
    }
    Py_INCREF(list.ptr());
    return list.ptr();
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
    Py_INCREF(list.ptr());
    return list.ptr();
}

//===================   BOOST OPENCV MAT CONVERTERS     ============================================

struct matToNDArrayBoostConverter {
	static PyObject* convert(Mat const& m);
};

struct matFromNDArrayBoostConverter {

	matFromNDArrayBoostConverter();

	/// @brief Check if PyObject is an array and can be converted to OpenCV matrix.
	static void* convertible(PyObject* object);

	/// @brief Construct a Mat from an NDArray object.
	static void construct(PyObject* object,
			py::converter::rvalue_from_python_stage1_data* data);
};


//===================   BOOST TUPLE CONVERTERS     =================================================

template<int ...> struct seq{};
template<int N, int ...S> struct gens : gens<N-1, N-1, S...>{};
template<int ...S> struct gens<0, S...> {typedef seq<S...> type;};

template <typename ...Args>
struct cpptuple2pytuple_wrapper {
    std::tuple<Args...> params;
    cpptuple2pytuple_wrapper(const std::tuple<Args...>& _params):params(_params){}

    py::tuple delayed_dispatch() {
        return callFunc(typename gens<sizeof...(Args)>::type());
    }

    template<int ...S>
    py::tuple callFunc(seq<S...>) {
        return py::make_tuple(std::get<S>(params) ...);
    }
};

template <typename ...Args>
struct pytuple2cpptuple_wrapper {
    py::tuple params;
    pytuple2cpptuple_wrapper(const py::tuple& _params):params(_params){}

    std::tuple<Args...> delayed_dispatch() {
        return callFunc(typename gens<sizeof...(Args)>::type());
    }

    template<int ...S>
    std::tuple<Args...> callFunc(seq<S...>) {
        return std::make_tuple((static_cast<Args>(py::extract<Args>(params[S])))...);
    }
};

// Convert (C++) tuple to (Python) tuple as PyObject*.
template<typename ... Args> PyObject* cpptuple2pytuple(const std::tuple<Args...>& t) {
    cpptuple2pytuple_wrapper<Args...> wrapper(t);
    py::tuple bpt = wrapper.delayed_dispatch();
    return py::incref(py::object(bpt).ptr());
}

// Convert (Python) tuple to (C++) tuple.
template<typename ... Args> std::tuple<Args...> pytuple2cpptuple(PyObject* obj) {
    py::tuple tup(py::borrowed(obj));
    pytuple2cpptuple_wrapper<Args...> wrapper(tup);
    std::tuple<Args...> bpt = wrapper.delayed_dispatch();
    return bpt;
}

template<typename ... Args>
struct cpptuple_to_python_tuple {
    static PyObject* convert(const std::tuple<Args...>& t) {
        return cpptuple2pytuple<Args...>(t);
    }
};

template<typename ... Args>
struct cpptuple_from_python_tuple {
    cpptuple_from_python_tuple() {
        py::converter::registry::push_back(&convertible, &construct, py::type_id<std::tuple<Args...> >());
    }

    static void* convertible(PyObject* obj_ptr) {
        if (!PyTuple_CheckExact(obj_ptr)) return 0;
        return obj_ptr;
    }

    static void construct(PyObject* obj_ptr, py::converter::rvalue_from_python_stage1_data* data) {
        void* storage = ((py::converter::rvalue_from_python_storage<std::tuple<Args...> >*)data)->storage.bytes;
        new (storage) std::tuple<Args...>(pytuple2cpptuple<Args...>(obj_ptr));
        data->convertible = storage;
    }
};

template<typename ...Args> void create_tuple_converter() {
    py::to_python_converter<std::tuple<Args...>, cpptuple_to_python_tuple<Args...> >();
    cpptuple_from_python_tuple<Args...>();
}

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

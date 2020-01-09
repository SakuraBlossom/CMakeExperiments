#ifndef CVBOOSTCONVERTER_HPP_SL
#define CVBOOSTCONVERTER_HPP_SL

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include <cstdio>
#include <vector>
#include <memory>
#include <string>

#include "PyBoostConfig.h"

#include <opencv2/core/core.hpp>

namespace pbcvt {

namespace py = boost::python;
using namespace cv;

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


} // end namespace pbcvt

#endif

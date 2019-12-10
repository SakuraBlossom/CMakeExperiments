#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/python.hpp>

#include "conversion.h"

namespace py = boost::python;
using namespace std;

typedef unsigned char uchar_t;


/**
 * Displays an image, passed in from python as an ndarray.
 */
void
display(PyObject *img)
{
    NDArrayConverter cvt;
    cv::Mat mat { cvt.toMat(img) };
    
    cv::namedWindow("display", CV_WINDOW_NORMAL);
    cv::imshow("display", mat);
    cv::waitKey(0);
}


/**
 * Converts a grayscale image to a bilevel image.
 */
PyObject* binarize(PyObject *grayImg, short threshold)
{
    NDArrayConverter cvt;
    cv::Mat img { cvt.toMat(grayImg) };
    for (int i = 0; i < img.rows; ++i)
    {
        uchar_t *ptr = img.ptr<uchar_t>(i);
        for (int j = 0; j < img.cols; ++j)
        {
            ptr[j] = ptr[j] < threshold ? 0 : 255;
        }
    }
    return cvt.toNDArray(img);
}

/**
 * Multiplies two ndarrays by first converting them to cv::Mat and returns
 * an ndarray containing the result back.
 */
PyObject* mul(PyObject *left, PyObject *right) {

    cout << "MUL! STEP 1" << endl;

    NDArrayConverter cvt;
    cv::Mat leftMat, rightMat;
    leftMat = cvt.toMat(left);
    rightMat = cvt.toMat(right);
    cout << "MUL! STEP 2" << endl;

    auto r1 = leftMat.rows, c1 = leftMat.cols, r2 = rightMat.rows,
         c2 = rightMat.cols;


    cout << "MUL! STEP 3 " <<  r1 << "x" << c1 << " - " <<  r2 << "x" << c2 << endl;
    // Work only with 2-D matrices that can be legally multiplied.
    if (c1 != r2)
    {
        PyErr_SetString(PyExc_TypeError, 
                        "Incompatible sizes for matrix multiplication.");
        py::throw_error_already_set();
    }

    cout << "MUL! STEP 4 " << endl;
    cv::Mat result = leftMat * rightMat;

    string matAsString (result.begin<unsigned char>(), result.end<unsigned char>());
    cout << "MUL! STEP 5 " <<  result.rows << "x" << result.cols << " - " << matAsString << endl;
    PyObject* ret = cvt.toNDArray(result);

    cout << "MUL! STEP 6 " << ret << endl;
    return ret;
}

static void init()
{
    Py_Initialize();
    import_array();
}

BOOST_PYTHON_MODULE(examples)
{
    init();
    py::def("display", display);
    py::def("binarize", binarize);
    py::def("mul", mul);
}

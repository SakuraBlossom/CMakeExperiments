#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API

#include <iostream>
#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/python.hpp>

#include "../include/CvBoostConverter.hpp"

namespace py = boost::python;
using namespace std;

typedef unsigned char uchar_t;


/**
 * Displays an image, passed in from python as an ndarray.
 */
void display(PyObject *img)
{
    cv::Mat mat { pbcvt::fromNDArrayToMat(img) };
    
    cv::namedWindow("display", CV_WINDOW_NORMAL);
    cv::imshow("display", mat);
    cv::waitKey(0);
}


/**
 * Converts a grayscale image to a bilevel image.
 */
PyObject* binarize(PyObject *grayImg, short threshold)
{
    cv::Mat img { pbcvt::fromNDArrayToMat(grayImg) };
    for (int i = 0; i < img.rows; ++i)
    {
        uchar_t *ptr = img.ptr<uchar_t>(i);
        for (int j = 0; j < img.cols; ++j)
        {
            ptr[j] = ptr[j] < threshold ? 0 : 255;
        }
    }

    PyObject *ret = pbcvt::fromMatToNDArray(img);
    return ret;
}

/**
 * Multiplies two ndarrays by first converting them to cv::Mat and returns
 * an ndarray containing the result back.
 */
PyObject* mul(PyObject *left, PyObject *right) {

    cv::Mat leftMat, rightMat;
    leftMat = pbcvt::fromNDArrayToMat(left);
    rightMat = pbcvt::fromNDArrayToMat(right);

    auto r1 = leftMat.rows, c1 = leftMat.cols, r2 = rightMat.rows,
         c2 = rightMat.cols;

    cout << "MUL! STEP 1 " <<  r1 << "x" << c1 << " - " <<  r2 << "x" << c2 << endl;
    // Work only with 2-D matrices that can be legally multiplied.
    if (c1 != r2)
    {
        PyErr_SetString(PyExc_TypeError, 
                        "Incompatible sizes for matrix multiplication.");
        py::throw_error_already_set();
    }
    cv::Mat result = leftMat * rightMat;

    string matAsString (result.begin<unsigned char>(), result.end<unsigned char>());
    cout << "MUL! STEP 2 " <<  result.rows << "x" << result.cols << " - " << matAsString << endl;
    PyObject *ret = pbcvt::fromMatToNDArray(result);
    return ret;
}

PyObject* wrap2Mats(PyObject *left, PyObject *right) {

    cv::Mat leftMat, rightMat;
    leftMat = pbcvt::fromNDArrayToMat(left);
    rightMat = pbcvt::fromNDArrayToMat(right);

    vector<cv::Mat> list;
    list.push_back(leftMat);
    list.push_back(rightMat);

    return pbcvt::std_vector_to_py_list(list, &pbcvt::fromMatToNDArray);
}

PyObject* passInts(PyObject *list) {
    auto temp = pbcvt::py_list_to_std_vector<int>(list);
    int total = 0;
    for (auto id : temp) {
        cout << id << endl;
        total += id;
    }
    temp.push_back(total);
    return pbcvt::std_vector_to_py_list(temp);
}

PyObject* passStrings(PyObject *list) {
    auto temp = pbcvt::py_list_to_std_vector<string>(list);
    for (auto id : temp) {
        cout << id << endl;
    }
    temp.push_back("!!");
    return pbcvt::std_vector_to_py_list(temp);
}

static void init()
{
    import_array();
    Py_Initialize();
}

BOOST_PYTHON_MODULE(examples)
{
    init();
    py::def("display", display);
    py::def("binarize", binarize);
    py::def("mul", mul);
    py::def("passInts", passInts);
    py::def("passStrings", &passStrings);
    py::def("wrap2Mats", &wrap2Mats);
}

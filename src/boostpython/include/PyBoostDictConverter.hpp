#ifndef PYBOOSTCONVERTER_HPP_LIST_SL
#define PYBOOSTCONVERTER_HPP_LIST_SL

#include "PyBoostListConverter.hpp"
#include <stdexcept>

namespace pbcvt {

namespace py = boost::python;
using namespace std;

typedef std::variant<int, double, cv::Mat, std::string> argVariants;

//===================   TEMPLATE HELPER FUNCTIONS     ==============================================
template<typename T, typename VARIANT_T>
struct isVariantMember;

template<typename T, typename... ALL_T>
struct isVariantMember<T, std::variant<ALL_T...>> 
  : public std::disjunction<std::is_same<T, ALL_T>...> {};

template <class T>
inline bool isPrimitiveType(const T& data) {
    return std::is_fundamental<T>::value;
}

template<typename Test, template<typename...> class Ref>
struct is_specialization : std::false_type {};

template<template<typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref>: std::true_type {};

/* Usage:
using var_t = std::variant<int, float>;
bool x = isVariantMember<int, var_t>::value; // x == true
bool y = isVariantMember<double, var_t>::value; // y == false
*/

//===================   STD::VARIANT STREAMER FUNCTIONS     ========================================
template<typename T>
struct streamer {
public:
    const T& val;

    py::object operator()();

};
template<typename T> streamer(T) -> streamer<T>;

template<typename T>
std::ostream& operator<<(std::ostream& os, streamer<T> s) {
    os << s.val;
    return os;
}

template<typename... Ts>
std::ostream& operator<<(std::ostream& os, streamer<std::variant<Ts...>> sv) {
   std::visit([&os](const auto& v) { os << streamer{v}; }, sv.val);
   return os;
}

template<typename T>
py::object streamer<T>::operator()() {
    return  to_python(this->val);
}


//===================   DICT CONVERTER FUNCTIONS     ===============================================

std::map<string, string> fromPythonAsStringDict(PyObject *dict) {

    py::handle<> handle(py::borrowed(dict));
    py::dict iterable(handle);

    std::map<string, string> res;

    vector<string> keys(py::stl_input_iterator<string>(iterable.keys()),
                             py::stl_input_iterator<string>());

    //boost::python::list keys = iterable.keys();  
    for (std::vector<string>::iterator itr = keys.begin() ; itr != keys.end(); ++itr) {


        py::object obj = iterable[*itr];
        py::extract<int> extracted_val_int(obj);
        if (extracted_val_int.check()) { 
            res[*itr] = std::to_string(extracted_val_int);
            continue;
        }

        py::extract<double> extracted_val_double(obj);
        if (extracted_val_double.check()) { 
            res[*itr] = std::to_string(extracted_val_double);
            continue;
        }

        py::extract<std::string> extracted_val_str(obj);
        if (extracted_val_str.check()) { 
            res[*itr] = (std::string)(extracted_val_str);
            continue;
        }

        py::extract<cv::Mat> extracted_val_mat(obj);
        if (extracted_val_mat.check()) {
            std::stringstream ss;
            ss << (cv::Mat)(extracted_val_mat);
            res[*itr] = ss.str();
            continue;
        }

        throw invalid_argument("Expects python dictionary with values consisting of only string, int, double and numpy!");
    }

    return res;
}

std::map<string, argVariants> fromPythonDict(PyObject *dict) {

    py::handle<> handle(py::borrowed(dict));
    py::dict iterable(handle);

    std::map<string, argVariants> res;

    vector<string> keys(py::stl_input_iterator<string>(iterable.keys()),
                             py::stl_input_iterator<string>());

    //boost::python::list keys = iterable.keys();  
    for (std::vector<string>::iterator itr = keys.begin() ; itr != keys.end(); ++itr) {

        py::object obj = iterable[*itr];
        py::extract<int> extracted_val_int(obj);
        if (extracted_val_int.check()) { 
            res[*itr] = (int)extracted_val_int;
            continue;
        }

        py::extract<double> extracted_val_double(obj);
        if (extracted_val_double.check()) { 
            res[*itr] = (double)extracted_val_double;
            continue;
        }

        py::extract<std::string> extracted_val_str(obj);
        if (extracted_val_str.check()) { 
            res[*itr] = (std::string)(extracted_val_str);
            continue;
        }

        py::extract<cv::Mat> extracted_val_mat(obj);
        if (extracted_val_mat.check()) { 
            res[*itr] = (cv::Mat)(extracted_val_mat);
            continue;
        }

        throw invalid_argument("Expects python dictionary with values consisting of only string, int, double and numpy!");
    }

    return res;
}

PyObject* toPythonDict(std::map<string, string>& map) {
	boost::python::dict dictionary;
    
	for (auto iter = map.begin(); iter != map.end(); ++iter) {
        dictionary[iter->first] = iter->second;
	}
    return py::incref(dictionary.ptr());
}

PyObject* toPythonDict(std::map<string, argVariants>& map) {
	boost::python::dict dictionary;
    
	for (auto iter = map.begin(); iter != map.end(); ++iter) {

        auto& key = iter->first;
        switch((iter->second).index()) {
            case 0:
                dictionary[key] = std::get<0>(iter->second);
                break;
            case 1:
                dictionary[key] = std::get<1>(iter->second);
                break;
            case 2:
                dictionary[key] = std::get<2>(iter->second);
                break;
            case 3:
                dictionary[key] = std::get<3>(iter->second);
                break;
            default:
                throw invalid_argument("Undefined std::variant type!");
        }
	}
    return py::incref(dictionary.ptr());
}

} // end namespace pbcvt

#endif

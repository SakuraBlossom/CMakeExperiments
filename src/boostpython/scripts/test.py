#!/usr/bin/env python
import numpy as np
import sys
import os
import boost_python_catkin_example.examples as examples


def test_vector_multiplication():
    a = np.array([[1., 2., 3.]])
    b = a.reshape(3, 1)
    res2 = a.dot(b)
    print(res2)
    res1 = examples.mul(a, b)
    print("res: {} == {}".format(res1, res2))
    examples.printStr(str(np.equal(res1, res2).all()))

def test_matrix_squaring():
    a = np.array([[1., 2., 3.], [4., 5., 6.],  [7., 8., 9.]])
    print(np.equal(examples.mul(a, a), a.dot(a)).all())

def test_matrix_multiplication():
    a = np.array([[1., 2., 3., 4.], [4., 3., 2., 1.]])
    b = np.array([[ 1., 0.], [ 2., 3.], [ 4., 4.], [-1., 2.]])
    print(np.equal(examples.mul(a, b), a.dot(b)).all())

def test_identity_multiplication():
    eye = np.eye(3)
    a = np.random.random((3, 3)) * 10
    print(np.equal(examples.mul(eye, a), examples.mul(a, eye)).all())
    print(np.equal(examples.mul(eye, a), eye.dot(a)).all())

def test_binarization():
    image = np.ones((8, 8), dtype=np.uint8) * 128 # Grayscale
    image[4:6, 4:6] = 126
    expected = np.ones((8, 8), dtype=np.uint8) * 255
    expected[4:6, 4:6] = 0
    binarized = examples.binarize(image, 127)
    print(np.equal(expected, binarized).all())

def test_print_list():
    print(examples.passInts([1, 2, 3]))
    print(examples.passStrings(['Hello', 'world', 'awesome']))

def test_wrap_tolist():
    a = np.array([[1., 2., 3., 4.], [4., 3., 2., 1.]])
    b = np.array([[ 1., 0.], [ 2., 3.], [ 4., 4.], [-1., 2.]])
    mylist = examples.wrap2Mats(a, b)
    print(mylist[0])
    print(mylist[1])

def test_wrap_tuple():
    mytuple = (3, 3.14)
    mytuple1 = examples.tupid1(mytuple)
    print(mytuple1)
    mytuple2 = examples.tupid2((5, 1.55, "Hi"))
    print(mytuple2)
    a = np.array([[1., 2., 3., 4.], [4., 3., 2., 1.]])
    mytuple3 = examples.tupid3((5, 1.55, a))
    print(mytuple3)

def test_none_tuple():
    a = np.array([[1., 2., 3., 4.], [4., 3., 2., 1.]])
    print(examples.tupidCheckNone((5, 1.55, a)))
    print(examples.tupidCheckNone(None))

def test_dict():
    a = np.array([[1., 2., 3., 4.], [4., 3., 2., 1.]])
    thisdict = { "brand": "Ford", "model": "Mustang", "year": 1964, "size" : a }
    print(examples.readDictStringOnly(thisdict))
    print(examples.readDict(thisdict))

def test_custom_class():
    x = examples.Detector('pi')
    x.value = 3.14
    print('{} is around {}'.format(x.name, x.value))
    return x
    
if __name__ == '__main__':
    print(os.getcwd())
    print(os.environ['PYTHONPATH'])
    test_vector_multiplication()
    test_matrix_squaring()
    test_matrix_multiplication()
    test_identity_multiplication()
    test_binarization()
    test_print_list()
    test_wrap_tolist()
    test_wrap_tuple()
    test_none_tuple()
    test_dict()
    test_custom_class()


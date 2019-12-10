#!/usr/bin/env python
import numpy as np
import sys
import boost_python_catkin_example.examples as examples


def test_vector_multiplication():
    a = np.array([[1., 2., 3.]])
    b = a.reshape(3, 1)
    res2 = a.dot(b)
    print(res2)
    res1 = examples.mul(a, b)
    print("res: {} == {}", res1, res2)
    print(np.equal(res1, res2).all())

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

if __name__ == '__main__':
    test_vector_multiplication()
    test_matrix_squaring()
    test_matrix_multiplication()
    test_identity_multiplication()
    test_binarization()
    test_print_list()
    test_wrap_tolist()
    test_wrap_tuple()


#!/usr/bin/env python
import numpy as np
import sys
import os
print(os.getcwd())
print(os.environ['PYTHONPATH'])

import boost_python_catkin_example.examples as examples

class Dog():
    def test_wrap_tuple(self):
        mytuple = (3, 3.14)
        mytuple1 = examples.tupid1(mytuple)
        print(mytuple1)
        mytuple2 = examples.tupid2((5, 1.55, "Hi"))
        print(mytuple2)
        a = np.array([[1., 2., 3., 4.], [4., 3., 2., 1.]])
        mytuple3 = examples.tupid3((5, 1.55, a))
        print(mytuple3)

    def test_none_tuple(self):
        a = np.array([[1., 2., 3., 4.], [4., 3., 2., 1.]])
        print(examples.tupidCheckNone((5, 1.55, a)))
        print(examples.tupidCheckNone(None))

    def test_dict(self):
        a = np.array([[1., 2., 3., 4.], [4., 3., 2., 1.]])
        thisdict = { "brand": "Ford", "model": "Mustang", "year": 1964, "size" : a }
        print(examples.readDictStringOnly(thisdict))
        print(examples.readDict(thisdict))
    
    def bark(self, message):
        print("The dog barks: {}".format(message))
        self.test_wrap_tuple()
        self.test_none_tuple()
        self.test_dict()

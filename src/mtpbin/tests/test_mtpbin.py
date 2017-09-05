#!/usr/bin/python
#
# Usage:
#    py.test test_mtpbin.py
#
# Documentation for the QtTest Module:
# https://docs.python.org/2/library/unittest.html
#

import os
import re
import unittest

class TestMTPbin(unittest.TestCase):

    def tearDown(self):
	os.system("rm retriever_test.new")
	os.system("rm rcf_set_test.out.s")
	os.system("rm rcf_set_test.new.s")
	os.system("rm rcf_set_test.new")
	os.system("rm rcf_test.new")
	os.system("rm calibrator_test.new")

    def test_retriever(self):
	# Compare the newly-generated retriever_test.new output file with the "control"
	# file retriever_test.out, which has been vetted to assure that it is correct.
	os.system("retriever_test > retriever_test.new")
	f = open("retriever_test.new","r")
	c = open("retriever_test.out","r")
        for line in c:
            fline = f.readline()
            self.assertEqual(line,fline)

    def test_rcf_set(self):
	# Compare the newly-generated rcf_set_test.new output file with the "control"
	# file rcf_set_test.out, which has been vetted to assure that it is correct.
	os.system("rcf_set_test > rcf_set_test.new")
	os.system("sort rcf_set_test.out > rcf_set_test.out.s")
	os.system("sort rcf_set_test.new > rcf_set_test.new.s")
	f = open("rcf_set_test.new.s","r")
	c = open("rcf_set_test.out.s","r")
        for line in c:
            fline = f.readline()
	    line = re.sub(r'/home/local/raf/instruments/mtp/src/mtpbin','..',line)
            self.assertEqual(line,fline)

    def test_rcf(self):
	# Compare the newly-generated rcf_test.new output file with the "control"
	# file rcf_test.out, which has been vetted to assure that it is correct.
	os.system("rcf_test > rcf_test.new")
	f = open("rcf_test.new","r")
	c = open("rcf_test.out","r")
        for line in c:
            fline = f.readline()
	    line = re.sub(r'/home/local/raf/instruments/mtp/src/mtpbin','..',line)
            self.assertEqual(line,fline)

    def test_calibrator(self):
	# Compare the newly-generated rcf_test.new output file with the "control"
	# file calibrator.out, which has been vetted to assure that it is correct.
	os.system("calibrator_test > calibrator_test.new")
	f = open("calibrator_test.new","r")
	c = open("calibrator_test.out","r")
        for line in c:
            fline = f.readline()
            self.assertEqual(line,fline)


if __name__ == '__main__':
        unittest.main()

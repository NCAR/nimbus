#! /usr/bin/python

from selenium import webdriver
import time
import sys
import os


# -77.9957, 28.9685, -74.2237, 37.1555

class RenderKML(object):

    def __init__(self):
        self.bounds = None
        self.kmlfiles = []
        self.cwd = os.getcwd()

    def setFiles(self, kmlfiles):
        self.kmlfiles = kmlfiles[:]

    def setBounds(self, bounds):
        self.bounds = bounds

    def write_js(self, path=None):
        if not path:
            path = "flight_data/flight_data.js"
        with open(path, "w") as js:
            js.write('var kmlfiles=[]\n')
            for kml in self.kmlfiles:
                js.write('kmlfiles.push("%s");\n' % (kml))
            if self.bounds:
                js.write('var defaultbounds="%s";\n' % (self.bounds))

    def dump_image(self, image):
        driver = webdriver.PhantomJS()
        driver.set_window_size(1024, 768) # optional
        driver.get('file://%s/flight_data.html' % (self.cwd))
        time.sleep(10)
        driver.save_screenshot(image) # save a screenshot to disk

    def main(self, args):
        kmlfiles = []
        bounds = None
        image = 'renderkml.png'
        if len(args) == 0:
            print("Usage: renderkml.py <image.png> <file.kml> [file2.kml ...] "
                  "[wlon,slat,elon,nlat]")
            sys.exit(1)
        for arg in args:
            if arg.endswith(".kml"):
                kmlfiles.append(arg)
            elif arg.endswith(".png"):
                image = arg
            else:
                bounds = arg
        self.setFiles(kmlfiles)
        self.setBounds(bounds)
        self.write_js()
        self.dump_image(image)
        return 0


if __name__ == "__main__":
    rkml = RenderKML()
    sys.exit(rkml.main(sys.argv[1:]))

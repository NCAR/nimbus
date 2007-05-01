#!/bin/sh

av_file=/tmp/avaps.tmp

gunzip - > $av_file
/h/9/cjw/bin/processD $av_file

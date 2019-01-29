#!/usr/bin/env python3
import os, shutil

os.mkdir('LSINF1252')

dir = os.scandir('.')

for item in dir:
    if item.name != 'LSINF1252' and (item.is_dir() or item.name == 'course.yaml'):
        try:
            os.mkdir(os.path.join(item.name,'test'))
        except:
            pass  # test directory already existing
        shutil.move('./' + item.name, 'LSINF1252/' + item.name)

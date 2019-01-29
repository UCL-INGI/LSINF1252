#!/usr/bin/env python3
import os, shutil, yaml
"""Update makefile and run for ctester tasks"""

MK_FILE = 'Makefile'
RUN = 'run'
CPPCHECK_TAG = {
    'visible': True,
    'id': 'cppcheck',
    'name': 'Cppcheck fails',
    'type': 1,
    'description': 'Your code does not compile with cppcheck'
}


def is_ctester_task(task_path):
    student_path = os.path.join(task_path, 'student')
    student_dir = os.scandir(student_path)
    names = [entry.name for entry in student_dir]
    return 'CTester' in names


def remove_cppcheck_tag(yaml_path):
    with open(yaml_path,'r') as yaml_file:
        yaml_data = yaml.load(yaml_file)
    tags = yaml_data['tags']
    to_del = []
    for k in tags:
        if tags[k] == CPPCHECK_TAG:
            to_del.append(k)
    for i in to_del:
        tags.pop(i, None)
    yaml_data['tags'] = tags
    with open(yaml_path,'w') as yaml_file:
        yaml_file.write(yaml.dump(yaml_data))


def add_cppcheck_tag(yaml_path):
    with open(yaml_path,'r') as yaml_file:
        yaml_data = yaml.load(yaml_file)
    tags = yaml_data['tags']
    try:
        last_index = max([int(k) for k in tags])
    except:
        last_index = 0
    tags[str(last_index+1)] = CPPCHECK_TAG
    yaml_data['tags'] = tags
    with open(yaml_path,'w') as yaml_file:
        yaml_file.write(yaml.dump(yaml_data))


def copy_mkfile_run(src_path, course_path, add_cppcheck=False, rm_cppcheck=False):
    make_path = os.path.join(src_path, MK_FILE)
    run_path = os.path.join(src_path, RUN)
    course_dir = os.scandir(course_path)
    for task in course_dir:
        if task.is_dir() and not task.name.startswith('.'):
            task_dir = os.scandir(task.path)
            names = [entry.name for entry in task_dir]
            if 'student' in names and 'task.yaml' in names:
                if is_ctester_task(task.path) :
                    shutil.copy2(make_path,os.path.join(task.path,'student'))
                    shutil.copy2(run_path,task.path)
                    if add_cppcheck:
                        add_cppcheck_tag(os.path.join(task.path,'task.yaml'))
                    if rm_cppcheck:
                        remove_cppcheck_tag(os.path.join(task.path,'task.yaml'))


copy_mkfile_run('.', '.', False, False)
#!/usr/bin/env python3
import os, yaml

dir = os.scandir('.')

for entry in dir:
    if entry.is_dir():
        sub_dir = os.scandir(entry.name)
        names = [item.name for item in sub_dir]
        if 'task.yaml' in names:  # task directory
            test_path = os.path.join(entry.name, 'test')
            test_dir = os.scandir(test_path)
            for yaml_entries in test_dir:
                if not yaml_entries.name.startswith('.') and yaml_entries.is_file():
                    data = {}
                    with open(os.path.join(test_path, yaml_entries.name),'r') as yaml_file:
                        data = yaml.load(yaml_file)
                    print(data)
                    data['username'] = 'anonymous'
                    data['input']['@username'] = 'anonymous'
                    with open(os.path.join(test_path, yaml_entries.name),'w') as yaml_file:
                        yaml_file.write(yaml.dump(data))
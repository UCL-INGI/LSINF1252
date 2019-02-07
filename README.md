# LSINF1252
[![Build Status](https://travis-ci.org/UCL-INGI/LSINF1252.svg?branch=master)](https://travis-ci.org/UCL-INGI/LSINF1252)

This repository contains the [INGInious](https://www.inginious.org) tasks for [UCLouvain](https://www.uclouvain.be)'s course LSINF1252, computing systems. 

Each [INGInious](https://www.inginious.org) task is structured as follows:

 - a ``student`` directory that contains the C code for the exercise
 - a ``run`` file that is executed by [INGInious](https://www.inginious.org) when a student provides an answer
 - a ``task.yaml`` file that provides the description of the exercise
 
The ``student`` directory contains several types of files:

 - a ``CTester`` directory that contains the code of the [Ctester library](https://github.com/UCL-INGI/CTester) that is used to execute the student code and intercept library calls
 - a ``student_code.c.tpl`` file which is the template that the students have to fill in
 - one or more ``test.c`` files that contain the actual tests
 
 You can find in [Report.md](Report.md) a description on how to write a bug report for an [INGInious](https://www.inginious.org) task

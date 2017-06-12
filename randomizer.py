#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun May 21 15:50:29 2017

@author: lburzawa
"""

import numpy as np
from sklearn.utils import shuffle

SIZE=500000
split_ind=int(0.8*SIZE)
order=np.random.permutation(SIZE)
f=open('all_data.csv','r')
data=f.readlines()
data=shuffle(data)
f.close()

f=open('train.csv','w')
for i in range(0,split_ind):
    f.write(data[i])
f.close()

f=open('test.csv','w')
for i in range(split_ind,SIZE):
    f.write(data[i])
f.close()

#!/usr/bin/python
# -*- coding: utf-8 -*-
import commands  
import os  
main = "./darknet detector demo cfg/coco.data cfg/yolov3-tiny.cfg yolov3-tiny.weights"  
if os.path.exists(main):  
    rc, out = commands.getstatusoutput(main)  
    print 'rc = %d, \nout = %s' % (rc, out)  
  
print '*'*10  /
f = os.popen(main)    
data = f.readlines()    
f.close()    
print data  
  
print '*'*10  
r_v = os.system(main) 
print r_v 

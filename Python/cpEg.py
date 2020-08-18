import os, sys


l_dir = filter(os.path.isdir, os.listdir(os.getcwd()))

for x in l_dir:
    cmd = 'cp index.php '+x+'/'
    print cmd
    os.system(cmd)

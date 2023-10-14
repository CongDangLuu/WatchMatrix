import os
import shutil
Import("env")

#
# Custom actions when building program/firmware
#

def after_building(source, target, env):
    srcfile=source[0].get_abspath()
    #read build number
    file1 = open('BuildNum.txt','r')
    BN=int(file1.readline(2))
    file1.close()
    dstfile=os.path.abspath(os.getcwd())+"/Target/Image_" + str(BN) + ".bin"
    BN +=1
    file1 = open('BuildNum.txt','w')
    file1.write(str(BN))
    file1.close()

    shutil.copyfile(srcfile, dstfile)
env.AddPostAction("buildprog", after_building)

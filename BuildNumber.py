import os
import shutil
Import("env")

#
# Custom actions when building program/firmware
#
def strBuildnumber(num):
    strnum = "" 
    if num > 999:
        return "000"
    
    strnum += str((int)(num/100))
    strnum += str((int)(num/10)%10)
    strnum += str((int)(num%100%10))
    return strnum

def after_building(source, target, env):
    print("run after_building")
    srcfile=source[0].get_abspath()
    #read build number
    file1 = open('BuildNum.txt','r')
    BN=int(file1.readline(2))
    file1.close()
    strBN = strBuildnumber(BN)
    dstfile=os.path.abspath(os.getcwd())+"/Target/Image-" + strBN + ".bin"
    print("File taget: "+ dstfile)
    BN +=1
    file1 = open('BuildNum.txt','w')
    file1.write(str(BN))
    file1.close()

    shutil.copyfile(srcfile, dstfile)
env.AddPostAction("buildprog", after_building)

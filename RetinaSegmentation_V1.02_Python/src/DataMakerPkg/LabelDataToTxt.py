'''
Created on Mar 27, 2015

@author: linhuangjing
'''
import os

class ExudateLabelDataToTxt:
    def __init__(self, wildcard='.jpg .png .JPG .PNG', recursive=1):
        self.wildcard = wildcard
        self.recursive = recursive
    
    def __FindAndWrtTxt(self, dir,file,wildcard='.jpg .png .JPG .PNG',recursive=1):
        '''__FindAndWrtTxt(dir,file,wildcard='.jpg .png .JPG .PNG',recursion=1)
           This function is a recursive function, which is used to find the files ended with wildcard in the dir recursively, and write it into .txt
           There are 3 type of classes: 0_background, 1_disk and 2_exudate.
           dir is the Directory to find.
           file is the streamfile. 
           wildcard is extends just like '.jpg .png'
           recursive is used to determine whether recurse to find.
        '''
        exts = wildcard.split(" ")
        files = os.listdir(dir)
        for name in files:
            fullname = os.path.join(dir,name)
            if(os.path.isdir(fullname)&recursive):
                self.__FindAndWrtTxt(fullname, file, wildcard, recursive)
            else:
                for ext in exts:
                    if(name.endswith(ext)):
                        foldername = os.path.split(dir)[1]
                        sup_folder = os.path.split(os.path.split(dir)[0])[1] 
                        if(foldername == '0_background'):
                            file.write('/'+sup_folder+'/'+foldername+'/'+name+'  '+'0'+'\n')
                        elif(foldername == '1_disk'):
                            file.write('/'+sup_folder+'/'+foldername+'/'+name+'  '+'1'+'\n')
                        elif(foldername == '2_exudate'):
                            file.write('/'+sup_folder+'/'+foldername+'/'+name+'  '+'2'+'\n')
                        break
                
    def Apply(self, fileDir, txtPath):
        '''Apply(fileDir, txtPath) is used to write the tuple "Path  label" to the .txt file..
           fileDir is the main Directory Path of Dateset. 
           txtPath is the Path of .txt.
        '''
        file = open(txtPath,'w')
        if not file:
            print ('Cannot open the file %s for writing' %fileDir)
        self.__FindAndWrtTxt(fileDir, file)

class HamorLabelDataToTxt:
    def __init__(self, wildcard='.jpg .png .JPG .PNG', recursive=1):
        self.wildcard = wildcard
        self.recursive = recursive
    
    def __FindAndWrtTxt(self, dir,file,wildcard='.jpg .png .JPG .PNG',recursive=1):
        '''__FindAndWrtTxt(dir,file,wildcard='.jpg .png .JPG .PNG',recursion=1)
           This function is a recursive function, which is used to find the files ended with wildcard in the dir recursively, and write it into .txt
           There are 3 type of classes: 0_background, 1_vessel, 2_edge and 3_hamorrage.
           dir is the Directory to find.
           file is the streamfile. 
           wildcard is extends just like '.jpg .png'
           recursive is used to determine whether recurse to find.
        '''
        exts = wildcard.split(" ")
        files = os.listdir(dir)
        for name in files:
            fullname = os.path.join(dir,name)
            if(os.path.isdir(fullname)&recursive):
                self.__FindAndWrtTxt(fullname, file, wildcard, recursive)
            else:
                for ext in exts:
                    if(name.endswith(ext)):
                        foldername = os.path.split(dir)[1]
                        sup_folder = os.path.split(os.path.split(dir)[0])[1] 
                        if(foldername == '0_background'):
                            file.write('/'+sup_folder+'/'+foldername+'/'+name+'  '+'0'+'\n')
                        elif(foldername == '1_vessel'):
                            file.write('/'+sup_folder+'/'+foldername+'/'+name+'  '+'1'+'\n')
                        elif(foldername == '2_edge'):
                            file.write('/'+sup_folder+'/'+foldername+'/'+name+'  '+'2'+'\n')
                        elif(foldername == '3_hamor'):
                            file.write('/'+sup_folder+'/'+foldername+'/'+name+'  '+'3'+'\n')
                        break
                
    def Apply(self, fileDir, txtPath):
        '''Apply(fileDir, txtPath) is used to write the tuple "Path  label" to the .txt file..
           fileDir is the main Directory Path of Dateset. 
           txtPath is the Path of .txt.
        '''
        file = open(txtPath,'w')
        if not file:
            print ('Cannot open the file %s for writing' %fileDir)
        self.__FindAndWrtTxt(fileDir, file)
        
class HamorLabelDataToTxt2:
    def __init__(self, wildcard='.jpg .png .JPG .PNG', recursive=1):
        self.wildcard = wildcard
        self.recursive = recursive
    
    def __FindAndWrtTxt(self, dir,file,wildcard='.jpg .png .JPG .PNG',recursive=1):
        '''__FindAndWrtTxt(dir,file,wildcard='.jpg .png .JPG .PNG',recursion=1)
           This function is a recursive function, which is used to find the files ended with wildcard in the dir recursively, and write it into .txt
           There are 3 type of classes: 0_background, 1_vessel, 2_edge and 3_hamorrage.
           dir is the Directory to find.
           file is the streamfile. 
           wildcard is extends just like '.jpg .png'
           recursive is used to determine whether recurse to find.
        '''
        exts = wildcard.split(" ")
        files = os.listdir(dir)
        for name in files:
            fullname = os.path.join(dir,name)
            if(os.path.isdir(fullname)&recursive):
                self.__FindAndWrtTxt(fullname, file, wildcard, recursive)
            else:
                for ext in exts:
                    if(name.endswith(ext)):
                        foldername = os.path.split(dir)[1]
                        sup_folder = os.path.split(os.path.split(dir)[0])[1] 
                        if(foldername == '0_background'):
                            file.write('/'+sup_folder+'/'+foldername+'/'+name+'  '+'0'+'\n')
                        elif(foldername == '1_vessel'):
                            file.write('/'+sup_folder+'/'+foldername+'/'+name+'  '+'1'+'\n')
                        elif(foldername == '2_hamor'):
                            file.write('/'+sup_folder+'/'+foldername+'/'+name+'  '+'2'+'\n')
                        break
                
    def Apply(self, fileDir, txtPath):
        '''Apply(fileDir, txtPath) is used to write the tuple "Path  label" to the .txt file..
           fileDir is the main Directory Path of Dateset. 
           txtPath is the Path of .txt.
        '''
        file = open(txtPath,'w')
        if not file:
            print ('Cannot open the file %s for writing' %fileDir)
        self.__FindAndWrtTxt(fileDir, file)
    
if __name__=='__main__': 
    fileDir = '/home/linhuangjing/Desktop/Retina_Data/Debug/TestingData'
    txtPath = '/home/linhuangjing/Desktop/Retina_Data/Debug/TestingData/testdata.txt'
    
    LData2Txt = HamorLabelDataToTxt2()
    LData2Txt.Apply(fileDir, txtPath)
    print 'Finish!'
    
    
    
#!/usr/bin/env python
# _*_ coding:utf-8 _*_

import os
import sys

#该结构体存放了目前ametal中开发板和芯片的映射关系，
#如果添加了新的开发，需要在添加新的开发板映射关系到下表中
__g_board_soc_map  = {
#      开发板      ： 芯片      
    'am217_core'   : 'zlg/zlg217',
    'am227_core'   : 'zlg/zlg217',
    'am237_core'   : 'zlg/zlg237',
    'am116_core'   : 'zlg/zlg116',
    'am118_core'   : 'zlg/zlg118',
    'aml166_core'  : 'zlg/zml166',
    'am126_core'   : 'zlg/zlg116',
    'am159_core'   : 'zlg/zmf159',
    'am824_core'   : 'nxp/lpc824',
    'am824zb'      : 'nxp/lpc824',
    'am824ble'     : 'nxp/lpc824',
    'am845_core'   : 'nxp/lpc845',
    'amks16z_core' : 'freescale/kl26',
    'amks16rfid'   : 'freescale/kl26',
    'amks16rfid8'  : 'freescale/kl26',

}

paramerror = 0
__g_board_name = ''
__g_company_name = ''

if len(sys.argv) == 2 :
    __g_board_name =  sys.argv[1]
else :
    paramerror = 1

if paramerror :
    print 'Usage (参考《AMetal新增文件管理工具说明》): ./exclude_new_board.py [new_board name]'
    print 'example : ./exclude_new_board.py am237_core'
    exit(-1)

# 设置环境变量和路径
cur_build_path = os.path.split(os.path.realpath(__file__))[0];
ametal_path = os.path.realpath('cur_build_path/../../..')
ametal_board_top_path = os.path.join(ametal_path, 'board')

def sdk_exclude() :
    soc_of_board = __g_board_soc_map[__g_board_name]
    soc_path = os.path.join('soc/', soc_of_board)
    global __g_company_name
    __g_company_name = (soc_of_board.rpartition('/'))[0]

    dir_list = os.listdir(ametal_board_top_path)
    for file in dir_list :
        same_cmay = 0
        if((file == 'bsp_common') or (file == __g_board_name)) :
            continue
        else :
            if __g_company_name in (__g_board_soc_map[file]):
                same_cmay = 1

            board_path = os.path.join(ametal_board_top_path, file)
            for root, dirs, files in os.walk(board_path, topdown = False):
                for file_name in files :
                    if (file_name == '.cproject'):
                        if same_cmay == 1:
                            exclude_soc_in_cproject(os.path.join(root, file_name))
                            exclude_soc_example_in_cproject(os.path.join(root, file_name))
                        if 'project_template' not in  root:
                            w_path = 'examples/board/' + __g_board_name
                            exclude_board_in_cproject(os.path.join(root, file_name), w_path)

#写入需要屏蔽的文件到 .cproject 文件的 excludeing 关键字中
def write_exclude_in_cproject(cpro_path, file_path):

    if not os.path.exists(cpro_path): 
        print '.cproject script does not exist!'
        return #不存在就退出
    
    with open(cpro_path, "r+") as f:
        lines = f.readlines()
        f.seek(0, 0)
        for line in lines:
            if 'excluding' in line:
                if file_path in line:
                    f.write(line)
                    return
                    
                str_list = list(line)
                npos = str_list.index('=')
                str_list.insert(npos + 2, file_path + '|')
                new_str = ''.join(str_list)
                f.write(new_str)
                continue
            f.write(line)
    f.close() 
 
def exclude_board_in_cproject(cpro_path, file_path):

    real_file_path = os.path.join(ametal_path, file_path)
    #首先查看文件是否存在
    if not os.path.exists(real_file_path): 
        print 'file or folder does not exist!'
        return #不存在就退出
    #首先查看文件是否存在
    if not os.path.exists(cpro_path): 
        print '.cproject script does not exist!'
        return #不存在就退出   
        
    write_exclude_in_cproject(cpro_path, file_path)

def exclude_soc_in_cproject(cpro_path):
    global __g_company_name
    #首先查看文件是否存在
    if not os.path.exists(cpro_path):
        print '.cproject script does not exist!'
        return #不存在就退出
    w_path = 'soc/' + __g_board_soc_map[__g_board_name]
    write_exclude_in_cproject(cpro_path, w_path)
     
    driver_path = os.path.join(ametal_path, 'soc', __g_company_name, 'drivers' , 'source')
    
    #首先查看文件是否存在
    if not os.path.exists(driver_path): 
        print 'file or folder does not exist!'
        return #不存在就退出

    soc_name = (__g_board_soc_map[__g_board_name].rpartition('/'))[2]
    for root, dirs, files in os.walk(driver_path, topdown = False):
        for file_name in files :
            if soc_name in file_name:
                win_path = os.path.join((root.rpartition('ametal\\'))[2], file_name)
                w_driver_path = win_path.replace('\\', '/')
                print w_driver_path
                write_exclude_in_cproject(cpro_path, w_driver_path)

def exclude_soc_example_in_cproject(cpro_path):
    global __g_company_name
    soc_exmaple_path = os.path.join(ametal_path, 'examples\\soc\\', __g_company_name)
    #查看文件是否存在
    if not os.path.exists(soc_exmaple_path): 
        print 'file or folder does not exist!'
        return #不存在就退出
    soc_name = (__g_board_soc_map[__g_board_name].rpartition('/'))[2]
    for root, dirs, files in os.walk(soc_exmaple_path, topdown = False):
        for file_name in files :
            if soc_name in file_name:
                win_path = os.path.join((root.rpartition('ametal\\'))[2], file_name)
                w_driver_path = win_path.replace('\\', '/')
                print w_driver_path
                write_exclude_in_cproject(cpro_path, w_driver_path)

sdk_exclude()

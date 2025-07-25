"""
清除目录下所有文件，主要用于清除build目录下的内容

argv:
    [1] : 需要清除的目录路径
"""

import sys
import shutil
import os


# 主函数
if __name__ == "__main__":
    print("< 删除文件夹 >")
    print("[argv: {}]".format(sys.argv))

    # 判断参数
    if len(sys.argv) < 2:
        print("[argv] : [1: path]")
        exit(0)

    # 获取参数
    sys_path = sys.argv[1]

    # 删除目录
    abs_path = os.path.abspath(sys_path)
    if os.path.exists(abs_path):
        shutil.rmtree(abs_path)
        print("< 删除完成 >\n", abs_path)

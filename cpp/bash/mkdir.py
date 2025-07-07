"""
在目录不存在时创建目录

argv:
    [1] : 需要创建的目录路径
"""

import sys
import os


# 主函数
if __name__ == "__main__":
    print("< 创建文件夹 >")
    print("[argv: {}]".format(sys.argv))

    # 判断参数
    if len(sys.argv) < 2:
        print("[argv] : [1: path]")
        exit(0)

    # 获取参数
    sys_path = sys.argv[1]

    # 创建不存在目录
    abs_path = os.path.abspath(sys_path)
    if os.path.exists(abs_path) == False:
        os.mkdir(abs_path)
        print("< 创建完成 >\n", abs_path)

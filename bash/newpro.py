"""
创建新项目到路径

argv:
    [1] : 项目类型
    [2] : 目标路径
"""

import sys
import os
import shutil


# 显示类型
def show_type():
    print("< 类型列表 >")
    print("[cpp qt5 qt6]")


# 主函数
if __name__ == "__main__":
    print("< 创建新项目 >")
    print("[argv: {}]".format(sys.argv))

    # 判断参数
    if len(sys.argv) <= 2:
        print("[argv] : [1: type] [2: path]")
        show_type()
        exit(0)

    # 获取参数
    sys_name = sys.argv[1]
    sys_template = sys.argv[2]

    # 复制项目
    if sys_name == "cpp" or sys_name == "qt5" or sys_name == "qt6":
        path_src = os.path.join(os.path.dirname(__file__), "..", sys_name)
        path_dst = os.path.join(sys_template, sys_name)

        if os.path.exists(path_dst) == False:
            os.mkdir(path_dst)

        os.path.join(os.path.dirname(__file__), "..", sys_name)
        shutil.copytree(path_src, path_dst, dirs_exist_ok=True)
        print("< 创建项目完成 >\n", path_src, path_dst)
    else:
        print("不存在类型 :{}", sys_name)
        show_type()

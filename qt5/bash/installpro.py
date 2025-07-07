"""
安装程序依赖到指定路径

argv:
    [2] : 需要安装依赖的目录路径
"""

import sys
import shutil
import os
import proconfig


def run_system(argv):
    cmd = ""
    for i in argv:
        cmd += i + " "
    os.system(cmd)
    print(cmd)


# 主函数
if __name__ == "__main__":
    print("< 安装程序依赖 >")
    print("[argv: {}]".format(sys.argv))

    # 判断参数
    if len(sys.argv) < 1:
        print("[argv] : [2: path]")
        exit(0)

    # 获取参数
    path_qt5 = proconfig.var_qtdir
    proname = proconfig.var_proname
    sys_path = sys.argv[1]

    # 创建新目录
    abs_path = os.path.abspath(sys_path)
    if os.path.exists(abs_path):
        shutil.rmtree(abs_path)
    os.mkdir(abs_path)

    # 复制程序并切换到安装目录
    shutil.copy(os.path.join(os.getcwd(), proname), abs_path)
    os.chdir(abs_path)

    # 安装依赖
    cmd_deploy = os.path.join(path_qt5, "bin", "linuxdeployqt")
    run_system([cmd_deploy, proname, "-unsupported-allow-new-glibc"])

    print("< 安装程序依赖 >\n", abs_path)

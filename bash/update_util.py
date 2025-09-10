"""
更新项目的默认资源文件
"""

import sys
import os
import shutil
import json

# 主函数
if __name__ == "__main__":
    print("< 更新资源文件 >")

    ls_name = ["cpp", "qt5", "qt6"]

    path_bhtools = os.path.join("../", "util", "bhtools", "include")
    paht_cpp = os.path.join("../", "cpp", "include", "bhtools")
    paht_qt5 = os.path.join("../", "qt5", "util", "bhtools")
    paht_qt6 = os.path.join("../", "qt6", "util", "bhtools")

    shutil.copytree(path_bhtools, paht_cpp, dirs_exist_ok=True)
    shutil.copytree(path_bhtools, paht_qt5, dirs_exist_ok=True)
    shutil.copytree(path_bhtools, paht_qt6, dirs_exist_ok=True)
    print("< 更新完成 >\n", paht_cpp, paht_qt5, paht_qt6)

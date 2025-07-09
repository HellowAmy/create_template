"""
创建新项目到路径

argv:
    [1] : 项目类型
    [2] : 项目名称
    [3] : 目标路径
"""

import sys
import os
import shutil
import json


# 显示类型
def show_type():
    print("< 类型列表 >")
    print("[cpp qt5 qt6]")


# 主函数
if __name__ == "__main__":
    print("< 创建新项目 >")
    print("[argv: {}]".format(sys.argv))

    # 判断参数
    if len(sys.argv) <= 3:
        print("[argv] : [1: type] [2: name] [3: path]")
        show_type()
        exit(0)

    # 获取参数
    sys_name = sys.argv[1]
    sys_proname = sys.argv[2]
    sys_template = sys.argv[3]

    # 复制项目
    path_dst = ""
    if sys_name == "cpp" or sys_name == "qt5" or sys_name == "qt6":
        path_src = os.path.join(os.path.dirname(__file__), "..", sys_name)
        path_dst = os.path.join(sys_template, sys_proname)

        if os.path.exists(path_dst) == False:
            os.mkdir(path_dst)

        os.path.join(os.path.dirname(__file__), "..", sys_name)
        shutil.copytree(path_src, path_dst, dirs_exist_ok=True)
        print("< 创建项目完成 >\n", path_src, path_dst)
    else:
        print("不存在类型 :{}", sys_name)
        show_type()

    # 修改项目名称-CMakeLists
    file_cmakelists = os.path.join(path_dst, "CMakeLists.txt")
    file_all = ""
    with open(file_cmakelists, "r", encoding="utf-8") as f:
        file_all = f.read()
        file_all = file_all.replace(
            "project(hellow LANGUAGES CXX)",
            "project({} LANGUAGES CXX)".format(sys_proname),
        )

    with open(file_cmakelists, "w", encoding="utf-8") as f:
        f.write(file_all)

    # 修改项目名称-launch
    file_launch = os.path.join(path_dst, ".vscode", "launch.json")
    js = None
    with open(file_launch, "r", encoding="utf-8") as f:
        js = json.load(f)
        for it in js["configurations"]:
            propath: str = it["program"]
            path = os.path.dirname(propath)
            it["program"] = os.path.join(path, sys_proname)

    with open(file_launch, "w", encoding="utf-8") as f:
        json.dump(js, f, indent=4, ensure_ascii=False)

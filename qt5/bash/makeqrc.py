"""
创建程序的资源文件

argv:
    [...]   : 传入ts翻译文件路径（可变参数）
"""

import sys
import os
import proconfig
import xml.etree.ElementTree as XML


def run_system(argv):
    cmd = ""
    for i in argv:
        cmd += i + " "
    os.system(cmd)


# 主函数
if __name__ == "__main__":
    print("< 更新资源文件 >")
    print("[argv: {}]".format(sys.argv))

    # 判断参数
    if len(sys.argv) < 1:
        print("[argv] : [...: path]")
        exit(0)

    # 解析参数
    path_qt5 = proconfig.var_qtdir
    ls_argv = []
    for i in sys.argv[1:]:
        ls_argv.append(i)

    # 创建资源文件
    xml_root = XML.Element("RCC")
    xml_prefix = XML.SubElement(xml_root, "qresource", prefix="/")
    for i in ls_argv:
        xml_file = XML.SubElement(xml_prefix, "file", alias=i)
        xml_file.text = os.path.join(os.getcwd(), i)

    # 写入资源文件
    file_name = "app_resources"
    xml_save = os.path.join("build", file_name + ".qrc")
    xml_tree = XML.ElementTree(xml_root)
    XML.indent(xml_tree, space="  ", level=0)
    xml_tree.write(xml_save, encoding="utf-8")

    # 生成rcc目标文件（CMake无法调用时使用）
    qrccpp = os.path.join("build", "qrc_" + file_name + ".cpp")
    if os.path.exists(qrccpp) == False:
        cmd_rcc = os.path.join(path_qt5, "bin", "rcc")
        run_system([cmd_rcc, xml_save, "-o", qrccpp, "-name", file_name])

    print("< 更新资源文件完成 >\n", ls_argv)

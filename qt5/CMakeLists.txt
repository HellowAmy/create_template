# == 函数 ==
# 递归目录加入路径下所有目录
# 参数 [变量][路径]
function(bhfn_include_dirs result root_dir)
    set(all "" PARENT_SCOPE)
    list(APPEND all ${root_dir} " ")

    set(abs_dir ${CMAKE_SOURCE_DIR}/${root_dir})
    if (IS_DIRECTORY ${abs_dir})
        file(GLOB children RELATIVE ${abs_dir} ${abs_dir}/*)
        foreach(child ${children})
            if(IS_DIRECTORY ${abs_dir}/${child})
                bhfn_include_dirs(all ${root_dir}/${child})
            endif()
        endforeach()
        set(${result} ${all} PARENT_SCOPE)
    endif()
endfunction()


# 递归目录加入路径下所有源文件
# 参数 [变量][路径]
function(bhfn_srcs result root_dir)
    file(GLOB_RECURSE SRC "${root_dir}/*.cpp")
    set(${result} ${SRC} PARENT_SCOPE)
endfunction()


# 递归获取目录下的库文件
# 参数 [变量][路径]
function(bhfn_libs result root_dir)
    file(GLOB_RECURSE reslib "${root_dir}/*.so*")
    set(${result} ${reslib} PARENT_SCOPE)
endfunction()


# 递归获取目录下的库文件
# 参数 [变量][路径]
function(bhfn_run_bash tarname pyname argv)
    add_custom_target(${tarname}
        COMMAND python3 ${CMAKE_SOURCE_DIR}/bash/${pyname}.py ${argv}  
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}  
    )
endfunction()
# == 函数 ==



# ===== 编译入口 =====
# 项目名
cmake_minimum_required(VERSION 3.20)
project(hellow LANGUAGES CXX)

# 调试模式
set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_BUILD_TYPE "Release")

# 启用CPP11版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# 查找Qt模块
find_package(Qt5 REQUIRED COMPONENTS Core Widgets LinguistTools)

# 开启Qt自动编译
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 写入翻译文件
set(SARG_TR
    file/tr/app_en_US.ts
    file/tr/app_ja_JP.ts
    file/tr/app_zh_CN.ts
)
bhfn_run_bash(SHELL_TR maketr "${SARG_TR}")

# 写入资源文件
set(SARG_QRC
    file/css/common.css
)
bhfn_run_bash(SHELL_QRC makeqrc "${SARG_QRC}")

# 加载资源文件
set(QRC_LIST
    build/app_translations.qrc
    build/app_resources.qrc
)
qt5_add_resources(QRC_FILE ${QRC_LIST})

# 载入头文件
bhfn_include_dirs(HEAD_util util)
bhfn_include_dirs(HEAD_window window)
include_directories(${HEAD_util} ${HEAD_window})

# 载入源文件与生成可执行程序
bhfn_srcs(SRC_window window)
bhfn_srcs(SRC_util util)
set(SRC_ALL 
    main.cpp
    ${SRC_window}
    ${SRC_util}
    ${QRC_FILE}
)

# 生成目标
add_executable(${CMAKE_PROJECT_NAME} ${SRC_ALL})

# 连接Qt模块
target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
    Qt::Core
    Qt::Widgets
)

# 添加到自动构建脚本列表
add_dependencies(${CMAKE_PROJECT_NAME} SHELL_TR)
add_dependencies(${CMAKE_PROJECT_NAME} SHELL_QRC)

# 指定安装脚本
install(SCRIPT "${CMAKE_SOURCE_DIR}/install.cmake")
 
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
# == 函数 ==



# ===== 编译入口 =====
# 项目名
cmake_minimum_required(VERSION 3.20)
project(hellow LANGUAGES CXX)

# 调试模式
set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_BUILD_TYPE "Release")

# 载入头文件
bhfn_include_dirs(HEAD_third third)
bhfn_include_dirs(HEAD_util util)
bhfn_include_dirs(HEAD_window window)
include_directories(${HEAD_third} ${HEAD_util} ${HEAD_window})

# 查找Qt模块
find_package(Qt6 REQUIRED COMPONENTS Core Widgets LinguistTools)

# 设置Qt基础配置
qt_standard_project_setup()

# 载入源文件与生成可执行程序
bhfn_srcs(SRC_window window)
bhfn_srcs(SRC_util util)
set(SRC_ALL 
    main.cpp
    ${SRC_window}
    ${SRC_util}
)
qt_add_executable(${CMAKE_PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SRC_ALL})

# 生成翻译文件
qt_add_translations(
    TARGETS ${CMAKE_PROJECT_NAME}
    TS_FILES file/tr/app_zh_CN.ts
    TS_FILES file/tr/app_en_US.ts
    TS_FILES file/tr/app_ja_JP.ts
)

# 添加资源文件
qt_add_resources(${CMAKE_PROJECT_NAME} "app_resources"
    PREFIX "/"
    FILES
        file/css/common.css
)

# 连接Qt模块
target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
    Qt::Core
    Qt::Widgets
)

# 设置安装位置
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/package)

# 安装目录格式和启动程序
include(GNUInstallDirs)
install(TARGETS ${CMAKE_PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# 安装插件与翻译文件
qt_generate_deploy_app_script(
    TARGET ${CMAKE_PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})


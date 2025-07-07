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
bhfn_include_dirs(HEAD_include include)
bhfn_include_dirs(HEAD_src src)
include_directories(${HEAD_include} ${HEAD_src})

# 载入源文件与生成可执行程序
bhfn_srcs(SRC_src src)
set(SRC_ALL 
    main.cpp
    ${SRC_src}
)

# 生成程序
add_executable(${CMAKE_PROJECT_NAME} ${SRC_ALL})

# 连接模块
target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
)
 
 


# 递归获取目录下的库文件
# 参数 [变量][路径]
function(bhfn_run_bash pyname argv)
    execute_process( 
        COMMAND python3 ${CMAKE_SOURCE_DIR}/../bash/${pyname}.py ${argv} 
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}  
    )
endfunction()

# 复制文件或目录到路径
# 参数 [文件][路径]
function(bhfn_copy_lib files dest)
    file(COPY ${files} DESTINATION ${DIR_SOURCE}/package/${dest})
endfunction()


# 启动安装程序
set(SARG_INSTALL
    ${CMAKE_SOURCE_DIR}/../package
)
bhfn_run_bash(installpro "${SARG_INSTALL}")

# 复制依赖文件
set(DIR_SOURCE ${CMAKE_SOURCE_DIR}/../)
bhfn_copy_lib("${DIR_SOURCE}/lib" ".")
bhfn_copy_lib("${DIR_SOURCE}/config" ".")

 


 
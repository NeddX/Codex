if (NOT EXISTS "D:/dev/Codex/builds/vs2022/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: \"D:/dev/Codex/builds/vs2022/install_manifest.txt\"")
endif(NOT EXISTS "D:/dev/Codex/builds/vs2022/install_manifest.txt")

file(READ "D:/dev/Codex/builds/vs2022/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach (file ${files})
    message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
    execute_process(
        COMMAND C:/Users/Nedd/scoop/apps/cmake/3.26.3/bin/cmake.exe -E remove "$ENV{DESTDIR}${file}"
        OUTPUT_VARIABLE rm_out
        RESULT_VARIABLE rm_retval
    )
    if(NOT ${rm_retval} EQUAL 0)
        message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif (NOT ${rm_retval} EQUAL 0)
endforeach(file)


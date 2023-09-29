cmake_minimum_required(VERSION 3.10)

set(FYTLOGGER_VERSION 1.0.0)

project(fytlogger VERSION ${FYTLOGGER_VERSION} LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -O3)
find_package(fmt REQUIRED)

add_library(fytlogger SHARED src/logger.cpp src/writer.cpp)

target_include_directories(fytlogger PUBLIC 
"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(fytlogger fmt::fmt)

# install 
# 设置常用的路径
message(STATUS "Generating install")
set(project_config_in "${CMAKE_CURRENT_LIST_DIR}/cmake/fytloggerConfig.cmake.in")
set(project_config_out "${CMAKE_CURRENT_BINARY_DIR}/fytloggerConfig.cmake")
set(config_targets_file "fytloggerConfigTargets.cmake")
set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/fytloggerConfigVersion.cmake")
set(export_dest_dir "${CMAKE_INSTALL_LIBDIR}/cmake/fytlogger")

# 生成.cmake文件
install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}") 
install(TARGETS fytlogger
        EXPORT fytlogger
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
export(TARGETS fytlogger 
      NAMESPACE fytlogger:: 
      FILE "${CMAKE_CURRENT_BINARY_DIR}/${config_targets_file}")
install(EXPORT fytlogger
        DESTINATION ${export_dest_dir}
        NAMESPACE fytlogger::
        FILE ${config_targets_file}
)

# 安装
include(CMakePackageConfigHelpers)
configure_package_config_file("${project_config_in}" "${project_config_out}"
                              INSTALL_DESTINATION ${export_dest_dir})
write_basic_package_version_file(${version_config_file}
                                 COMPATIBILITY SameMajorVersion)
install(FILES "${project_config_out}" "${version_config_file}" DESTINATION "${export_dest_dir}")
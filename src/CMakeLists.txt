cmake_minimum_required(VERSION 3.15)
project(LuaIntf LANGUAGES CXX)
include(GNUInstallDirs)

option(ENABLE_BENCH, "Build Benchmarks")

find_package(Lua REQUIRED)

add_library(LuaIntf SHARED
    CppBindClass.cpp
    CppBindModule.cpp
    CppFunction.cpp
    CppObject.cpp
    LuaCompat.cpp
    LuaRef.cpp
    LuaState.cpp
)

target_link_libraries(LuaIntf
    PUBLIC
        ${LUA_LIBRARIES}
)

target_include_directories(LuaIntf
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(ENABLE_BENCH)
    find_package(benchmark REQUIRED)
    add_executable(bench
        bench/main.cpp
        bench/string.cpp
        bench/unordered_map.cpp
        bench/vector.cpp
    )
    target_link_libraries(bench benchmark::benchmark LuaIntf)
endif()

install(
    FILES
        include/impl/CppArg.h
        include/impl/CppBindClass.h
        include/impl/CppBindModule.h
        include/impl/CppFunction.h
        include/impl/CppInvoke.h
        include/impl/CppObject.h
        include/impl/LuaException.h
        include/impl/LuaType.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LuaIntf/impl
)

install(
    FILES
        include/LuaCompat.h
        include/LuaContext.h
        include/LuaIntf.h
        include/LuaRef.h
        include/LuaState.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LuaIntf
)

target_compile_definitions(LuaIntf PUBLIC -DLUAINTF_HEADERS_ONLY=0)
target_compile_features(LuaIntf PUBLIC cxx_std_17)

install(
    TARGETS LuaIntf
    EXPORT LuaIntfTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/LuaIntf)

install(
    EXPORT LuaIntfTargets
    FILE LuaIntfConfig.cmake
    NAMESPACE LuaIntf::
    DESTINATION ${INSTALL_CONFIGDIR}
)

export(TARGETS LuaIntf NAMESPACE LuaIntf:: FILE LuaIntfTargets.cmake)

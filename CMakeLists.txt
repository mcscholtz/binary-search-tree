cmake_minimum_required (VERSION 3.9.2)
project (ProjectName)
enable_testing()
add_subdirectory(tests)
include(Project.cmake)

#Project Sources
set (Q_SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/list-queue/src)
set (PROJECT_SOURCES 
    ${PROJECT_SOURCE_DIR}/bst.c
    ${Q_SOURCE_DIR}/queue.c
)

#LL Include dir for headers
INCLUDE_DIRECTORIES(${Q_SOURCE_DIR})

#Add the library as static
ADD_LIBRARY( ${PROJECT_LIB} STATIC ${PROJECT_SOURCES} )

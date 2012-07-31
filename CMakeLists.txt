project(qlocate)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)

include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(qlocate_SRCS
    BaseForm.cpp
    builddbcommand.cpp
    ConfigForm.cpp
    enviro.cpp
    filestat.cpp
    getdatacommand.cpp
    main.cpp
    rpmstat.cpp
    themenu.cpp
    utils.cpp
)
set(qlocate_MOC_HDRS
    BaseForm.h
    ConfigForm.h
    enviro.h
    filestat.h
    getdatacommand.h
    rpmstat.h
    themenu.h
)
set(qlocate_UIS
    ui/BaseForm.ui
    ui/ConfigForm.ui
)

QT4_WRAP_CPP(qlocate_MOC_SRCS ${qlocate_MOC_HDRS})
QT4_WRAP_UI(qlocate_UIS_H ${qlocate_UIS})

add_executable(qlocate ${qlocate_SRCS} ${qlocate_UIS_H} ${qlocate_MOC_SRCS})
target_link_libraries(qlocate ${QT_LIBRARIES})
install(TARGETS qlocate RUNTIME DESTINATION bin)

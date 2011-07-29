include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

FIND_PACKAGE(VTK)
INCLUDE(${VTK_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/whipple/include)
set(obdgui_sources main.cpp mainwindow.cpp parameters.cpp myquprighttab.cpp
    myqsteadytab.cpp myqsimtab.cpp myqwhipple.cpp myvtkTriad.cpp
    myvtkHolonomic.cpp)
set(obdgui_headers mainwindow.h parameters.h myqsimtab.h myqsteadytab.h
    myquprighttab.h myqwhipple.h myvtkTriad.h myvtkHolonomic.h)

qt4_wrap_cpp(obdgui_headers_moc ${obdgui_headers})

add_executable(obdgui
               ${obdgui_sources}
               ${obdgui_headers_moc})

target_link_libraries(obdgui
               whipple
               ${GSL_LIBRARIES}
               ${GSLCBLAS_LIBRARIES}
               ${QT_LIBRARIES}
               ${QT_QTMAIN_LIBRARY}
               QVTK
               vtkHybrid
               vtkRendering
               vtkCharts)


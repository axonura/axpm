add_library(libaxpm SHARED
    pkgparser.cpp
)

target_link_libraries(libaxpm PRIVATE LibArchive::LibArchive jsoncpp)

add_executable(axpm
    main.cpp
)

target_link_libraries(axpm PRIVATE libaxpm)
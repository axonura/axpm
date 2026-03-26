add_executable(calc
    source.cpp
)

add_custom_target(sample
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/package
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:calc> ${CMAKE_BINARY_DIR}/package
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/sample/manifest.json ${CMAKE_BINARY_DIR}/package
    COMMAND ${CMAKE_COMMAND} -E tar cf ${CMAKE_BINARY_DIR}/package.axpkg
            ${CMAKE_BINARY_DIR}/package

    DEPENDS calc
    COMMENT "Copying files and creating tar archive"
)
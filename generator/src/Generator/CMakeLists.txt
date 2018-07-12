set(SOURCE_FILES
        Generator.cpp
        Debug.cpp
        )

add_library(generator ${SOURCE_FILES})
target_link_libraries(generator helper)
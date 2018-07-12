set(SOURCE_FILES
        Server.cpp
        Proto.cpp
        )

add_library(server ${SOURCE_FILES})
target_link_libraries(server generator helper)
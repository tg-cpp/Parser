if( WIN32 )
    SET(PARSER_INSTALL_DESTINATION_RUNTIME bin)
    SET(PARSER_INSTALL_DESTINATION_LIBRARY lib)
    SET(PARSER_INSTALL_DESTINATION_ARCHIVE lib)
    SET(PARSER_INSTALL_DESTINATION_INCLUDE include)
else( WIN32 )
    SET(PARSER_INSTALL_DESTINATION_RUNTIME bin/Parser)
    SET(PARSER_INSTALL_DESTINATION_LIBRARY lib/Parser)
    SET(PARSER_INSTALL_DESTINATION_ARCHIVE lib/Parser)
    SET(PARSER_INSTALL_DESTINATION_INCLUDE include/Parser)
endif( WIN32 )

if( NOT WIN32 )
    # don't skip the full RPATH for the build tree
    SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

    # when building, don't use the install RPATH already
    # (but later on when installing)
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/Parser")

    # not sure if there is any use from below command
    # add the automatically determined parts of the RPATH
    # which point to directories outside the build tree to the install RPATH
    #SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

add_subdirectory(Parser)

# following is used for find_package functionality
install(FILES parser-config.cmake DESTINATION ${PARSER_INSTALL_DESTINATION_LIBRARY})
install(EXPORT Parser.package NAMESPACE tg:: DESTINATION ${PARSER_INSTALL_DESTINATION_LIBRARY})



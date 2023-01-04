# interface library for headers only module
add_library(Parser INTERFACE)
add_library(tg::Parser ALIAS Parser)
# modules linking to this library will include following
# directories. two options are provided for installed package
# and for in-tree source build
target_include_directories(Parser INTERFACE
    $<INSTALL_INTERFACE:include/Parser>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)

# libraries this module links to
target_link_libraries(Parser INTERFACE tg::Utility)

# what to do on make install
install(TARGETS Parser
        EXPORT Parser.package
        DESTINATION ${PARSER_INSTALL_DESTINATION_LIBRARY})

set(SRC_FILES
    Global.hpp
    DataLayer.hpp)

install(FILES
    ${SRC_FILES}
    DESTINATION ${PARSER_INSTALL_DESTINATION_INCLUDE}/Parser)

#
# add custom target simply for IDE to show the files
#
add_custom_target(Parser.include SOURCES
    ${SRC_FILES})

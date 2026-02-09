add_library(mod-ac130 INTERFACE)

target_sources(mod-ac130 INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/src/ac130_script.cpp"
)

target_link_libraries(mod-ac130 INTERFACE azerothcore-node)

# Copy the config file to the build directory
# This allows the server to see the .conf.dist file
if (COPY_CONF_FILES)
    get_filename_component(module_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    install(FILES "conf/ac130.conf.dist" DESTINATION "${CONF_DIR}")
endif()
function(kaccounts_add_provider provider_in_file)
    if (NOT IS_ABSOLUTE "${provider_in_file}")
        set(provider_in_file ${CMAKE_CURRENT_SOURCE_DIR}/${provider_in_file})
    endif()

    get_filename_component(provider_filename ${provider_in_file} NAME_WE)
    set(provider_file ${CMAKE_CURRENT_BINARY_DIR}/${provider_filename}.provider)
    execute_process(COMMAND intltool-merge -x -u --no-translations ${provider_in_file} ${provider_file} ERROR_VARIABLE intltool_error)
    if(intltool_error)
        message(FATAL_ERROR "error processing ${provider_in_file}: ${intltool_error}")
    endif()
    # The suffix must match whatever we set for $XDG_CURRENT_DESKTOP
    install(FILES ${provider_file} DESTINATION ${ACCOUNTS_PROVIDERS_DIR}/kde/)
endfunction()

function(kaccounts_add_service service_file_in)
    if (NOT IS_ABSOLUTE "${service_file_in}")
        set(service_file_in ${CMAKE_CURRENT_SOURCE_DIR}/${service_file_in})
    endif()

    get_filename_component(service_filename ${service_file_in} NAME_WE)
    set(service_file ${CMAKE_CURRENT_BINARY_DIR}/${service_filename}.service)
    execute_process(COMMAND intltool-merge -x -u --no-translations ${service_file_in} ${service_file} ERROR_VARIABLE intltool_error)
    if(intltool_error)
        message(FATAL_ERROR "error processing  ${service_file_in}: ${intltool_error}")
    endif()
    # The suffix must match whatever we set for $XDG_CURRENT_DESKTOP
    install(FILES ${service_file} DESTINATION ${ACCOUNTS_SERVICES_DIR}/kde/)
endfunction()

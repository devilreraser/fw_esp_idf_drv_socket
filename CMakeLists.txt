idf_build_get_property(project_dir PROJECT_DIR)

# Initialize an empty list to hold conditional requirements
set(conditionally_required_components "")

if(EXISTS "${project_dir}/components/drv_stream")
    list(APPEND conditionally_required_components "drv_stream")
endif()

if(EXISTS "${project_dir}/components/drv_console")
    list(APPEND conditionally_required_components "drv_console")
endif()

if(EXISTS "${project_dir}/components/drv_version")
    list(APPEND conditionally_required_components "drv_version")
endif()

if(EXISTS "${project_dir}/components/drv_eth")
    list(APPEND conditionally_required_components "drv_eth")
endif()

if(EXISTS "${project_dir}/components/drv_wifi")
    list(APPEND conditionally_required_components "drv_wifi")
endif()

if(EXISTS "${project_dir}/components/drv_dns")
    list(APPEND conditionally_required_components "drv_dns")
endif()

idf_component_register(SRCS "drv_socket.c" "cmd_socket.c"
                    INCLUDE_DIRS "." 
                    REQUIRES    "lwip" 
                                "console" 
                                "esp_netif"
                                "esp_wifi" 
                                ${conditionally_required_components}
                                      )
                 


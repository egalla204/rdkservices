set(PLUGIN_NAME TextToSpeech)
set(MODULE_NAME ${NAMESPACE}${PLUGIN_NAME})

set(PLUGIN_TEXTTOSPEECH_AUTOSTART "true" CACHE STRING "Automatically start TestToSpeech plugin")
set(PLUGIN_TEXTTOSPEECH_STARTUPORDER "" CACHE STRING "Automatically start TextToSpeech plugin")
set(PLUGIN_TEXTTOSPEECH_MODE "Local" CACHE STRING "Controls if the plugin should run in its own process, in process or remote")

find_package(${NAMESPACE}Plugins REQUIRED)

find_package(WPEFrameworkSecurityUtil)
if(NOT WPEFRAMEWORK_SECURITYUTIL_FOUND)
    add_definitions(-DSECURITY_TOKEN_ENABLED=0)
endif()

add_subdirectory(test)

add_library(${MODULE_NAME} SHARED
        Module.cpp
        TextToSpeech.cpp
        TextToSpeechJsonRpc.cpp
        TextToSpeechImplementation.cpp
        impl/TTSManager.cpp
        impl/TTSSpeaker.cpp
        impl/logger.cpp
        impl/TTSDownloader.cpp
        impl/TTSURLConstructer.cpp
        impl/SatToken.cpp
        )
set_target_properties(${MODULE_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES)

list(APPEND CMAKE_MODULE_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

find_package(GStreamer REQUIRED)

find_package(Curl)

find_package(${NAMESPACE}Plugins REQUIRED)

find_package(${NAMESPACE}Definitions REQUIRED)

set(AUDIO_CLIENT_LIB "")
if (BUILD_AMLOGIC)
    set(AUDIO_CLIENT_LIB "audio_client")
elseif (BUILD_REALTEK)
    set(AUDIO_CLIENT_LIB "RtkHALMisc") 
endif()

target_include_directories(${MODULE_NAME} PRIVATE ../helpers ${GSTREAMER_INCLUDES})

target_link_libraries(${MODULE_NAME} PRIVATE
    ${NAMESPACE}Plugins::${NAMESPACE}Plugins
    ${NAMESPACE}Definitions::${NAMESPACE}Definitions
    ${CURL_LIBRARY} ${GSTREAMER_LIBRARIES} ${AUDIO_CLIENT_LIB} ${WPEFRAMEWORK_SECURITYUTIL_LIBRARIES}
    )

install(TARGETS ${MODULE_NAME}
        DESTINATION lib/${STORAGE_DIRECTORY}/plugins)

write_config( TextToSpeech )

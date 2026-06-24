cmake_minimum_required(VERSION 3.16)

project(LicenseActivator VERSION 1.0 LANGUAGES CXX)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickControls2
)


set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")

find_package(OpenSSL REQUIRED)

qt_standard_project_setup(
    REQUIRES 6.8
)


qt_add_executable(${PROJECT_NAME}

    main.cpp

    src/licensegenerator.cpp
    src/licensegenerator.h
)



qt_add_qml_module(${PROJECT_NAME}

    URI ActivatorUI

    VERSION 1.0

    QML_FILES
        qml/Main.qml
)



set_target_properties(${PROJECT_NAME}
PROPERTIES
    WIN32_EXECUTABLE TRUE
)



target_link_libraries(${PROJECT_NAME}

PRIVATE

    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2

    OpenSSL::Crypto
    OpenSSL::SSL
)



# =====================
# Windows deploy
# =====================

if(WIN32)


get_target_property(QMAKE_EXE Qt6::qmake IMPORTED_LOCATION)

get_filename_component(
    QT_BIN_DIR
    "${QMAKE_EXE}"
    DIRECTORY
)


find_program(
    WINDEPLOYQT_EXECUTABLE
    windeployqt
    HINTS "${QT_BIN_DIR}"
)


if(WINDEPLOYQT_EXECUTABLE)


add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD


    COMMAND
    ${WINDEPLOYQT_EXECUTABLE}

    --qmldir
    "${CMAKE_SOURCE_DIR}/qml"

    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"

)


endif()



# keys

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD

    COMMAND ${CMAKE_COMMAND}
    -E copy_directory

    "${CMAKE_SOURCE_DIR}/keys"

    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/keys"
)



# OpenSSL dll

get_filename_component(
    OPENSSL_BIN_DIR
    "${OPENSSL_CRYPTO_LIBRARY}"
    DIRECTORY
)


if(EXISTS "${OPENSSL_BIN_DIR}/libcrypto-3-x64.dll")

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD

    COMMAND ${CMAKE_COMMAND}
    -E copy_if_different

    "${OPENSSL_BIN_DIR}/libcrypto-3-x64.dll"

    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"

)

endif()



if(EXISTS "${OPENSSL_BIN_DIR}/libssl-3-x64.dll")

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD

    COMMAND ${CMAKE_COMMAND}
    -E copy_if_different

    "${OPENSSL_BIN_DIR}/libssl-3-x64.dll"

    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"

)

endif()



endif()
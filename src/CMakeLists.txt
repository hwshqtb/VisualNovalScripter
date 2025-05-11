find_package(Qt6 REQUIRED
    COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        MultimediaWidgets
        StateMachine
)

qt_standard_project_setup()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

qt_add_executable(${PROJECT_NAME}
    LuaBasicWidget.hpp
    LuaMainWindow.hpp
    LuaEvent.hpp
    LuaTransition.hpp
    LuaState.hpp
    LuaStateMachine.hpp

    main.cpp
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC 
        liblua_static

    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Multimedia
        Qt6::MultimediaWidgets
        Qt6::StateMachine
)

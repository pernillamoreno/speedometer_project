cmake_minimum_required(VERSION 3.22) 

project(av23tr LANGUAGES CXX) 

set(CMAKE_CXX_STANDARD 14) 
set(CMAKE_CXX_EXTENSIONS OFF) 
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
add_compile_options(-Wall -Wextra -Wpedantic -Werror) 


set(CLIENT_DIR desktop/client)
set(SERVER_DIR desktop/server)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia SerialPort) 

set(CLIENT_HEADERS shared/setting.h ${CLIENT_DIR}/include/comservice.h ${CLIENT_DIR}/include/window.h ${CLIENT_DIR}/include/canvas.h) 
set(CLIENT_SOURCES ${CLIENT_DIR}/main.cpp ${CLIENT_DIR}/src/comservice.cpp ${CLIENT_DIR}/src/canvas.cpp ${CLIENT_DIR}/src/window.cpp)
set(CLIENT_LIBRARIES Qt6::Core Qt::Widgets Qt6::Multimedia)

set(SERVER_HEADERS shared/setting.h ${SERVER_DIR}/include/comservice.h ${SERVER_DIR}/include/window.h) 
set(SERVER_SOURCES ${SERVER_DIR}/main.cpp ${SERVER_DIR}/src/comservice.cpp ${SERVER_DIR}/src/window.cpp)
set(SERVER_LIBRARIES Qt6::Core Qt::Widgets)

# Fonts installation 

if(NOT EXISTS "$ENV{HOME}/.local/share/fonts/MaterialIcons.ttf") 
file(COPY "./desktop/client/res/MaterialIcons.ttf" DESTINATION "$ENV{HOME}/.local/share/fonts") 

endif() 

if(${UARTCOM} MATCHES ON)
    add_compile_definitions(UARTCOM)

    set(CLIENT_HEADERS ${CLIENT_HEADERS} ${CLIENT_DIR}/include/uartservice.h) 
    set(CLIENT_SOURCES ${CLIENT_SOURCES} ${CLIENT_DIR}/src/uartservice.cpp)
    set(CLIENT_LIBRARIES ${CLIENT_LIBRARIES} Qt6::SerialPort)

    set(SERVER_HEADERS ${SERVER_HEADERS} ${SERVER_DIR}/include/uartservice.h) 
    set(SERVER_SOURCES ${SERVER_SOURCES} ${SERVER_DIR}/src/uartservice.cpp)
    set(SERVER_LIBRARIES ${SERVER_LIBRARIES} Qt6::SerialPort)
 
else()

    set(CLIENT_HEADERS ${CLIENT_HEADERS} ${CLIENT_DIR}/include/tcpservice.h) 
    set(CLIENT_SOURCES ${CLIENT_SOURCES} ${CLIENT_DIR}/src/tcpservice.cpp)

    set(SERVER_HEADERS ${SERVER_HEADERS} ${SERVER_DIR}/include/tcpservice.h) 
    set(SERVER_SOURCES ${SERVER_SOURCES} ${SERVER_DIR}/src/tcpservice.cpp)
endif()

add_executable(client ${CLIENT_HEADERS} ${CLIENT_SOURCES}) 
target_link_libraries(client PUBLIC ${CLIENT_LIBRARIES})
target_include_directories(client PUBLIC shared ${CLIENT_DIR}/include) 

add_executable(server ${SERVER_HEADERS} ${SERVER_SOURCES}) 
target_link_libraries(server PUBLIC ${SERVER_LIBRARIES})  
target_include_directories(server PUBLIC shared ${SERVER_DIR}/include) 

add_custom_target(upload_client cd ${CMAKE_CURRENT_LIST_DIR}/esp32/client && pio run --target upload)
add_custom_target(upload_server cd ${CMAKE_CURRENT_LIST_DIR}/esp32/server && pio run --target upload)

add_custom_target(use_uart
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} ${CMAKE_COMMAND} -DUARTCOM=ON ${CMAKE_BINARY_DIR}
    COMMENT "Enabling UARTCOM and rebuilding..."
)

add_custom_target(use_tcp
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} ${CMAKE_COMMAND} -DUARTCOM=OFF ${CMAKE_BINARY_DIR}
    COMMENT "Disabling UARTCOM and rebuilding..."
)



 


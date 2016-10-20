cmake_minimum_required (VERSION 2.8)
project (Engine)

include_directories(Bowls)

file(GLOB_RECURSE SOURCES "Engine/*.cpp")

add_executable(Engine ${SOURCES})

find_package (SDL)
if (SDL_FOUND)
  include_directories(${SDL_INCLUDE_DIRS})
  target_link_libraries (Engine ${BZIP2_LIBRARIES})
endif (SDL_FOUND)

link_directories(

)



target_link_libraries(Engine
	SDL
)


cmake_minimum_required(VERSION 2.8)

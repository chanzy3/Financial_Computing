file(GLOB sourcefiles "Src/*.cpp")
file(GLOB headerfiles "*.hpp")
add_library(cfl_test STATIC ${sourcefiles} ${headerfiles})
target_link_libraries(cfl_test cfl)

configure_file (
  "${PROJECT_SOURCE_DIR}/test/Output.hpp.in"
  "${PROJECT_SOURCE_DIR}/test/Output.hpp"
  )

  

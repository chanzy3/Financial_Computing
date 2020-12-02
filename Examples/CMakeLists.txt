set(project_name "Examples")

file(GLOB sourcefiles "Src/*.cpp")
file(GLOB headerfiles "*.hpp")
add_executable(${project_name} ${sourcefiles} ${headerfiles})


target_link_libraries(${project_name} cfl)
target_link_libraries(${project_name} cfl_test)

if(${cfl-doc})
set(DOXYGEN_PROJECT_NAME ${project_name})
set(DOXYGEN_TAGFILES "${cfl_tag};${std_tag}")
set(DOXYGEN_OUTPUT_DIRECTORY "${doc_dir}/${project_name}")
doxygen_add_docs(
  doc-${project_name}
  ALL
  ${headerfiles}
  )
endif()


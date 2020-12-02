set(project_name "cfl")

file(GLOB sourcefiles "Src/*.cpp")
file(GLOB headerfiles "*.hpp" "Inline/*.hpp")
file(GLOB DOC_FILES "*.hpp")
add_library(cfl STATIC ${sourcefiles} ${headerfiles})

if(${cfl-doc})
set(DOXYGEN_PROJECT_NAME ${project_name})
set(DOXYGEN_TAGFILES ${std_tag})
set(DOXYGEN_GENERATE_TAGFILE "${doc_dir}/cfl/cfl.tag")
set(DOXYGEN_OUTPUT_DIRECTORY "${doc_dir}/${project_name}")
doxygen_add_docs(
  doc-${project_name}
  ALL
  ${DOC_FILES}
  )
endif()

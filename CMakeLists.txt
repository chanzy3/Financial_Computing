cmake_minimum_required (VERSION 3.10)

project(Course VERSION 1.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

set(cfl-doc NO)

if(${cfl-doc})
  find_package(Doxygen REQUIRED dot)
  set(DOXYGEN_EXTERNAL_GROUPS NO)
  set(doc_dir "${PROJECT_BINARY_DIR}/doc")
  set (std_tag "${PROJECT_SOURCE_DIR}/cfl/cppreference-doxygen-web.tag.xml=http://en.cppreference.com/w/")
  set(cfl_tag "${doc_dir}/cfl/cfl.tag=../../cfl/html")
endif()

# write your Student ID in ""
set (StudentID "chengzhh")

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(cfl)
add_subdirectory(test)
#add_subdirectory(Examples)

# add_subdirectory(Homework1)
add_subdirectory(Homework2)
# add_subdirectory(Homework3)
# add_subdirectory(Homework4)
# add_subdirectory(Homework5)

# add_subdirectory(Session1)
# add_subdirectory(Session2)
# add_subdirectory(Session3)
# add_subdirectory(Session4)

# add_subdirectory(SampleExam1)
# add_subdirectory(SampleExam2)
# add_subdirectory(SampleExam3)

# add_subdirectory(ReviewExam)
# add_subdirectory(Exam)

# generates hpp and sources and collect them into one list
function(collect_hpp_cpp hpp cpp)
  # collects hpp
  foreach(header ${hpp})
    list(APPEND hpp_cpp_files ${CMAKE_CURRENT_LIST_DIR}/${header}.hpp)
  endforeach()

  # collects cpp
  foreach(source ${cpp})
    list(APPEND hpp_cpp_files ${CMAKE_CURRENT_LIST_DIR}/${source}.cpp)
  endforeach()
  
  # makes variable visible in point where function was called
  set(hpp_cpp_files ${hpp_cpp_files} PARENT_SCOPE)
endfunction()

# collects files from subdirectories
function(collect_subdirs subdirs)
  foreach(subdir ${subdirs})
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/${subdir})
    list(APPEND subdirs_src ${${subdir}})
  endforeach()

  set(subdirs_src ${subdirs_src} PARENT_SCOPE)
endfunction()

# prepare project sources
macro(prepare_hpp_cpp hpp cpp subdirs)
  get_filename_component(current_dir ${CMAKE_CURRENT_LIST_DIR} NAME)

  collect_hpp_cpp("${hpp}" "${cpp}")
  list(APPEND ${current_dir} ${hpp_cpp_files})

  collect_subdirs("${subdirs}")
  list(APPEND ${current_dir} ${subdirs_src})

  set(${current_dir} ${${current_dir}} PARENT_SCOPE)
endmacro()

# adds static library
macro(generate_lib_target subdirs)
  collect_subdirs("${subdirs}") 

  add_library(${CMAKE_PROJECT_NAME}_objs OBJECT ${subdirs_src})
  target_link_libraries(${CMAKE_PROJECT_NAME}_objs PUBLIC ${link_libs})
  target_include_directories(${CMAKE_PROJECT_NAME}_objs PRIVATE "${CMAKE_CURRENT_LIST_DIR}/")
endmacro()

# adds executable target
macro(generate_exec_target subdirs is_test)
  collect_subdirs("${subdirs}") 

  if (is_test)
    get_filename_component(current_dir ${CMAKE_CURRENT_LIST_DIR} NAME)

    add_executable(${CMAKE_PROJECT_NAME}_test ${subdirs_src})
    target_link_libraries(${CMAKE_PROJECT_NAME}_test PRIVATE ${CMAKE_PROJECT_NAME}_objs gtest:gtest) 
    target_include_directories(${CMAKE_PROJECT_NAME}_objs PUBLIC "${CMAKE_SOURCE_DIR}/lib")
  else()
    add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/main.cpp ${subdirs_src})
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}_objs) 
  endif()
endmacro()
# generates headers and sources and collect them into one list
function(collect_files headers sources list_name)
  # collects headers
  foreach(header ${headers})
    list(APPEND list_files ${CMAKE_CURRENT_LIST_DIR}/${header}.hpp)
  endforeach()

  # collects sources
  foreach(source ${sources})
    list(APPEND list_files ${CMAKE_CURRENT_LIST_DIR}/${source}.cpp)
  endforeach()
  
  # makes variable visible in point where function was called
  set(${list_name} ${list_files} PARENT_SCOPE)
endfunction()

# prepare project sources
macro(prepare_sources name_headers name_sources)
  get_filename_component(current_dir ${CMAKE_CURRENT_LIST_DIR} NAME)

  collect_files("${name_headers}" "${name_sources}" ${current_dir})
  set(${current_dir} ${${current_dir}} PARENT_SCOPE)
endmacro()

# generates test targets
macro(generate_test_targets name_headers name_sources)
  get_filename_component(current_dir ${CMAKE_CURRENT_LIST_DIR} NAME)

  collect_files(${test_names} "${name_headers}" "${name_sources}" test_files)

  add_executable(${current_dir}_test ${test_files} ${common_modules} ${${current_dir}_module})
  target_link_libraries(${current_dir}_test 
    PRIVATE 
    Threads::Threads
    gtest::gtest)
endmacro()

# adds subdirectories and generates main target.
# <module_names> is list with names of modules that will be created in this macro.
macro(generate_main_target dir_names module_names)
  # adds subdirectories with their scripts
  foreach(dir_name ${dir_names})
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/${dir_name})
    list(APPEND src ${${dir_name}})
    list(APPEND module_names ${dir_name})
  endforeach()

  # adds enrty point
  list(APPEND src ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
  
  add_executable(${CMAKE_PROJECT_NAME} ${src})
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${link_libs})
endmacro()

# imports variables associated with certain modules into parent directory
macro(import_module_vars module_names)
  foreach(module_name ${module_names})
    set(${module_name}_module ${${module_name}} PARENT_SCOPE)
  endforeach()
endmacro()
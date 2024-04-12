set(skeletion_common_files
  common.hpp
  json.hpp)

set(skeletion_server_files
  server.cpp
  ${skeletion_common_files})

set(skeletion_client_files
  client.cpp
  ${skeletion_common_files})

add_executable(skeletion_server ${skeletion_server_files})
target_link_libraries(skeletion_server PRIVATE ${link_libs})

add_executable(skeletion_client ${skeletion_client_files})
target_link_libraries(skeletion_client PRIVATE ${link_libs})
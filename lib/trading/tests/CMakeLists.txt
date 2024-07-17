set(subdirs
  logic
  util)

set(is_test TRUE)

generate_exec_target("${subdirs}" ${is_test})
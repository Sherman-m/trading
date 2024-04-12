set(dir_names
  util
  trading)

generate_main_target("${dir_names}" module_names)
import_module_vars("${module_names}")
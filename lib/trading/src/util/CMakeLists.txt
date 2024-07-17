set(subdirs
  impl)

set(hpp
  tagged_numeric
  tagged
  custom_type_traits
  util)

set(cpp)

prepare_hpp_cpp("${hpp}" "${cpp}" "${subdirs}")
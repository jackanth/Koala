# cmake script for finding the cereal library (header-only)
#
# This script sets the following values:
#   - CEREAL_FOUND
#   - Cereal_INCLUDE_DIR
#

include(FindPackageHandleStandardArgs)
unset(CEREAL_FOUND)

find_path(Cereal_INCLUDE_DIR cereal/cereal.hpp)
find_package_handle_standard_args(Cereal DEFAULT_MSG Cereal_INCLUDE_DIR)

mark_as_advanced(CEREAL_FOUND Cereal_INCLUDE_DIR Cereal_INCLUDE_DIRS)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/cases.txt")
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/cases.txt" SUITC_CASES REGEX "^[a-z0-9-]+:")
foreach(line IN LISTS SUITC_CASES)
  string(REGEX MATCH "^([a-z0-9-]+):[ \t]*(.*)$" _ "${line}")
  set(name "${CMAKE_MATCH_1}")
  set(args "${CMAKE_MATCH_2}")
  add_test(NAME suitc.${name}
    COMMAND "${CMAKE_COMMAND}"
      -DSUITC=$<TARGET_FILE:suitc>
      "-DARGS=${args}"
      -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/expected/${name}.out
      -DACTUAL=${CMAKE_CURRENT_BINARY_DIR}/actual/${name}.out
      -P "${CMAKE_CURRENT_SOURCE_DIR}/run_case.cmake")
endforeach()

# Run SuitC with ARGS and compare its output with the EXPECTED file.
separate_arguments(arglist UNIX_COMMAND "${ARGS}")
execute_process(
  COMMAND "${SUITC}" ${arglist}
  OUTPUT_VARIABLE out
  ERROR_VARIABLE err
  RESULT_VARIABLE rc)
string(REPLACE "\r\n" "\n" out "${out}")
if(NOT rc EQUAL 0)
  message(FATAL_ERROR "SuitC ${ARGS} exited with ${rc}\n${err}")
endif()

if(UPDATE)
  file(WRITE "${EXPECTED}" "${out}")
  return()
endif()

file(READ "${EXPECTED}" expected)
string(REPLACE "\r\n" "\n" expected "${expected}")
if(NOT out STREQUAL expected)
  file(WRITE "${ACTUAL}" "${out}")
  message(FATAL_ERROR "Output of 'SuitC ${ARGS}' differs from ${EXPECTED}\nActual output written to ${ACTUAL}")
endif()

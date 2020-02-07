add_test(NAME door_flow_regulation COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/systemtest/door_tests/test_flow_regulation/01_door_flow_regulation.py ${jpscore_exe})
set_tests_properties(
        door_flow_regulation
        PROPERTIES LABELS "CI:FAST")

add_test(NAME door_flow_regulation_slow COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/systemtest/door_tests/test_flow_regulation_slow/02_door_flow_regulation_slow.py ${jpscore_exe})
set_tests_properties(
        door_flow_regulation_slow
        PROPERTIES LABELS "CI:MEDIUM")

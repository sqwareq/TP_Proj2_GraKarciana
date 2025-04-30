cmake_minimum_required (VERSION 3.12)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Gra_Karciana_cmake")

add_executable (Gra_Karciana_cmake "Gra_Karciana_cmake.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Gra_Karciana_cmake PROPERTY CXX_STANDARD 20)
endif()

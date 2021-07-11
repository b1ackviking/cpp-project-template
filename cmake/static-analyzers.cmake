option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
option(ENABLE_INCLUDE_WHAT_YOU_USE
       "Enable static analysis with include-what-you-use" OFF)

if(ENABLE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    set(CMAKE_CXX_CPPCHECK
        ${CPPCHECK} --enable=all --inline-suppr --error-exitcode=42
        --suppress=missingIncludeSystem --suppress=unmatchedSuppression)
    set(CMAKE_C_CPPCHECK ${CMAKE_CXX_CPPCHECK})
  else()
    message(SEND_ERROR "Cppcheck requested but executable not found")
  endif()
endif()

if(ENABLE_CLANG_TIDY)
  find_program(CLANGTIDY clang-tidy)
  if(CLANGTIDY)
    set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY}
                             -extra-arg=-Wno-unknown-warning-option)
    set(CMAKE_C_CLANG_TIDY ${CMAKE_CXX_CLANG_TIDY})
  else()
    message(SEND_ERROR "clang-tidy requested but executable not found")
  endif()
endif()

if(ENABLE_INCLUDE_WHAT_YOU_USE)
  find_program(INCLUDE_WHAT_YOU_USE include-what-you-use)
  if(INCLUDE_WHAT_YOU_USE)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE})
    set(CMAKE_C_INCLUDE_WHAT_YOU_USE ${CMAKE_CXX_INCLUDE_WHAT_YOU_USE})
  else()
    message(
      SEND_ERROR "include-what-you-use requested but executable not found")
  endif()
endif()

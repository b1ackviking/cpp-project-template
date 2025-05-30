option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
option(ENABLE_IPO
       "Enable Interprocedural Optimization, aka Link Time Optimization (LTO)"
       OFF)
option(ENABLE_CACHE "Enable cache if available" OFF)
option(ENABLE_ASAN "Enable address sanitizer" OFF)
if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang|GNU")
  option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
  option(ENABLE_LSAN "Enable leak sanitizer" OFF)
  option(ENABLE_UBSAN "Enable undefined behavior sanitizer" OFF)
  option(ENABLE_TSAN "Enable thread sanitizer" OFF)
  option(ENABLE_FORTIFY_SOURCE
         "Enable -D_FORTIFY_SOURCE=3 (requires optimized build)" OFF)
endif()
option(ENABLE_HARDENINGS "Enable hardenings" OFF)

# Useful CMake defaults
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_LINK_WHAT_YOU_USE TRUE)
set(CMAKE_VS_JUST_MY_CODE_DEBUGGING ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_GTEST_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)
set(CMAKE_CTEST_ARGUMENTS --progress --output-on-failure)
list(APPEND CMAKE_CTEST_ARGUMENTS --parallel)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(
    STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE
      RelWithDebInfo
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui, ccmake
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                               "MinSizeRel" "RelWithDebInfo")
endif()

# This function will prevent in-source builds
function(assure_out_of_source_builds)
  # make sure the user doesn't play dirty with symlinks
  get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
  get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

  # disallow in-source builds
  if(srcdir PATH_EQUAL bindir)
    message("######################################################")
    message("Warning: in-source builds are disabled")
    message("Please create a separate build directory and run cmake from there")
    message("######################################################")
    message(FATAL_ERROR "Quitting configuration")
  endif()
endfunction()
assure_out_of_source_builds()

if(ENABLE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    set(CMAKE_CXX_CPPCHECK
        ${CPPCHECK}
        -v
        --enable=all
        --inline-suppr
        --error-exitcode=42
        --suppress=missingIncludeSystem
        --suppress=unmatchedSuppression)
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

if(ENABLE_IPO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT result OUTPUT output)
  if(result)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(SEND_ERROR "IPO is not supported: ${output}")
  endif()
endif()

if(ENABLE_CACHE)
  set(CACHE_OPTION
      "ccache"
      CACHE STRING "Compiler cache to be used")
  set(CACHE_OPTION_VALUES "ccache" "sccache")
  set_property(CACHE CACHE_OPTION PROPERTY STRINGS ${CACHE_OPTION_VALUES})
  list(FIND CACHE_OPTION_VALUES ${CACHE_OPTION} CACHE_OPTION_INDEX)

  if(${CACHE_OPTION_INDEX} EQUAL -1)
    message(STATUS "Using custom compiler cache system: '${CACHE_OPTION}', "
                   "explicitly supported entries are ${CACHE_OPTION_VALUES}")
  endif()

  find_program(CACHE_BINARY ${CACHE_OPTION})
  if(CACHE_BINARY)
    message(STATUS "${CACHE_OPTION} found and enabled")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CACHE_BINARY})
  else()
    message(
      WARNING "${CACHE_OPTION} is enabled but was not found. Not using it")
  endif()
endif()

# Apply the default set of warnings to the target
function(set_target_warnings target_name)
  set(msvc_warnings
      /W4 # Baseline reasonable warnings
      /w14242 # 'identifier': conversion from 'type1' to 'type1', possible loss
              # of data
      /w14254 # 'operator': conversion from 'type1:field_bits' to
              # 'type2:field_bits', possible loss of data
      /w14263 # 'function': member function does not override any base class
              # virtual member function
      /w14265 # 'classname': class has virtual functions, but destructor is not
              # virtual instances of this class may not be destructed correctly
      /w14287 # 'operator': unsigned/negative constant mismatch
      /we4289 # nonstandard extension used: 'variable': loop control variable
              # declared in the for-loop is used outside the for-loop scope
      /w14296 # 'operator': expression is always 'boolean_value'
      /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
      /w14545 # expression before comma evaluates to a function which is missing
              # an argument list
      /w14546 # function call before comma missing argument list
      /w14547 # 'operator': operator before comma has no effect; expected
              # operator with side-effect
      /w14549 # 'operator': operator before comma has no effect; did you intend
              # 'operator'?
      /w14555 # expression has no effect; expected expression with side- effect
      /w14619 # pragma warning: there is no warning number 'number'
      /w14640 # Enable warning on thread un-safe static member initialization
      /w14826 # Conversion from 'type1' to 'type_2' is sign-extended. This may
              # cause unexpected runtime behavior.
      /w14905 # wide string literal cast to 'LPSTR'
      /w14906 # string literal cast to 'LPWSTR'
      /w14928 # illegal copy-initialization; more than one user-defined
              # conversion has been implicitly applied
      /permissive- # standards conformance mode for MSVC compiler.
  )
  set(clang_warnings
      -Wall
      -Wextra # reasonable and standard
      -Wpedantic # warn if non-standard C++ is used
      -Wshadow # warn the user if a variable declaration shadows one from a
               # parent context
      -Wnon-virtual-dtor # warn the user if a class with virtual functions has a
                         # non-virtual destructor. This helps catch hard to
                         # track down memory errors
      -Wold-style-cast # warn for c-style casts
      -Wcast-align # warn for potential performance problem casts
      -Wunused # warn on anything being unused
      -Woverloaded-virtual # warn if you overload (not override) a virtual
                           # function
      -Wconversion # warn on type conversions that may lose data
      -Wsign-conversion # warn on sign conversions
      -Wnull-dereference # warn if a null dereference is detected
      -Wdouble-promotion # warn if float is implicit promoted to double
      -Wformat=2 # warn on security issues around functions that format output
                 # (ie printf)
  )
  set(gcc_warnings
      ${clang_warnings}
      -Wmisleading-indentation # warn if indentation implies blocks where blocks
                               # do not exist
      -Wduplicated-cond # warn if if / else chain has duplicated conditions
      -Wduplicated-branches # warn if if / else branches have duplicated code
      -Wlogical-op # warn about logical operations being used where bitwise were
                   # probably wanted
      -Wuseless-cast # warn if you perform a cast to the same type
  )
  if(MSVC)
    set(warnings ${msvc_warnings})
  elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(warnings ${clang_warnings})
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(warnings ${gcc_warnings})
  else()
    message(
      AUTHOR_WARNING
        "No compiler warnings set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
  endif()
  target_compile_options(${target_name} INTERFACE ${warnings})
endfunction()

function(enable_coverage target_name)
  if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES ".*Clang|GNU")
    target_compile_options(${target_name} INTERFACE --coverage -O0 -g)
    target_link_libraries(${target_name} INTERFACE --coverage)
  endif()
endfunction()

function(enable_sanitizers target_name)
  set(sanitizers "")

  if(ENABLE_ASAN)
    list(APPEND sanitizers "address")
  endif()

  if(ENABLE_LSAN)
    list(APPEND sanitizers "leak")
  endif()

  if(ENABLE_UBSAN)
    list(APPEND sanitizers "undefined")
  endif()

  if(ENABLE_TSAN)
    if("address" IN_LIST sanitizers OR "leak" IN_LIST sanitizers)
      message(WARNING "Thread sanitizer does not work with Address "
                      "and Leak sanitizer enabled")
    else()
      list(APPEND sanitizers "thread")
    endif()
  endif()

  list(JOIN sanitizers "," list_of_sanitizers)
  if(list_of_sanitizers AND NOT list_of_sanitizers STREQUAL "")
    if(MSVC)
      target_compile_options(
        ${target_name} INTERFACE /fsanitize=${list_of_sanitizers} /Zi
                                 /INCREMENTAL:NO)
      target_compile_definitions(
        ${target_name} INTERFACE _DISABLE_VECTOR_ANNOTATION
                                 _DISABLE_STRING_ANNOTATION)
      target_link_options(${target_name} INTERFACE /INCREMENTAL:NO)
    else()
      target_compile_options(${target_name}
                             INTERFACE -fsanitize=${list_of_sanitizers})
      target_link_options(${target_name} INTERFACE
                          -fsanitize=${list_of_sanitizers})
    endif()
  endif()
endfunction()

include(CheckCXXCompilerFlag)
function(enable_hardenings target_name)
  if(ENABLE_HARDENINGS)
    if(MSVC)
      target_compile_options(${target_name} INTERFACE /sdl /guard:cf /GS)
      target_link_options(
        ${target_name}
        INTERFACE
        /NXCOMPAT
        /CETCOMPAT
        /DYNAMICBASE
        /LARGEADDRESSAWARE
        /HIGHENTROPYVA)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang|GNU")
      target_compile_definitions(${target_name} INTERFACE _GLIBCXX_ASSERTIONS)
      if(ENABLE_FORTIFY_SOURCE)
        target_compile_options(${target_name} INTERFACE -U_FORTIFY_SOURCE
                                                        -D_FORTIFY_SOURCE=3)
      endif()
      if(LINUX)
        target_link_options(${target_name} INTERFACE -Wl,-z,noexecstack)
      endif()

      check_cxx_compiler_flag(-fpie PIE)
      if(PIE)
        target_compile_options(${target_name} INTERFACE -fpie)
        target_link_options(${target_name} INTERFACE -fpie)
      endif()

      check_cxx_compiler_flag(-fstack-protector-strong STACK_PROTECTOR)
      if(STACK_PROTECTOR)
        target_compile_options(${target_name}
                               INTERFACE -fstack-protector-strong)
      endif()

      check_cxx_compiler_flag(-fcf-protection CF_PROTECTION)
      if(CF_PROTECTION)
        target_compile_options(${target_name} INTERFACE -fcf-protection)
      endif()

      check_cxx_compiler_flag(-fstack-clash-protection CLASH_PROTECTION)
      if(CLASH_PROTECTION AND (LINUX OR CMAKE_CXX_COMPILER_ID MATCHES "GNU"))
        target_compile_options(${target_name}
                               INTERFACE -fstack-clash-protection)
      endif()
    endif()
  endif()
endfunction()

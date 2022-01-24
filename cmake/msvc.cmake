option(ENABLE_MSVC_STATIC_RUNTIME_LIBRARY "Link against static MSVC runtime"
       OFF)

if(ENABLE_MSVC_STATIC_RUNTIME_LIBRARY)
  set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif()

set(CMAKE_C_COMPILER cl)
set(CMAKE_CXX_COMPILER cl)
set(CMAKE_AR lib)
set(CMAKE_RC_COMPILER rc)

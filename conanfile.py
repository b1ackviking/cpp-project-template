from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain

class Example(ConanFile):
  settings = "os", "arch", "compiler", "build_type"

  def requirements(self):
    self.requires("gtest/1.11.0")

  def generate(self):
    deps = CMakeDeps(self)
    deps.generate()
    tc = CMakeToolchain(self)
    tc.generate()

  def build(self):
    cmake = CMake(self)
    if self.should_configure:
      cmake.configure()
    if self.should_build:
      cmake.build();
    if self.should_test:
      cmake.test()
    if self.should_install:
      cmake.install()

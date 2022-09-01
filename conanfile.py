from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain


class Example(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    options = {
        "fPIC": [True, False]
    }
    default_options = {"fPIC": True}

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("fmt/9.0.0")
        if not self.conf.get("tools.build:skip_test"):
            self.requires("gtest/1.12.1")

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
            cmake.build()
        if self.should_test and not self.conf.get("tools.build:skip_test"):
            cmake.test()
        if self.should_install:
            cmake.install()

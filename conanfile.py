from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain


class Example(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    options = {
        "fPIC": [True, False],
        "build_testing": [True, False]
    }
    default_options = {"fPIC": True, "build_testing": True}

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("fmt/9.0.0")
        if self.options.build_testing:
            self.requires("gtest/1.12.1")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self, "Ninja")
        tc.variables["BUILD_TESTING"] = self.options.build_testing
        tc.generate()

    def build(self):
        cmake = CMake(self)
        if self.should_configure:
            cmake.configure()
        if self.should_build:
            cmake.build()
        if self.should_test and self.options.build_testing:
            cmake.test()
        if self.should_install:
            cmake.install()

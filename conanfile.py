from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.env import VirtualBuildEnv


class Example(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    languages = "C", "C++"
    options = {
        "fPIC": [True, False]
    }
    default_options = {"fPIC": True}

    def layout(self):
        cmake_layout(self)

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("fmt/12.0.0")
        if not self.conf.get("tools.build:skip_test"):
            self.requires("gtest/1.17.0")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()
        be = VirtualBuildEnv(self)
        be.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if not self.conf.get("tools.build:skip_test", default=False):
            cmake.test()

    def package(self):
        CMake(self).install()

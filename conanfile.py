from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
from conan.tools.layout import basic_layout

class BoostGTestConan(ConanFile):
    name = "boost_gtest_project"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    requires = (
        "asio/1.32.0",
        "gtest/1.15.0",
        "nlohmann_json/3.11.3",
    )

    def layout(self):
        basic_layout(self)

    def generate(self):
        # Generate CMake toolchain file
        tc = CMakeToolchain(self)
        tc.generate()

        # Generate CMake find files for dependencies
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["asio", "gtest", "gtest_main", "nlohmann_json"]

# For install run
# conan install . --output-folder=build --build=missing

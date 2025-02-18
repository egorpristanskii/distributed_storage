from conan import ConanFile

class BoostGTestConan(ConanFile):
    name = "boost_gtest_project"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"

    # Add dependencies: Boost (with asio) and GTest 1.15.0
    requires = (
        "boost/1.83.0",
        "gtest/1.15.0"
    )

    # Use modern Conan generators
    generators = "CMakeDeps", "CMakeToolchain"

    def layout(self):
        from conan.tools.layout import basic_layout
        basic_layout(self)

    def generate(self):
        # Force Conan to use Clang-16 explicitly
        self.conf.define("tools.build:compiler", "clang-19")
        self.conf.define("tools.build:compiler_version", "19")  # Adjust as needed
        self.conf.define("tools.build:compiler.libcxx", "libc++")

    def build(self):
        cmake = self.cmake
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = self.cmake
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["boost", "gtest", "gtest_main"]

# For install run
# conan install . --output-folder=build --build=missing
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, CMakeDeps, cmake_layout


class TungstenTigerConan(ConanFile):
    name = "TungstenTiger"
    version = "0.1.0"

    # Optional metadata
    license = "MIT"
    author = "Anthony Young <anthony.r.young@gmail.com>"
    url = "https://github.com/tiger-chan/tungsten-tiger.git"
    homepage = url
    description = "ECS based game engine"
    topics = ("conan," "gaming", "entity", "ecs", "engine")

    # Binary configuration
    settings = {
        "arch": None,
        "os": None,
        "build_type": None,
        "compiler": None,
    }

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*"

    requires = [
        "bgfx/1.115.8266-06b9950",
        "sdl/2.0.20",
        "glm/0.9.9.8",
        "entt/3.10.1",
        "constr/0.1.0@tiger-chan/stable",
        "mime/0.1.0@tiger-chan/stable"
    ]

    generators = []

    def package_info(self):
        self.cpp_info.set_property("cmake_find_mode", "both")
        pass

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)

        # Extra Preprossesor definitions
        #tc.preprocessor_definitions["MY_PROJECT_DEFINITION"] = 32

        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def configure(self):
        self.options["bgfx"].build_tools = True

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

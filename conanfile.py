import os

from conan import ConanFile
from conan.tools.cmake import cmake_layout

class CurrencyMarketReceipe(ConanFile):
  settings = "compiler", "build_type"
  generators = "CMakeToolchain", "CMakeDeps"

  def requirements(self):
    self.requires("boost/1.84.0")
    
  def build_requirements(self):
    self.tool_requires("cmake/3.22.6")

  def layout(self):
    cmake_layout(self)
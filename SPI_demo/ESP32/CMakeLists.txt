# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS main.c spi_gpio.c spi_m.c        # list the source files of this component
    INCLUDE_DIRS "."       # optional, add here public include directories
)

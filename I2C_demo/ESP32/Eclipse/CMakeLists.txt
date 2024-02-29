# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS main.c i2c_s.c gpio_config.c   # list the source files of this component
    INCLUDE_DIRS "."    				# optional, add here public include directories
#    REQUIRES esp_driver_i2c
#PRIV_REQUIRES esp_driver_i2c
)

include(FetchContent)

FetchContent_Declare(
  adxl343
  GIT_REPOSITORY https://github.com/pcjpl/pico-adxl343 
  GIT_TAG        main
)

FetchContent_MakeAvailable(adxl343)

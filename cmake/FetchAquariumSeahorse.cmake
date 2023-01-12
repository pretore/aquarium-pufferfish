include(FetchContent)

FetchContent_Declare(
        aquarium-seahorse
        GIT_REPOSITORY https://github.com/pretore/aquarium-seahorse.git
        GIT_TAG v2.3.1
        GIT_SHALLOW 1
)

FetchContent_MakeAvailable(aquarium-seahorse)

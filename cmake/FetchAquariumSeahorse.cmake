include(FetchContent)

FetchContent_Declare(
        aquarium-seahorse
        GIT_REPOSITORY https://github.com/pretore/aquarium-seahorse.git
        GIT_TAG v3.0.0
        GIT_SHALLOW 1
)

FetchContent_MakeAvailable(aquarium-seahorse)

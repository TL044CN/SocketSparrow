find_package(Git)
# PROJECT_REVISION is the version tag that points to the latest tagged commit,
# sanitized to only be numbers delimited by ".".
set(PROJECT_REVISION "" CACHE INTERNAL "")

if(GIT_FOUND)
    # find the correct version tag, given a text tag (e.g. experimental, latest) or version tag.
    # If the textTag is not a version tag (e.g. v1.0.0), then it will find the version tag
    # that points to the commit hash. The version tag is then stored in PROJECT_REVISION.
    function(findVersionTag textTag)
        set(VERSION_REGEX "^[vV]?[0-9]+(\.[0-9]+)*([a-zA-Z]*)$")
        string(REGEX MATCH ${VERSION_REGEX} MATCHED_VERSION ${textTag})

        # if not a version tag, find the version tag that points to the commit hash.
        if(NOT MATCHED_VERSION)
            execute_process(
                COMMAND ${GIT_EXECUTABLE} rev-list -n 1 ${textTag}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                OUTPUT_VARIABLE GIT_COMMIT_HASH
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )

            # get all tags that point to the commit hash
            execute_process(
                COMMAND ${GIT_EXECUTABLE} tag --points-at ${GIT_COMMIT_HASH}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                OUTPUT_VARIABLE TAGS_STRING
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            string(REPLACE "\n" ";" TAGS_LIST ${TAGS_STRING})

            # find the version tag
            foreach(tag ${TAGS_LIST})
                string(REGEX MATCH ${VERSION_REGEX} GIT_TAG_VERSION ${tag})
                if(GIT_TAG_VERSION)
                    break()
                endif()
            endforeach()
        endif()

        # sanitize the version tag to only be numbers delimited by "."
        string(REGEX REPLACE "[^0-9.]" "" PROJECT_REVISION "${GIT_TAG_VERSION}")
        # persist the version tag
        set(PROJECT_REVISION "${PROJECT_REVISION}" CACHE INTERNAL "")
    endfunction()

    # get the latest tag
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_TAG_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(NOT GIT_TAG_VERSION)
        set(GIT_TAG_VERSION "0.0.1")
    endif()

    # find the appropriate version tag
    findVersionTag(${GIT_TAG_VERSION})

    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMODULE_RESULT
        )
        if(NOT GIT_SUBMODULE_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMODULE_RESULT}")
        endif()
    endif()

else()
    # if git is not found, default to 0.0.1
    set(PROJECT_REVISION "0.0.1")
endif()

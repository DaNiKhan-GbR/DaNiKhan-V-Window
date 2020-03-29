###############################################################################
# DaNiKhan Virtual Window (dnkvw) devtool Cmake file                          #
#                                                                             #
# This is a sub cmake file for the dnkvw devtool.                             #
# DO NOT USE THIS DIRECTLY AS A CMAKE PROJECT.                                #
# ONLY USE THE MAIN CMakeLists.txt!                                           #
#                                                                             #
# In-Source builds are explicitly disabled!                                   #
# Please refere to the README.md for build instructions.                      #
###############################################################################

project(dnkvwtool CXX)

# The devtool istself.
add_executable(${PROJECT_NAME}
    src/devtool.cpp
)

# Link with our library.
target_link_libraries(${PROJECT_NAME} dnkvw)

# END OF FILE

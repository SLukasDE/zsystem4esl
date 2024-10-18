include(CMakeFindDependencyMacro)

find_dependency(esa)
find_dependency(esl)
#find_dependency(common4esl)
find_dependency(zsystem)

include("${CMAKE_CURRENT_LIST_DIR}/zsystem4eslTargets.cmake")

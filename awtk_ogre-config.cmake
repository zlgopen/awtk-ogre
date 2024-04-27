set(AWTK_OGRE_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "Path to AWTK OGRE root directory")

# 查找awtk库的头文件和库文件
find_library(AWTK_OGRE_LIBRARY NAMES awtk_ogre PATHS ${AWTK_OGRE_ROOT_DIR}/lib)

if (AWTK_OGRE_LIBRARY)
    get_filename_component(AWTK_OGRE_LIBRARY_PATH ${AWTK_OGRE_LIBRARY} DIRECTORY)
else()
    message(FATAL_ERROR "Library not found " ${AWTK_OGRE_LIBRARY})
endif()

set(AWTK_OGRE_LIBRARIES ${AWTK_OGRE_LIBRARY})
set(AWTK_OGRE_INCLUDE_DIRS ${AWTK_OGRE_ROOT_DIR}/src)

include_directories(${AWTK_OGRE_ROOT_DIR}/src)
link_directories(${AWTK_OGRE_LIBRARY_PATH})

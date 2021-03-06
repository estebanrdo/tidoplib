##########################################################################
#                                                                        #
# Copyright (C) 2021 by Tidop Research Group                             #
# Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       #
#                                                                        #
# This file is part of TidopLib                                          #
#                                                                        #
# TidopLib is free software: you can redistribute it and/or modify       #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, either version 3 of the     #
# License, or (at your option) any later version.                        #
#                                                                        #
# TidopLib is distributed in the hope that it will be useful,            #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          #
# GNU Lesser General Public License for more details.                    #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with TidopLib. If not, see <http://www.gnu.org/licenses>.#
#                                                                        #
# @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         #
#                                                                        #
##########################################################################


##########################################################################
#  TidopLib image processing module                                      #
##########################################################################


if (BUILD_TL_IMG_PROCESS)

    project(tl_imgprocess)
  
    add_library(${PROJECT_NAME} ${LIB_TYPE}
                anaglyph.cpp
                anaglyph.h
                colorconvert.cpp
                colorconvert.h
                correlation.h
                filters.cpp
                filters.h
                fourier.cpp
                fourier.h
                imgprocess.cpp
                imgprocess.h
                imgtransform.cpp
                imgtransform.h
                linedetector.cpp
                linedetector.h
                skeleton.cpp
                skeleton.h
                whitebalance.cpp
                whitebalance.h
                morphologicaloper.cpp
                morphologicaloper.h)

    target_link_libraries(${PROJECT_NAME} PRIVATE
                          tl_core
                          tl_graphic
                          ${OpenCV_LIBS})  

    set_property(TARGET ${PROJECT_NAME} 
                 PROPERTY FOLDER "modules")


    if(BUILD_SHARED_LIBS)
        install(TARGETS ${PROJECT_NAME}
                EXPORT "${TARGETS_EXPORT_NAME}"
                RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
                LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
                ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib) 
  			
        set_target_properties(${PROJECT_NAME} PROPERTIES
                              DEFINE_SYMBOL TIDOPLIB_EXPORTS)
    else()
        install(TARGETS ${PROJECT_NAME} 
                EXPORT "${TARGETS_EXPORT_NAME}"
                LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
                ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
    endif()
  
    install(DIRECTORY . 
            DESTINATION include/tidop/imgprocess
            FILES_MATCHING PATTERN "*.h")
          
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})       
  
    set(TL_HAVE_IMG_PROCESS YES CACHE INTERNAL "")
    message(STATUS "  TidopLib::tl_imgprocess enabled")

elseif(BUILD_TL_CORE AND BUILD_TL_GRAPHIC)

    message(STATUS "  TidopLib::tl_imgprocess disabled")

endif(BUILD_TL_IMG_PROCESS)

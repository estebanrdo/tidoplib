##########################################################################
#                                                                        #
# Copyright (C) 2021 by Tidop Research Group                             #
# Copyright (C) 2021 by Esteban Ruiz de O�a Crespo                       #
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

# TidopLib experimental module

project(tl_experimental)

if (BUILD_TL_EXPERIMENTAL)

    add_library(${PROJECT_NAME} ${LIB_TYPE}
                desingpatterns.cpp
                designpatterns.h
                video/videoio.cpp
                video/videoio.h
                compression.cpp
                compression.h
                csv.cpp
                csv.h)
    
    #target_include_directories(${PROJECT_NAME} PUBLIC 
    #                           $<BUILD_INTERFACE:${TidopLib_SOURCE_DIR}/src>
    #                           $<INSTALL_INTERFACE:include>)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GDAL_INCLUDE_DIR})

    target_link_libraries(${PROJECT_NAME} PRIVATE 
                          tl_core
                          tl_graphic
                          ${Boost_FILESYSTEM_LIBRARY} 
                          ${Boost_SYSTEM_LIBRARY}
                          $<$<BOOL:${GDAL_FOUND}>:${GDAL_LIBRARY}>)

    if(HAVE_OPENCV)
        target_link_libraries(${PROJECT_NAME} PRIVATE 
                              ${OpenCV_LIBS})
    endif(HAVE_OPENCV)

    set_property(TARGET ${PROJECT_NAME} 
                 PROPERTY FOLDER "modules")

  #install
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
          DESTINATION include/tidop/experimental
          FILES_MATCHING PATTERN "*.h")
  
  set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})

  set(TL_HAVE_EXPERIMENTAL YES CACHE INTERNAL "")
  message(STATUS "  TidopLib::${PROJECT_NAME} enabled")

elseif(BUILD_TL_CORE AND BUILD_TL_GRAPHIC)
     
    message(STATUS "  TidopLib::${PROJECT_NAME} disabled")

endif()
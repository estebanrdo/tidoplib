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
#  TidopLib math module                                                  #
##########################################################################

if(BUILD_TL_MATH)

    project(tl_math)
    
    set(TL_MATH_SOURCES
        angles.cpp)
                
    set(TL_MATH_HEADERS
        math.h
        angles.h
        statistics.h
        mathutils.h
        simd.h
        statistic/series.h
        statistic/descriptive.h
        statistic/confmat.h
		statistic/covariance.h
        statistic/tukeyfences.h
        algebra/quaternion.h
        algebra/euler_angles.h
        algebra/rotations.h
        algebra/rotation_matrix.h
        algebra/rotation_convert.h
        algebra/axis_angle.h
        algebra/matrix.h
        algebra/vector.h
        algebra/svd.h
        algebra/lu.h
        algebra/qr.h
        algebra/cholesky.h
        lapack.h
        blas.h)
                
    add_library(${PROJECT_NAME} ${LIB_TYPE}
                ${TL_MATH_SOURCES}
                ${TL_MATH_HEADERS})

    foreach(_source IN ITEMS ${TL_MATH_SOURCES})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "/" "\\" _group_path "${_source_path}")
        source_group("Source Files\\${_group_path}" FILES "${_source}")
    endforeach()
    
    foreach(_source IN ITEMS ${TL_MATH_HEADERS})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "/" "\\" _group_path "${_source_path}")
        source_group("Header Files\\${_group_path}" FILES "${_source}")
    endforeach()
    
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE "/bigobj")
    endif(MSVC)

    target_compile_definitions(${PROJECT_NAME} PUBLIC
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:HAVE_LAPACK_CONFIG_H>
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:LAPACK_COMPLEX_STRUCTURE>)

    target_link_libraries(${PROJECT_NAME} PUBLIC 
                          tl_core
                          $<$<BOOL:${TL_HAVE_OPENBLAS}>:OpenBLAS::OpenBLAS>)
       
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
            DESTINATION include/tidop/math
            FILES_MATCHING PATTERN "*.h")
            
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
    
    set(TL_HAVE_MATH YES CACHE INTERNAL "")
    message(STATUS "  TidopLib::tl_math enabled")

elseif(BUILD_TL_CORE)
    
    message(STATUS "  TidopLib::tl_math disabled")
    message(STATUS "  TidopLib::tl_geom disabled         [Module dependent on tl_math that is deactivated]") 
    message(STATUS "  TidopLib::tl_img disabled          [Module dependent on tl_math that is deactivated]") 
    message(STATUS "  TidopLib::tl_geospatial disabled   [Module dependent on tl_math that is deactivated]") 

endif(BUILD_TL_MATH)
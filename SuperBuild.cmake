#-----------------------------------------------------------------------------
# Enable and setup External project global properties
#-----------------------------------------------------------------------------
include(ExternalProject)
set(ep_base "${CMAKE_BINARY_DIR}")

# Compute -G arg for configuring external projects with the same CMake generator:
if(CMAKE_EXTRA_GENERATOR)
  set(gen "${CMAKE_EXTRA_GENERATOR} - ${CMAKE_GENERATOR}")
else()
  set(gen "${CMAKE_GENERATOR}")
endif()

#-----------------------------------------------------------------------------
# Build options
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Project dependencies
#-----------------------------------------------------------------------------

set( inner_DEPENDENCIES "" )
  
set( PerkTutor_Modules
  PerkEvaluator
  TargetFiducials
  TransformRecorder
  WorkflowSegmentation
  )

  
foreach( proj ${PerkTutor_Modules} )
  set( inner_DEPENDENCIES ${proj}Download ${inner_DEPENDENCIES} )
  set( ${proj}_SOURCE_DIR ${CMAKE_BINARY_DIR}/${proj} )
  set( ${proj}_BINARY_DIR ${${proj}_SOURCE_DIR}-build )
  message( STATUS "Source: "${${proj}_SOURCE_DIR} )
  ExternalProject_Add(
    ${proj}Download
    GIT_REPOSITORY "https://github.com/PerkTutor/${proj}.git"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    TEST_COMMAND ""
    INSTALL_COMMAND ""
    SOURCE_DIR ${${proj}_SOURCE_DIR}
    BINARY_DIR ${${proj}_BINARY_DIR}
    )
endforeach( proj )


#SlicerMacroCheckExternalProjectDependency(inner)


set(proj inner)
ExternalProject_Add( ${proj}
  DOWNLOAD_COMMAND ""
  INSTALL_COMMAND ""
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  BINARY_DIR ${EXTENSION_BUILD_SUBDIRECTORY}
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY:PATH=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY:PATH=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY:PATH=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
    -DADDITIONAL_C_FLAGS:STRING=${ADDITIONAL_C_FLAGS}
    -DADDITIONAL_CXX_FLAGS:STRING=${ADDITIONAL_CXX_FLAGS}
    -D${EXTENSION_NAME}_SUPERBUILD:BOOL=OFF
    -DEXTENSION_SUPERBUILD_BINARY_DIR:PATH=${${EXTENSION_NAME}_BINARY_DIR}
    -DPerkEvaluator_SOURCE_DIR:PATH=${PerkEvaluator_SOURCE_DIR}
    -DPerkEvaluator_BINARY_DIR:PATH=${PerkEvaluator_BINARY_DIR}
    -DTargetFiducials_SOURCE_DIR:PATH=${TargetFiducials_SOURCE_DIR}
    -DTargetFiducials_BINARY_DIR:PATH=${TargetFiducials_BINARY_DIR}      
    -DTransformRecorder_SOURCE_DIR:PATH=${TransformRecorder_SOURCE_DIR}
    -DTransformRecorder_BINARY_DIR:PATH=${TransformRecorder_BINARY_DIR}      
    -DWorkflowSegmentation_SOURCE_DIR:PATH=${WorkflowSegmentation_SOURCE_DIR}
    -DWorkflowSegmentation_BINARY_DIR:PATH=${WorkflowSegmentation_BINARY_DIR}      
    -DSlicer_DIR:PATH=${Slicer_DIR}
    -DCTK_DIR:PATH=${CTK_DIR}
    -DQtTesting_DIR:PATH=${QtTesting_DIR}
    -DITK_DIR:PATH=${ITK_DIR}
    -DOpenIGTLink_DIR:PATH=${OpenIGTLink_DIR}
    -DPYTHON_EXECUTABLE:PATH=${PYTHON_EXECUTABLE}
    -DPYTHON_INCLUDE_DIR:PATH=${PYTHON_INCLUDE_DIR}
    -DMIDAS_PACKAGE_EMAIL:STRING=${MIDAS_PACKAGE_EMAIL}
    -DMIDAS_PACKAGE_API_KEY:STRING=${MIDAS_PACKAGE_API_KEY}    
    ${ep_cmake_args}
  DEPENDS ${${proj}_DEPENDENCIES}
  )
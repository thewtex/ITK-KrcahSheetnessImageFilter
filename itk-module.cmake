# the top-level README is used for describing this module, just
# re-used it for documentation here
get_filename_component(MY_CURRENT_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
file(READ "${MY_CURRENT_DIR}/README.md" DOCUMENTATION)

set(_extra_test_deps )
# ITKGoogleTest is only available in ITK 4.13.0 and later
if(ITK_VERSION VERSION_GREATER_EQUAL 4.13.0)
  set(_extra_test_deps ITKGoogleTest)
endif()

itk_module(KrcahSheetness
  DEPENDS
    ITKCommon
    ITKImageFeature
    ITKImageFilterBase
    ITKImageIntensity
    ITKImageStatistics
    ITKSmoothing
  TEST_DEPENDS
    ITKTestKernel
    ${_extra_test_deps}
  DESCRIPTION
    "${DOCUMENTATION}"
  EXCLUDE_FROM_DEFAULT
  ENABLE_SHARED
)

itk_module_test()

set(KrcahSheetnessTests
  itkKrcahSheetnessFeatureImageFilterTest.cxx
  )

CreateTestDriver(KrcahSheetness "${KrcahSheetness-Test_LIBRARIES}" "${KrcahSheetnessTests}")

itk_add_test(NAME itkKrcahSheetnessFeatureImageFilterTest
  COMMAND KrcahSheetnessTestDriver
  --compare
    ${ITK_TEST_OUTPUT_DIR}/itkKrcahSheetnessFeatureImageFilterTest.nii
    DATA{Baseline/itkKrcahSheetnessFeatureImageFilterTest.nii}
  itkKrcahSheetnessFeatureImageFilterTest
    DATA{Input/001-CT.nii}
    ${ITK_TEST_OUTPUT_DIR}/itkKrcahSheetnessFeatureImageFilterTest.nii
  )

# ITKGoogleTest is only available in ITK 4.13.0 and later
if(ITK_VERSION VERSION_LESS 4.13.0)
  return()
endif()

set(KrcahSheetnessGoogleTests
  test_Sheetness.cxx
  test_ModifiedSheetnessFunctor.cxx
  )

CreateGoogleTestDriver(KrcahSheetness "${KrcahSheetness-Test_LIBRARIES}" "${KrcahSheetnessGoogleTests}")

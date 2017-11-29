itk_module_test()

set(KrcahSheetnessTests
  test_Sheetness.cxx
  test_ModifiedSheetnessFunctor.cxx
  )

# ITKGoogleTest is only available in ITK 4.13.0 and later
if(ITK_VERSION VERSION_LESS 4.13.0)
  return()
endif()

CreateGoogleTestDriver(KrcahSheetness "${KrcahSheetness-Test_LIBRARIES}" "${KrcahSheetnessTests}")

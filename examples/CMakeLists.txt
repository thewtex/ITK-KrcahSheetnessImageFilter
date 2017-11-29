cmake_minimum_required(VERSION 2.8.12)
project(KrcahSheetnessExamples)

# LesionSizingToolkit contains Descoteaux sheetness
find_package(ITK REQUIRED COMPONENTS
  ITKBinaryMathematicalMorphology
  ITKCommon
  ITKConnectedComponents
  ITKDistanceMap
  ITKIOImageBase
  ITKImageFeature
  ITKImageFilterBase
  ITKImageIntensity
  ITKImageStatistics
  ITKLabelMap
  ITKMathematicalMorphology
  ITKThresholding
  ITKImageIO
  LesionSizingToolkit
  KrcahSheetness
)
include(${ITK_USE_FILE})

add_subdirectory(Descoteaux)
add_subdirectory(DescoteauxWithScaling)
add_subdirectory(DescoteauxMax)
add_subdirectory(Krcah)
add_subdirectory(ModifiedSheetness)

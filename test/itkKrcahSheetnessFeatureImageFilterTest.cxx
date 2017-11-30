/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkKrcahSheetnessFeatureImageFilter.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTestingMacros.h"

int itkKrcahSheetnessFeatureImageFilterTest( int argc, char * argv[] )
{
  if( argc < 3 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage outputImage";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }
  const char * inputImageFileName = argv[1];
  const char * outputImageFileName = argv[2];

  const unsigned int Dimension = 3;
  typedef short                                   InputPixelType;
  typedef itk::Image< InputPixelType, Dimension > InputImageType;

  typedef itk::ImageFileReader< InputImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputImageFileName );

  typedef float                                       SheetnessPixelType;
  typedef itk::Image< SheetnessPixelType, Dimension > SheetnessImageType;

  typedef itk::KrcahSheetnessFeatureImageFilter< InputImageType, SheetnessImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );

  EXERCISE_BASIC_OBJECT_METHODS( filter, KrcahSheetnessFeatureImageFilter , ImageToImageFilter );

  const double gaussianSigma = 0.5;
  filter->SetGaussianSigma( gaussianSigma );
  TEST_SET_GET_VALUE( gaussianSigma, filter->GetGaussianSigma() );
  const double scalingConstant = 1.;
  filter->SetScalingConstant( scalingConstant );
  TEST_SET_GET_VALUE( scalingConstant, filter->GetScalingConstant() );
  const double alpha = 0.2;
  filter->SetAlpha( alpha );
  TEST_SET_GET_VALUE( alpha, filter->GetAlpha() );
  const double beta = 0.01;
  filter->SetBeta( beta );
  TEST_SET_GET_VALUE( beta, filter->GetBeta() );
  const double gamma = 1035.25;
  filter->SetGamma( gamma );
  TEST_SET_GET_VALUE( gamma, filter->GetGamma() );
  FilterType::SheetnessScalesType sheetnessScales;
  sheetnessScales.push_back( 0.75 );
  sheetnessScales.push_back( 1.50 );
  filter->SetSheetnessScales( sheetnessScales );

  typedef itk::ImageFileWriter< SheetnessImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( outputImageFileName );
  writer->SetInput( filter->GetOutput() );
  writer->UseCompressionOn();
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

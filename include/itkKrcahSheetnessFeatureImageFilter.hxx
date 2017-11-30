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
#ifndef itkKrcahSheetnessFeatureImageFilter_hxx
#define itkKrcahSheetnessFeatureImageFilter_hxx

#include "itkKrcahSheetnessFeatureImageFilter.h"

namespace itk
{

template<typename TInput, typename TOutput>
KrcahSheetnessFeatureImageFilter<TInput, TOutput>
::KrcahSheetnessFeatureImageFilter()
// suggested values by Krcah el. al.
  : m_GaussianSigma(1.0) // =s
  , m_ScalingConstant(10.0) // =k
  , m_Alpha(0.5), m_Beta(0.5), m_Gamma(0.25)
{
  m_SheetnessScales.push_back(0.75);
  m_SheetnessScales.push_back(1.00);

  this->SetNumberOfRequiredInputs(1);

  m_InputCastFilter = InputCastFilterType::New();
  m_GaussianFilter = GaussianFilterType::New();
  m_SubtractFilter = SubtractFilterType::New();
  m_MultiplyFilter = MultiplyFilterType::New();
  m_AddFilter = AddFilterType::New();
  m_HessianFilter = HessianFilterType::New();
  m_EigenAnalysisFilter = EigenAnalysisFilterType::New();
  m_TraceFilter = TraceFilterType::New();
  m_StatisticsFilter = StatisticsFilterType::New();
  m_SheetnessFilter = SheetnessFilterType::New();
}


template<typename TInput, typename TOutput>
KrcahSheetnessFeatureImageFilter<TInput, TOutput>
::~KrcahSheetnessFeatureImageFilter()
{
}


template<typename TInput, typename TOutput>
void KrcahSheetnessFeatureImageFilter<TInput, TOutput>
::GenerateData()
{
  // get input
  typename InputImageType::Pointer input = InputImageType::New();
  input->Graft( const_cast< InputImageType * >( this->GetInput() ));

  // assert we have a valid m_SheetnessScales
  if(m_SheetnessScales.empty())
    {
    itkExceptionMacro( "SheetnessScales is empty!" );
    }

  TOutput * output = this->GetOutput();

  // Calculate first sheetness
  typename OutputImageType::Pointer sheetnessOutput = OutputImageType::New();
  sheetnessOutput->CopyInformation( output );
  sheetnessOutput->SetBufferedRegion( output->GetBufferedRegion() );
  sheetnessOutput->SetRequestedRegion( output->GetRequestedRegion() );
  sheetnessOutput->Allocate();

  GenerateSheetnessWithSigma(input, sheetnessOutput, m_SheetnessScales.at(0));

  if( m_SheetnessScales.size() > 1 )
    {
    typename OutputImageType::Pointer tempSheetnessOutput = OutputImageType::New();
    tempSheetnessOutput->CopyInformation( sheetnessOutput );
    tempSheetnessOutput->SetBufferedRegion( sheetnessOutput->GetBufferedRegion() );
    tempSheetnessOutput->SetRequestedRegion( sheetnessOutput->GetRequestedRegion() );
    tempSheetnessOutput->Allocate();
    for( SheetnessScalesType::iterator scalesIterator = ++(m_SheetnessScales.begin()); scalesIterator != m_SheetnessScales.end(); ++scalesIterator )
      {
      // Calculte the remaining (n-1) sheetnesses
      GenerateSheetnessWithSigma( input, tempSheetnessOutput, (*scalesIterator) );

      // Take abs max
      typename MaximumAbsoluteValueFilterType::Pointer maximumAbsoluteValueFilter = MaximumAbsoluteValueFilterType::New();
      maximumAbsoluteValueFilter->SetInput1( sheetnessOutput );
      maximumAbsoluteValueFilter->SetInput2( tempSheetnessOutput );
      maximumAbsoluteValueFilter->Update();

      // Save max and move on
      sheetnessOutput = maximumAbsoluteValueFilter->GetOutput();
      }
    }

  // copy output
  output->Graft( sheetnessOutput );
}


template<typename TInput, typename TOutput>
void
KrcahSheetnessFeatureImageFilter<TInput, TOutput>
::GenerateSheetnessWithSigma(TInput * input, TOutput * output, double sigma)
{
  /******
  * Input preprocessing
  ******/
  m_InputCastFilter->SetInput(input);

  // I*G (discrete gauss)
  m_GaussianFilter->SetVariance(m_GaussianSigma * m_GaussianSigma); // =s
  m_GaussianFilter->SetInput(m_InputCastFilter->GetOutput());

  // I - (I*G)
  m_SubtractFilter->SetInput1(m_InputCastFilter->GetOutput());
  m_SubtractFilter->SetInput2(m_GaussianFilter->GetOutput());

  // k(I-(I*G))
  m_MultiplyFilter->SetInput(m_SubtractFilter->GetOutput());
  m_MultiplyFilter->SetConstant(m_ScalingConstant); // =k

  // I+k*(I-(I*G))
  m_AddFilter->SetInput1(m_InputCastFilter->GetOutput());
  m_AddFilter->SetInput2(m_MultiplyFilter->GetOutput());

  /******
  * sheetness prerequisites
  ******/
  // hessian
  m_HessianFilter->SetSigma(sigma);
  m_HessianFilter->SetInput(m_AddFilter->GetOutput());

  // eigen analysis
  m_EigenAnalysisFilter->SetDimension(NDimension);
  m_EigenAnalysisFilter->SetInput(m_HessianFilter->GetOutput());

  // calculate trace
  m_TraceFilter->SetImageDimension(NDimension);
  m_TraceFilter->SetInput(m_HessianFilter->GetOutput());

  // calculate average
  m_StatisticsFilter->SetInput(m_TraceFilter->GetOutput());
  m_StatisticsFilter->Update(); // needed! ->GetMean() will not trigger an update!

  /******
  * Sheetness
  ******/
  m_SheetnessFilter->SetInput(m_EigenAnalysisFilter->GetOutput());
  m_SheetnessFilter->SetConstant(m_StatisticsFilter->GetMean());
  m_SheetnessFilter->SetAlpha(m_Alpha);
  m_SheetnessFilter->SetBeta(m_Beta);
  m_SheetnessFilter->SetGamma(m_Gamma);

  // return
  m_SheetnessFilter->GetOutput()->Graft( output );
  m_SheetnessFilter->Update();
  output->Graft( m_SheetnessFilter->GetOutput() );
}

} // end namespace itk

#endif // itkKrcahSheetnessFeatureImageFilter_hxx

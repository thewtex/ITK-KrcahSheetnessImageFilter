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
#ifndef itkKrcahSheetnessFeatureGenerator_hxx
#define itkKrcahSheetnessFeatureGenerator_hxx

#include "itkKrcahSheetnessFeatureGenerator.h"

namespace itk
{

template<typename TInput, typename TOutput>
KrcahSheetnessFeatureGenerator<TInput, TOutput>
::KrcahSheetnessFeatureGenerator()
// suggested values by Krcah el. al.
  : m_GaussVariance(1) // =s
  , m_ScalingConstant(10) // =k
  , m_Alpha(0.5), m_Beta(0.5), m_Gamma(0.25)
{
  m_SheetnessScales.push_back(0.75);
  m_SheetnessScales.push_back(1.00);

  this->SetNumberOfRequiredInputs(1);
}


template<typename TInput, typename TOutput>
KrcahSheetnessFeatureGenerator<TInput, TOutput>
::~KrcahSheetnessFeatureGenerator()
{
}


template<typename TInput, typename TOutput>
void KrcahSheetnessFeatureGenerator<TInput, TOutput>
::GenerateData()
{
  // get input
  typename InputImageType::ConstPointer input(this->GetInput());

  // assert we have a valid m_SheetnessScales
  assert(m_SheetnessScales.size() > 0);

  // Calculate first sheetness
  typename OutputImageType::Pointer sheetnessOutputImageTypePointer = GenerateSheetnessWithSigma(input, m_SheetnessScales.at(0));

  if (m_SheetnessScales.size() > 1)
  { // need for std::next()
  for(SheetnessScalesType::iterator scalesIterator = ++(m_SheetnessScales.begin()); scalesIterator != m_SheetnessScales.end(); ++scalesIterator)
    {
    // Calculte the remaining (n-1) sheetnesses
    typename OutputImageType::Pointer tempSheetnessOutputImageTypePointer = GenerateSheetnessWithSigma(input, (*scalesIterator));

    // Take abs max
    typename MaximumAbsoluteValueFilterType::Pointer maximumAbsoluteValueFilter = MaximumAbsoluteValueFilterType::New();
    maximumAbsoluteValueFilter->SetInput1(sheetnessOutputImageTypePointer);
    maximumAbsoluteValueFilter->SetInput2(tempSheetnessOutputImageTypePointer);
    maximumAbsoluteValueFilter->Update();

    // Save max and move on
    sheetnessOutputImageTypePointer = maximumAbsoluteValueFilter->GetOutput();
    }
  }

  // copy output
  this->GetOutput()->Graft(sheetnessOutputImageTypePointer);
}


template<typename TInput, typename TOutput>
typename TOutput::Pointer KrcahSheetnessFeatureGenerator<TInput, TOutput>
::GenerateSheetnessWithSigma(typename TInput::ConstPointer input, float sigma)
{
  /******
  * Input preprocessing
  ******/
  typename InputCastFilterType::Pointer castFilter = InputCastFilterType::New();
  castFilter->SetInput(input);

  // I*G (discrete gauss)
  typename GaussianFilterType::Pointer m_DiffusionFilter = GaussianFilterType::New();
  m_DiffusionFilter->SetVariance(m_GaussVariance); // =s
  m_DiffusionFilter->SetInput(castFilter->GetOutput());

  // I - (I*G)
  typename SubstractFilterType::Pointer m_SubstractFilter = SubstractFilterType::New();
  m_SubstractFilter->SetInput1(castFilter->GetOutput());
  m_SubstractFilter->SetInput2(m_DiffusionFilter->GetOutput());

  // k(I-(I*G))
  typename MultiplyFilterType::Pointer m_MultiplyFilter = MultiplyFilterType::New();
  m_MultiplyFilter->SetInput(m_SubstractFilter->GetOutput());
  m_MultiplyFilter->SetConstant(m_ScalingConstant); // =k

  // I+k*(I-(I*G))
  typename AddFilterType::Pointer m_AddFilter = AddFilterType::New();
  m_AddFilter->SetInput1(castFilter->GetOutput());
  m_AddFilter->SetInput2(m_MultiplyFilter->GetOutput());

  /******
  * sheetness prerequisites
  ******/
  // hessian
  typename HessianFilterType::Pointer m_HessianFilter = HessianFilterType::New();
  m_HessianFilter->SetSigma(sigma);
  m_HessianFilter->SetInput(m_AddFilter->GetOutput());

  // eigen analysis
  typename EigenAnalysisFilterType::Pointer m_EigenAnalysisFilter = EigenAnalysisFilterType::New();
  m_EigenAnalysisFilter->SetDimension(NDimension);
  m_EigenAnalysisFilter->SetInput(m_HessianFilter->GetOutput());

  // calculate trace
  typename TraceFilterType::Pointer m_TraceFilter = TraceFilterType::New();
  m_TraceFilter->SetImageDimension(NDimension);
  m_TraceFilter->SetInput(m_HessianFilter->GetOutput());

  // calculate average
  typename StatisticsFilterType::Pointer m_StatisticsFilter = StatisticsFilterType::New();
  m_StatisticsFilter->SetInput(m_TraceFilter->GetOutput());
  m_StatisticsFilter->Update(); // needed! ->GetMean() will not trigger an update!

  /******
  * Sheetness
  ******/
  typename SheetnessFilterType::Pointer m_SheetnessFilter = SheetnessFilterType::New();
  m_SheetnessFilter->SetInput(m_EigenAnalysisFilter->GetOutput());
  m_SheetnessFilter->SetConstant(m_StatisticsFilter->GetMean());
  m_SheetnessFilter->SetAlpha(m_Alpha);
  m_SheetnessFilter->SetBeta(m_Beta);
  m_SheetnessFilter->SetGamma(m_Gamma);

  // return
  m_SheetnessFilter->Update();
  return m_SheetnessFilter->GetOutput();
}

} // end namespace itk

#endif // itkKrcahSheetnessFeatureGenerator_hxx

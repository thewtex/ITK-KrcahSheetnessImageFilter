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
#ifndef itkAutomaticSheetnessParameterEstimationImageFilter_hxx
#define itkAutomaticSheetnessParameterEstimationImageFilter_hxx

#include "itkAutomaticSheetnessParameterEstimationImageFilter.h"

namespace itk
{

template< typename TInputImage, typename TLabelImage >
AutomaticSheetnessParameterEstimationImageFilter<TInputImage, TLabelImage>
::AutomaticSheetnessParameterEstimationImageFilter():
  m_Alpha(0.5f), m_Beta(0.5f), m_C(0.5f),
  m_Label(1.0f), m_Scale(0.1f)
{
  this->SetNumberOfRequiredInputs(1);
}


template< typename TInputImage, typename TLabelImage >
AutomaticSheetnessParameterEstimationImageFilter<TInputImage, TLabelImage>
::~AutomaticSheetnessParameterEstimationImageFilter()
{
}


template< typename TInputImage, typename TLabelImage >
void
AutomaticSheetnessParameterEstimationImageFilter<TInputImage, TLabelImage>
::GenerateData()
{
  // Compute Frobenius norm
  typename FrobeniusNormImageFilterType::Pointer frobeniusFilter = FrobeniusNormImageFilterType::New();
  frobeniusFilter->SetInput(this->GetInput());

  // Compute max
  if (this->GetLabelInput() == ITK_NULLPTR)
  { // Not verified yet...
    typename StatisticsImageFilterType::Pointer statisticsFilter = StatisticsImageFilterType::New();
    statisticsFilter->SetInput(frobeniusFilter->GetOutput());
    statisticsFilter->Update();

    // Set C
    m_C = static_cast<double>(this->GetScale() * statisticsFilter->GetMaximum());
  }
  else
  {
    typename LabelStatisticsImageFilterType::Pointer statisticsFilter = LabelStatisticsImageFilterType::New();
    statisticsFilter->SetInput(frobeniusFilter->GetOutput());
    statisticsFilter->SetLabelInput(this->GetLabelInput());
    statisticsFilter->Update();

    // Set C
    m_C = static_cast<double>(this->GetScale() * statisticsFilter->GetMaximum(this->GetLabel()));
  }

  // Set output
  this->GetOutput()->Graft(this->GetInput());
}

} // end namespace itk

#endif /* itkAutomaticSheetnessParameterEstimationImageFilter_hxx */

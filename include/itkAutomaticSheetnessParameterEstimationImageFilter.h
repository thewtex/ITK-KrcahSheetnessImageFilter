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
#ifndef itkAutomaticSheetnessParameterEstimationImageFilter_h
#define itkAutomaticSheetnessParameterEstimationImageFilter_h

#include "FrobeniusNormImageFilter.h"
#include "itkLabelStatisticsImageFilter.h"
#include "itkStatisticsImageFilter.h"

namespace itk
{

template< typename TInputImage, typename TLabelImage >
class ITK_TEMPLATE_EXPORT AutomaticSheetnessParameterEstimationImageFilter:
  public ImageToImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef AutomaticSheetnessParameterEstimationImageFilter Self;
  typedef SmartPointer<Self>                               Pointer;
  typedef SmartPointer<const Self>                         ConstPointer;
  typedef typename TLabelImage::PixelType                  TLabelPixelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(AutomaticSheetnessParameterEstimationImageFilter, ImageToImageFilter);

  itkSetMacro(Label,TLabelPixelType);
  itkGetMacro(Label,TLabelPixelType);

  itkSetMacro(Scale,double);
  itkGetMacro(Scale,double);

  itkGetMacro(Alpha,double);
  itkGetMacro(Beta,double);
  itkGetMacro(C,double);

  void SetLabelInput(const TLabelImage *input) {
    // Process object is not const-correct so the const casting is required.
    this->SetNthInput( 1, const_cast< TLabelImage * >( input ) );
  }

  const TLabelImage * GetLabelInput() const{
    return itkDynamicCastInDebugMode< TLabelImage * >( const_cast< DataObject * >( this->ProcessObject::GetInput(1) ) );
  }

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  typedef typename TInputImage::PixelType InputPixelType;
  itkConceptMacro(BracketOperatorsCheck,
    (Concept::BracketOperator< InputPixelType, unsigned int, double >));
  itkConceptMacro(DoubleConvertibleToOutputCheck,
    (Concept::Convertible<double, typename TInputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  AutomaticSheetnessParameterEstimationImageFilter();
  virtual ~AutomaticSheetnessParameterEstimationImageFilter();

  void GenerateData() ITK_OVERRIDE;
private:
  AutomaticSheetnessParameterEstimationImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  // Parameters
  double m_Alpha;
  double m_Beta;
  double m_C;
  double m_Scale;
  TLabelPixelType m_Label;

  // Filter types
  itkStaticConstMacro(NDimension, unsigned int, TInputImage::ImageDimension);
  typedef double FrobeniusImagePixelType;
  typedef Image<FrobeniusImagePixelType, NDimension> TFrobeniusOutputImage;

  typedef FrobeniusNormImageFilter<TInputImage, TFrobeniusOutputImage> FrobeniusNormImageFilterType;
  typedef LabelStatisticsImageFilter<TFrobeniusOutputImage, TLabelImage> LabelStatisticsImageFilterType;
  typedef StatisticsImageFilter<TFrobeniusOutputImage> StatisticsImageFilterType;
}; // class AutomaticSheetnessParameterEstimationImageFilter

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAutomaticSheetnessParameterEstimationImageFilter.hxx"
#endif

#endif /* AutomaticSheetnessParameterEstimationImageFilter_h */

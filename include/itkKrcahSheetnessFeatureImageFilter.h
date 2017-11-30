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
#ifndef itkKrcahSheetnessFeatureImageFilter_h
#define itkKrcahSheetnessFeatureImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkAddImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkSymmetricEigenAnalysisImageFilter.h"
#include "itkStatisticsImageFilter.h"

#include "itkMaximumAbsoluteValueImageFilter.h"
#include "itkKrcahSheetnessImageFilter.h"
#include "itkTraceImageFilter.h"

#include <vector>

namespace itk
{

template< typename TInput, typename TOutput >
class ITK_TEMPLATE_EXPORT KrcahSheetnessFeatureImageFilter:
  public ImageToImageFilter<TInput, TOutput>
{
public:
  typedef KrcahSheetnessFeatureImageFilter    Self;
  typedef ImageToImageFilter<TInput, TOutput> Superclass;
  typedef SmartPointer<Self>                  Pointer;
  typedef SmartPointer<const Self>            ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(KrcahSheetnessFeatureImageFilter, ImageToImageFilter);

  itkStaticConstMacro(NDimension, unsigned int, TInput::ImageDimension);

  typedef float                                    InternalPixelType;
  typedef TInput                                   InputImageType;
  typedef Image<InternalPixelType, NDimension>     InternalImageType;
  typedef TOutput                                  OutputImageType;
  typedef std::vector<double>                      SheetnessScalesType; // 1-dimensional vector of sigmas

  /** Unsharp mask Gaussian sigma. */
  itkSetMacro( GaussianSigma, double );
  itkGetConstMacro( GaussianSigma, double );

  /** Unsharp mask scaling factor */
  itkSetMacro( ScalingConstant, double );
  itkGetConstMacro( ScalingConstant, double );

  /** Sheetness normalization */
  itkSetMacro( Alpha, double );
  itkGetConstMacro( Alpha, double );

  /** Tubeness normalization */
  itkSetMacro( Beta, double );
  itkGetConstMacro( Beta, double );

  /** Hessian noise normalization */
  itkSetMacro( Gamma, double );
  itkGetConstMacro( Gamma, double );

  void SetSheetnessScales(SheetnessScalesType v)
    {
      m_SheetnessScales = v;
      this->Modified();
    }

protected:
  KrcahSheetnessFeatureImageFilter();
  virtual ~KrcahSheetnessFeatureImageFilter();

  void GenerateData() ITK_OVERRIDE;

private:
  KrcahSheetnessFeatureImageFilter(const Self &);
  void operator=(const Self &);

  // parameters
  double              m_GaussianSigma;
  double              m_ScalingConstant;
  double              m_Alpha;
  double              m_Beta;
  double              m_Gamma;
  SheetnessScalesType m_SheetnessScales;

  void GenerateSheetnessWithSigma(InputImageType * input, OutputImageType * output, double sigma);

  // input processing
  typedef CastImageFilter<InputImageType, InternalImageType>                                 InputCastFilterType;
  typename InputCastFilterType::Pointer m_InputCastFilter;
  typedef DiscreteGaussianImageFilter<InternalImageType, InternalImageType>                  GaussianFilterType;
  typename GaussianFilterType::Pointer m_GaussianFilter;
  typedef SubtractImageFilter<InternalImageType, InternalImageType, InternalImageType>       SubtractFilterType;
  typename SubtractFilterType::Pointer m_SubtractFilter;
  typedef MultiplyImageFilter<InternalImageType, InternalImageType, InternalImageType>       MultiplyFilterType;
  typename MultiplyFilterType::Pointer m_MultiplyFilter;
  typedef AddImageFilter<InternalImageType, InternalImageType, InternalImageType>            AddFilterType;
  typename AddFilterType::Pointer m_AddFilter;

  // sheetness prerequisites
  typedef HessianRecursiveGaussianImageFilter<InternalImageType>                             HessianFilterType;
  typename HessianFilterType::Pointer m_HessianFilter;
  typedef typename HessianFilterType::OutputImageType                                        HessianImageType;
  typedef typename HessianImageType::PixelType                                               HessianPixelType;
  typedef FixedArray<double, HessianPixelType::Dimension>                                    EigenValueArrayType;
  typedef Image<EigenValueArrayType, NDimension>                                             EigenValueImageType;
  typedef SymmetricEigenAnalysisImageFilter<HessianImageType, EigenValueImageType>           EigenAnalysisFilterType;
  typename EigenAnalysisFilterType::Pointer m_EigenAnalysisFilter;
  typedef TraceImageFilter<HessianImageType, InternalImageType>                              TraceFilterType;
  typename TraceFilterType::Pointer m_TraceFilter;
  typedef StatisticsImageFilter<InternalImageType>                                           StatisticsFilterType;
  typename StatisticsFilterType::Pointer m_StatisticsFilter;

  // sheetness
  typedef KrcahSheetnessImageFilter<EigenValueImageType, double, OutputImageType>            SheetnessFilterType;
  typename SheetnessFilterType::Pointer m_SheetnessFilter;

  // post processing
  typedef MaximumAbsoluteValueImageFilter<OutputImageType, OutputImageType, OutputImageType> MaximumAbsoluteValueFilterType;

};
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkKrcahSheetnessFeatureImageFilter.hxx"
#endif

#endif // itkKrcahSheetnessFeatureImageFilter_h

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
#ifndef itkKrcahSheetnessFeatureGenerator_h
#define itkKrcahSheetnessFeatureGenerator_h

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

template<typename TInput, typename TOutput>
class ITK_TEMPLATE_EXPORT KrcahSheetnessFeatureGenerator:
  public ImageToImageFilter<TInput, TOutput>
{
public:
  typedef KrcahSheetnessFeatureGenerator      Self;
  typedef ImageToImageFilter<TInput, TOutput> Superclass;
  typedef SmartPointer<Self>                  Pointer;
  typedef SmartPointer<const Self>            ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(KrcahSheetnessFeatureGenerator, ImageToImageFilter);

  itkStaticConstMacro(NDimension, unsigned int, TInput::ImageDimension);

  typedef float                                    InternalPixelType;
  typedef TInput                                   InputImageType;
  typedef Image<InternalPixelType, NDimension>     InternalImageType;
  typedef TOutput                                  OutputImageType;
  typedef std::vector<double>                      SheetnessScalesType; // 1-dimensional vector of sigmas

  void SetGaussVariance(double d) {
      m_GaussVariance = d;
  }

  void SetScalingConstant(double d) {
      m_ScalingConstant = d;
  }

  void SetAlpha(double d) {
      m_Alpha = d;
  }

  void SetBeta(double d) {
      m_Beta = d;
  }

  void SetGamma(double d) {
      m_Gamma = d;
  }

  void SetSheetnessScales(SheetnessScalesType v) {
      m_SheetnessScales = v;
  }

protected:
  KrcahSheetnessFeatureGenerator();

  virtual ~KrcahSheetnessFeatureGenerator();

  void GenerateData() ITK_OVERRIDE;

private:
  KrcahSheetnessFeatureGenerator(const Self &);
  void operator=(const Self &);

  // parameters
  double              m_GaussVariance;
  double              m_ScalingConstant;
  double              m_Alpha;
  double              m_Beta;
  double              m_Gamma;
  SheetnessScalesType m_SheetnessScales;

  typename OutputImageType::Pointer GenerateSheetnessWithSigma(typename InputImageType::ConstPointer img, float sigma);

  // input processing
  typedef CastImageFilter<InputImageType, InternalImageType>                                 InputCastFilterType;
  typedef DiscreteGaussianImageFilter<InternalImageType, InternalImageType>                  GaussianFilterType;
  typedef SubtractImageFilter<InternalImageType, InternalImageType, InternalImageType>       SubstractFilterType;
  typedef MultiplyImageFilter<InternalImageType, InternalImageType, InternalImageType>       MultiplyFilterType;
  typedef AddImageFilter<InternalImageType, InternalImageType, InternalImageType>            AddFilterType;

  // sheetness prerequisites
  typedef HessianRecursiveGaussianImageFilter<InternalImageType>                             HessianFilterType;
  typedef typename HessianFilterType::OutputImageType                                        HessianImageType;
  typedef typename HessianImageType::PixelType                                               HessianPixelType;
  typedef FixedArray<double, HessianPixelType::Dimension>                                    EigenValueArrayType;
  typedef Image<EigenValueArrayType, NDimension>                                             EigenValueImageType;
  typedef SymmetricEigenAnalysisImageFilter<HessianImageType, EigenValueImageType>           EigenAnalysisFilterType;
  typedef TraceImageFilter<HessianImageType, InternalImageType>                              TraceFilterType;
  typedef StatisticsImageFilter<InternalImageType>                                           StatisticsFilterType;

  // sheetness
  typedef KrcahSheetnessImageFilter<EigenValueImageType, double, OutputImageType>            SheetnessFilterType;

  // post processing
  typedef MaximumAbsoluteValueImageFilter<OutputImageType, OutputImageType, OutputImageType> MaximumAbsoluteValueFilterType;

};
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkKrcahSheetnessFeatureGenerator.hxx"
#endif

#endif // itkKrcahSheetnessFeatureGenerator_h

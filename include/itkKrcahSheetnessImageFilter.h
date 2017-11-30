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
#ifndef itkKrcahSheetnessImageFilter_h
#define itkKrcahSheetnessImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "itkKrcahSheetnessFunctor.h"

namespace itk
{
template<typename TInputImage, typename TConstant, typename TOutputImage>
class KrcahSheetnessImageFilter :
  public BinaryFunctorImageFilter<TInputImage, Image<TConstant, TInputImage::ImageDimension>, TOutputImage,
    Functor::KrcahSheetness<typename TInputImage::PixelType, TConstant, typename TOutputImage::PixelType> >
{
public:
  // itk requirements
  typedef KrcahSheetnessImageFilter                                                                               Self;
  typedef BinaryFunctorImageFilter<TInputImage, Image<TConstant, TInputImage::ImageDimension>,                    TOutputImage,
          Functor::KrcahSheetness<typename TInputImage::PixelType, TConstant, typename TOutputImage::PixelType> > Superclass;
  typedef SmartPointer<Self>                                                                                      Pointer;
  typedef SmartPointer<const Self>                                                                                ConstPointer;

  itkNewMacro(Self); // create the smart pointers and register with ITKs object factory
  itkTypeMacro(KrcahSheetnessImageFilter, BinaryFunctorImageFilter); // type information for runtime evaluation

  // member functions
  void SetAlpha(double value)
  {
    this->GetFunctor().SetAlpha(value);
  }

  void SetBeta(double value)
  {
    this->GetFunctor().SetBeta(value);
  }

  void SetGamma(double value)
  {
    this->GetFunctor().SetGamma(value);
  }

protected:
  KrcahSheetnessImageFilter() {};
  virtual ~KrcahSheetnessImageFilter() {};

private:
  KrcahSheetnessImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);   //purposely not implemented
};

} // end namespace itk

#endif // itkKrcahSheetnessImageFilter_h

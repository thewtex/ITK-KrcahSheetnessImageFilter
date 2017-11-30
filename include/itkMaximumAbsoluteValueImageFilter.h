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
#ifndef itkMaximumAbsoluteValueImageFilter_h
#define itkMaximumAbsoluteValueImageFilter_h

#include "itkBinaryFunctorImageFilter.h"

namespace itk
{

namespace Functor
{
template<typename TInputPixel1, typename TInputPixel2, typename TOutputPixel>
class MaximumAbsoluteValue
{
public:
  MaximumAbsoluteValue() {}

  ~MaximumAbsoluteValue() {}

  inline TOutputPixel operator()(const TInputPixel1 A, const TInputPixel2 B) {
      return static_cast<TOutputPixel>(vnl_math_abs(A) >= vnl_math_abs(B) ? A : B);
  }
};
} // namespace Functor

template<typename TInputImage1, typename TInputImage2, typename TOutputImage>
class MaximumAbsoluteValueImageFilter:
        public BinaryFunctorImageFilter<TInputImage1, TInputImage2, TOutputImage,
                Functor::MaximumAbsoluteValue<typename TInputImage1::PixelType, typename TInputImage2::PixelType,
                        typename TOutputImage::PixelType> >
{
public:
  // itk requirements
  typedef MaximumAbsoluteValueImageFilter             Self;
  typedef BinaryFunctorImageFilter<TInputImage1, TInputImage2, TOutputImage,
          Functor::MaximumAbsoluteValue<typename TInputImage1::PixelType, typename TInputImage2::PixelType,
                  typename TOutputImage::PixelType> > Superclass;
  typedef SmartPointer<Self>                          Pointer;
  typedef SmartPointer<const Self>                    ConstPointer;

  itkNewMacro(Self); // create the smart pointers and register with ITKs object factory
  itkTypeMacro(MaximumAbsoluteValueImageFilter, BinaryFunctorImageFilter); // type information for runtime evaluation

protected:
  MaximumAbsoluteValueImageFilter() {};
  virtual ~MaximumAbsoluteValueImageFilter() {};

private:
  MaximumAbsoluteValueImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);   //purposely not implemented
};
}

#endif //itkMaximumAbsoluteValueImageFilter_h

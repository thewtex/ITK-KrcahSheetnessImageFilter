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
#ifndef itkFrobeniusNormImageFilter_h
#define itkFrobeniusNormImageFilter_h

#include "vnl/vnl_math.h"

namespace itk
{

namespace Function
{

template< typename TInput, typename TOutput>
class FrobeniusMatrixNorm
{
public:
  FrobeniusMatrixNorm()
    {}
  ~FrobeniusMatrixNorm() {}
  bool operator!=( const FrobeniusMatrixNorm & ) const {
    return false;
  }
  bool operator==( const FrobeniusMatrixNorm & other ) const{
    return !(*this != other);
  }
  inline TOutput operator()( const TInput & A ) {
    double frobeniusNorm = 0;

    // Loop over all eigenvalues and square
    typename TInput::ConstIterator it = A.Begin();
    while (it != A.End()) {
        frobeniusNorm += static_cast<double>((*it) * (*it));
        ++it;
    }

    return static_cast<TOutput>( vcl_sqrt(frobeniusNorm) );
  }
}; // class FrobeniusMatrixNorm
} // Function

template <typename TInputImage, typename TOutputImage>
class ITK_TEMPLATE_EXPORT FrobeniusNormImageFilter:
    public UnaryFunctorImageFilter<TInputImage,TOutputImage,
                        Function::FrobeniusMatrixNorm< typename TInputImage::PixelType,
                                       typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef FrobeniusNormImageFilter          Self;
  typedef UnaryFunctorImageFilter< TInputImage,TOutputImage,
    Function::FrobeniusMatrixNorm<
      typename TInputImage::PixelType,
      typename TOutputImage::PixelType> >   Superclass;
  typedef SmartPointer<Self>                Pointer;
  typedef SmartPointer<const Self>          ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(FrobeniusNormImageFilter,
               UnaryFunctorImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  typedef typename TInputImage::PixelType InputPixelType;
  itkConceptMacro(BracketOperatorsCheck,
    (Concept::BracketOperator< InputPixelType, unsigned int, double >));
  itkConceptMacro(DoubleConvertibleToOutputCheck,
    (Concept::Convertible<double, typename TOutputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  FrobeniusNormImageFilter() {}
  virtual ~FrobeniusNormImageFilter() {}

private:
  FrobeniusNormImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
}; // class FrobeniusNormImageFilter_h

} // namespace itk

#endif /* FrobeniusNormImageFilter_h */

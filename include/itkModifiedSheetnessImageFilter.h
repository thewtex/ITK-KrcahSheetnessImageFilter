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
#ifndef itkModifiedSheetnessImageFilter_h
#define itkModifiedSheetnessImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace itk
{

namespace Functor
{

template< typename TInput, typename TOutput>
class ModifiedSheetness
{
public:
  ModifiedSheetness() {
    m_Alpha = 0.5;              // Suggested value from Vesselness paper
    m_C     = 1.0;              // Should be tuned from data
    m_DetectBrightSheets = -1;  // Detect bright sheets is default
  }
  ~ModifiedSheetness() {}
  bool operator!=( const ModifiedSheetness & ) const {
    return false;
  }

  bool operator==( const ModifiedSheetness & other ) const {
    return !(*this != other);
  }

  inline TOutput operator()( const TInput & A ) {
    double sheetness = 0.0;

    // Get eigenvalues
    double a1 = static_cast<double>( A[0] );
    double a2 = static_cast<double>( A[1] );
    double a3 = static_cast<double>( A[2] );

    double l1 = vnl_math_abs( a1 );
    double l2 = vnl_math_abs( a2 );
    double l3 = vnl_math_abs( a3 );

    // Sort eigen values
    sortEigenValues(a1, a2, a3, l1, l2, l3);

    // Avoid divisions by zero (or close to zero)
    if( static_cast<double>( l3 ) < vnl_math::eps ) {
      // If l3 approx. 0, Rs -> inf, sheetness -> 0
      return static_cast<TOutput>( sheetness );
    }

    const double Rt = l1 / (l2 + l3);
    const double Rn = vcl_sqrt( l3*l3 + l2*l2 + l1*l1 );

    // Calculate sheetness
    sheetness = m_DetectBrightSheets * (a3 / l3);
    sheetness *= vcl_exp( - ( Rt * Rt ) / ( 2.0 * m_Alpha  * m_Alpha  ) );
    sheetness *= ( 1.0 - vcl_exp( - ( Rn * Rn ) / ( 2.0 * m_C     * m_C     ) ) );

    return static_cast<TOutput>( sheetness );
  }

  void sortEigenValues(double &a1, double &a2, double &a3, double &l1, double &l2, double &l3) {
    double tmpl, tmpa;

    // Sort the values by their absolute value.
    //  l1 <= l2 <= l3
    if( l2 > l3 ) {
      tmpl = l3;
      l3 = l2;
      l2 = tmpl;
      tmpa = a3;
      a3 = a2;
      a2 = tmpa;
    }

    if( l1 > l2 ) {
      tmpl = l1;
      l1 = l2;
      l2 = tmpl;
      tmpa = a1;
      a1 = a2;
      a2 = tmpa;
    }

    if( l2 > l3 ){
      tmpl = l3;
      l3 = l2;
      l2 = tmpl;
      tmpa = a3;
      a3 = a2;
      a2 = tmpa;
    }
  }

  void SetAlpha(double value) {
    m_Alpha = value;
  }

  double GetAlpha() {
    return m_Alpha;
  }

  void SetC(double value) {
    m_C = value;
  }

  double GetC() {
    return m_C;
  }

  void DetectBrightSheetsOn() {
    this->m_DetectBrightSheets = -1;
  }

  void DetectDarkSheetsOn() {
    this->m_DetectBrightSheets = 1;
  }

  bool IsDetectBrightSheetsOn() {
    return  (m_DetectBrightSheets == -1);
  }

private:
  double    m_Alpha;
  double    m_C;
  double    m_DetectBrightSheets;
}; // class ModifiedSheetness
} // namespace Functor

template <typename TInputImage, typename TOutputImage>
class ITK_TEMPLATE_EXPORT ModifiedSheetnessImageFilter :
    public UnaryFunctorImageFilter<TInputImage,TOutputImage,
Functor::ModifiedSheetness< typename TInputImage::PixelType,
                                       typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef ModifiedSheetnessImageFilter      Self;
  typedef UnaryFunctorImageFilter<
    TInputImage,TOutputImage,
    Functor::ModifiedSheetness<
      typename TInputImage::PixelType,
      typename TOutputImage::PixelType> >   Superclass;
  typedef SmartPointer<Self>                Pointer;
  typedef SmartPointer<const Self>          ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ModifiedSheetnessImageFilter, UnaryFunctorImageFilter);

  /** Set the normalization term for sheetness */
  void SetNormalization( double value ) {
    this->GetFunctor().SetAlpha( value );
  }

  /** Set the normalization term for noise. */
  void SetNoiseNormalization( double value ) {
    this->GetFunctor().SetC( value );
  }
  void DetectBrightSheetsOn() {
    this->GetFunctor().DetectBrightSheetsOn();
  }
  void DetectDarkSheetsOn() {
    this->GetFunctor().DetectDarkSheetsOn();
  }

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
  ModifiedSheetnessImageFilter() {}
  virtual ~ModifiedSheetnessImageFilter() {}

private:
  ModifiedSheetnessImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
}; // class ModifiedSheetnessImageFilter


} // end namespace itk

#endif /* itkModifiedSheetnessImageFilter_h */

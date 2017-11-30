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
#ifndef itkKrcahBackgroundFunctor_h
#define itkKrcahBackgroundFunctor_h

namespace itk
{

namespace Functor
{
template<typename TThresholdPixel, typename TSheetnessPixel, typename TOutputPixel>
class KrcahBackground
{
public:
   KrcahBackground() {}

   inline TOutputPixel operator()(const TThresholdPixel T, const TSheetnessPixel S)
   {
      // 1 = pixel is NOT background, 0 = pixel is background
      if (T >= 400 && S > 0)
      {
        return static_cast<TOutputPixel>(1);
      }
      return static_cast<TOutputPixel>(0);
    }
};
} // namespace Funtor
} // namespace itk

#endif // itkKrcahBackgroundfunctor_h

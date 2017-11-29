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
#include "gtest/gtest.h"

#include "ModifiedSheetnessImageFilter.h"

namespace
{

template <typename T>
class ModifiedSheetnessFunctorTest : public ::testing::Test {
protected:
  typedef T InternalPixelType;
  static const unsigned int DIMENSION = 3;
  typedef  itk::FixedArray< InternalPixelType, DIMENSION >                        EigenValueArrayType;
  typedef itk::Functor::ModifiedSheetness<EigenValueArrayType, InternalPixelType> FunctorType;

  FunctorType         m_ModifiedSheetness;
  EigenValueArrayType m_EigenValueArray;

  ModifiedSheetnessFunctorTest() {
    m_ModifiedSheetness = FunctorType();
    m_EigenValueArray = EigenValueArrayType();
  }

  virtual ~ModifiedSheetnessFunctorTest() {
    // Nothing to do
  }
};

  // Define the templates we would like to test
  typedef ::testing::Types<char, int, char, float> TestingLabelTypes;
  TYPED_TEST_CASE(ModifiedSheetnessFunctorTest, TestingLabelTypes);

  TYPED_TEST(ModifiedSheetnessFunctorTest, ZeroInput) {
    this->m_EigenValueArray[0] = 0;
    this->m_EigenValueArray[1] = 0;
    this->m_EigenValueArray[2] = 0;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), 0);
  }

  TYPED_TEST(ModifiedSheetnessFunctorTest, ZeroOneZeroInput) {
    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 0;
    this->m_EigenValueArray[2] = 0;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.3934693402873666);

    this->m_EigenValueArray[0] = 0;
    this->m_EigenValueArray[1] = 0;
    this->m_EigenValueArray[2] = 1;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.3934693402873666);

    this->m_EigenValueArray[0] = 0;
    this->m_EigenValueArray[1] = 1;
    this->m_EigenValueArray[2] = 0;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.3934693402873666);
  }

  TYPED_TEST(ModifiedSheetnessFunctorTest, ZeroOneOneInput) {
    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 1;
    this->m_EigenValueArray[2] = 0;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.6321205588285578);
  }

  TYPED_TEST(ModifiedSheetnessFunctorTest, SortsOnAbsoluteValueWithBrightSheets) {
    this->m_EigenValueArray[0] = 2;
    this->m_EigenValueArray[1] = 1;
    this->m_EigenValueArray[2] = 1;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.760871035093);

    this->m_EigenValueArray[0] = -2;
    this->m_EigenValueArray[1] = 1;
    this->m_EigenValueArray[2] = 1;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)0.760871035093);
  }

  TYPED_TEST(ModifiedSheetnessFunctorTest, SortsOnAbsoluteValueWithDarkSheets) {
    this->m_ModifiedSheetness.DetectDarkSheetsOn();

    this->m_EigenValueArray[0] = 2;
    this->m_EigenValueArray[1] = 1;
    this->m_EigenValueArray[2] = 1;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)0.760871035093);

    this->m_EigenValueArray[0] = -2;
    this->m_EigenValueArray[1] = 1;
    this->m_EigenValueArray[2] = 1;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.760871035093);
  }

  TYPED_TEST(ModifiedSheetnessFunctorTest, BrightnessWorks) {
    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = 3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.922274573238);

    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = -3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)0.922274573238);

    this->m_ModifiedSheetness.DetectDarkSheetsOn();
    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = 3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)0.922274573238);

    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = -3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.922274573238);

    this->m_ModifiedSheetness.DetectBrightSheetsOn();
    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = 3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.922274573238);

    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = -3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)0.922274573238);
  }

  TYPED_TEST(ModifiedSheetnessFunctorTest, SetAndGetAlpha) {
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness.GetAlpha(), 0.5);
    this->m_ModifiedSheetness.SetAlpha(0);
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness.GetAlpha(), 0);
    this->m_ModifiedSheetness.SetAlpha(100);
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness.GetAlpha(), 100);
  }

  TYPED_TEST(ModifiedSheetnessFunctorTest, ChangingAlphaWorks) {
    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = 3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.922274573238);

    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = -3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)0.922274573238);

    this->m_ModifiedSheetness.SetAlpha(1.0);
    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = 3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.979304847814);

    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = -3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)0.979304847814);

    this->m_ModifiedSheetness.SetAlpha(2.5);
    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = 3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.995896145936);

    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = -3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)0.995896145936);
  }

  TYPED_TEST(ModifiedSheetnessFunctorTest, SetAndGetC) {
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness.GetC(), 1.0);
    this->m_ModifiedSheetness.SetC(0);
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness.GetC(), 0);
    this->m_ModifiedSheetness.SetC(100);
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness.GetC(), 100);
  }

  TYPED_TEST(ModifiedSheetnessFunctorTest, ChangingCWorks) {
    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = 3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.922274573238);

    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = -3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)0.922274573238);

    this->m_ModifiedSheetness.SetC(0.5);
    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = 3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.923116346386);

    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = -3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)0.923116346386);

    this->m_ModifiedSheetness.SetC(2.5);
    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = 3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)-0.621922134474);

    this->m_EigenValueArray[0] = 1;
    this->m_EigenValueArray[1] = 2;
    this->m_EigenValueArray[2] = -3;
    EXPECT_DOUBLE_EQ(this->m_ModifiedSheetness(this->m_EigenValueArray), (TypeParam)0.621922134474);
  }

}

/*
* Copyright 2017 Huy Cuong Nguyen
* Copyright 2014 ZXing authors
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "gtest/gtest.h"
#include "BitMatrix.h"
#include "BitMatrixUtility.h"
#include "oned/ODCode128Writer.h"
#include "oned/ODCode128Reader.h"
#include "DecodeHints.h"
#include "Result.h"

using namespace ZXing;
using namespace ZXing::OneD;

static const std::string FNC1 = "11110101110";
static const std::string FNC2 = "11110101000";
static const std::string FNC3 = "10111100010";
static const std::string FNC4 = "10111101110";
static const std::string START_CODE_B = "11010010000";
static const std::string START_CODE_C = "11010011100";
static const std::string SWITCH_CODE_B = "10111101110";
static const std::string QUIET_SPACE = "00000";
static const std::string STOP = "1100011101011";

static std::string LineMatrixToString(const BitMatrix& matrix)
{
	auto result = Utility::ToString(matrix, '1', '0', false);
	return result.substr(0, result.size() - 1);
}

static ZXing::Result Decode(const BitMatrix &matrix)
{
	BitArray row;
	matrix.getRow(0, row);
	std::unique_ptr<RowReader::DecodingState> state;
	return Code128Reader(DecodeHints()).decodeRow(0, row, state);
}

TEST(ODCode128Writer, EncodeWithFunc1)
{
	auto toEncode = L"\xf1""123";
    //                                                       "12"                           "3"          check digit 92
	auto expected = QUIET_SPACE + START_CODE_C + FNC1 + "10110011100" + SWITCH_CODE_B + "11001011100" + "10101111000" + STOP + QUIET_SPACE;

	auto actual = LineMatrixToString(Code128Writer().encode(toEncode, 0, 0));
	EXPECT_EQ(actual, expected);
}

TEST(ODCode128Writer, EncodeWithFunc2)
{
	auto toEncode = L"\xf2""123";
	//                                                       "1"            "2"             "3"          check digit 56
	auto expected = QUIET_SPACE + START_CODE_B + FNC2 + "10011100110" + "11001110010" + "11001011100" + "11100010110" + STOP + QUIET_SPACE;

	auto actual = LineMatrixToString(Code128Writer().encode(toEncode, 0, 0));
	EXPECT_EQ(actual, expected);
}

TEST(ODCode128Writer, EncodeWithFunc3)
{
	auto toEncode = L"\xf3""123";
	//                                                       "1"            "2"             "3"          check digit 51
	auto expected = QUIET_SPACE + START_CODE_B + FNC3 + "10011100110" + "11001110010" + "11001011100" + "11101000110" + STOP + QUIET_SPACE;

	auto actual = LineMatrixToString(Code128Writer().encode(toEncode, 0, 0));
	EXPECT_EQ(actual, expected);
}

TEST(ODCode128Writer, EncodeWithFunc4)
{
	auto toEncode = L"\xf4""123";
	//                                                       "1"            "2"             "3"          check digit 59
	auto expected = QUIET_SPACE + START_CODE_B + FNC4 + "10011100110" + "11001110010" + "11001011100" + "11100011010" + STOP + QUIET_SPACE;

	auto actual = LineMatrixToString(Code128Writer().encode(toEncode, 0, 0));
	EXPECT_EQ(actual, expected);
}

TEST(ODCode128Writer, Roundtrip)
{
	auto toEncode = L"\xf1" "10958" "\xf1" "17160526";
	auto expected = L"1095817160526";

    auto encResult = Code128Writer().encode(toEncode, 0, 0);
	auto actual = Decode(encResult).text();
	EXPECT_EQ(actual, expected);
}
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "FooA.h"

// ========================================
// MockCalculator：ICalculator のモック実装
// ========================================
class MockCalculator : public ICalculator {
public:
    MOCK_METHOD(int, add, (int a, int b), (override));
    MOCK_METHOD(int, multiply, (int a, int b), (override));
};

// ========================================
// 共通処理：CSVファイル読み込み（文字列で返す）
// ========================================
std::vector<std::vector<std::string>> LoadCSVStrings(const std::string& filename) {
    std::vector<std::vector<std::string>> csvData;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    std::string line;
    // ヘッダー行をスキップ
    std::getline(file, line);
    
    // データを読み込む
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> row;
        
        // CSV をパース
        while (std::getline(ss, token, ',')) {
            row.push_back(token);
        }
        
        csvData.push_back(row);
    }
    
    file.close();
    return csvData;
}

// ========================================
// 個別処理：テストケース構造体（変換メソッド付き）
// ========================================

// テストケースを表す構造体（整数型用）
struct TestCaseInt {
    int a;
    int b;
    int expected;
    
    // CSVの文字列ベクトルから構造体に変換するメソッド
    static TestCaseInt FromStrings(const std::vector<std::string>& values) {
        if (values.size() < 3) {
            throw std::invalid_argument("Expected at least 3 columns for TestCaseInt");
        }
        return {std::stoi(values[0]), std::stoi(values[1]), std::stoi(values[2])};
    }
};

// ========================================
// 汎用テンプレート関数：共通処理と個別処理を統合
// ========================================
template<typename T>
std::vector<T> LoadTestCasesFromCSV(const std::string& filename) {
    auto csvData = LoadCSVStrings(filename);
    std::vector<T> testCases;
    
    for (const auto& row : csvData) {
        testCases.push_back(T::FromStrings(row));
    }
    
    return testCases;
}

// ========================================
// FooA_Test：テスト基底クラス
// ========================================
class FooA_Test {
protected:
    FooA fooA;
    MockCalculator mockCalc;
    
    void SetUpMock() {
        // friend なので直接 m_calculator に mock をセット
        fooA.m_calculator = &mockCalc;
    }
    
    void TearDownMock() {
        // モックをクリア
        fooA.m_calculator = nullptr;
    }
};

// ========================================
// Google Test：パラメータ化テスト
// ========================================

// 加算テストのパラメータ化テスト
class AddTest : public FooA_Test, public ::testing::TestWithParam<TestCaseInt> {
protected:
    void SetUp() override {
        SetUpMock();
    }
    
    void TearDown() override {
        TearDownMock();
    }
};

TEST_P(AddTest, AddWithCSVParameters) {
    TestCaseInt testCase = GetParam();
    
    // モックの期待値を設定
    EXPECT_CALL(mockCalc, add(testCase.a, testCase.b))
        .WillOnce(::testing::Return(testCase.expected));
    
    // テスト実行
    EXPECT_EQ(fooA.add(testCase.a, testCase.b), testCase.expected)
        << "add(" << testCase.a << ", " << testCase.b << ") should equal " << testCase.expected;
}

// CSV ファイルからテストケースを読み込んで INSTANTIATE
INSTANTIATE_TEST_SUITE_P(
    AddTestSuite,
    AddTest,
    ::testing::ValuesIn(LoadTestCasesFromCSV<TestCaseInt>("test_cases_add.csv")),
    [](const ::testing::TestParamInfo<TestCaseInt>& info) {
        std::string aStr = (info.param.a >= 0 ? "pos_" : "neg_") + std::to_string(std::abs(info.param.a));
        std::string bStr = (info.param.b >= 0 ? "pos_" : "neg_") + std::to_string(std::abs(info.param.b));
        return aStr + "_b_" + bStr;
    }
);

// 乗算テストのパラメータ化テスト
class MultiplyTest : public FooA_Test, public ::testing::TestWithParam<TestCaseInt> {
protected:
    void SetUp() override {
        SetUpMock();
    }
    
    void TearDown() override {
        TearDownMock();
    }
};

TEST_P(MultiplyTest, MultiplyWithCSVParameters) {
    TestCaseInt testCase = GetParam();
    
    // モックの期待値を設定
    EXPECT_CALL(mockCalc, multiply(testCase.a, testCase.b))
        .WillOnce(::testing::Return(testCase.expected));
    
    // テスト実行
    EXPECT_EQ(fooA.multiply(testCase.a, testCase.b), testCase.expected)
        << "multiply(" << testCase.a << ", " << testCase.b << ") should equal " << testCase.expected;
}

INSTANTIATE_TEST_SUITE_P(
    MultiplyTestSuite,
    MultiplyTest,
    ::testing::ValuesIn(LoadTestCasesFromCSV<TestCaseInt>("test_cases_multiply.csv")),
    [](const ::testing::TestParamInfo<TestCaseInt>& info) {
        std::string aStr = (info.param.a >= 0 ? "pos_" : "neg_") + std::to_string(std::abs(info.param.a));
        std::string bStr = (info.param.b >= 0 ? "pos_" : "neg_") + std::to_string(std::abs(info.param.b));
        return aStr + "_b_" + bStr;
    }
);

// メイン関数 - Google Test で必須
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

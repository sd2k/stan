#include <stan/callbacks/csv_writer.hpp>
#include <test/unit/util.hpp>
#include <gtest/gtest.h>
#include <string>

struct deleter_noop {
  template <typename T>
  constexpr void operator()(T* arg) const {}
};
class StanInterfaceCallbacksCsvWriter : public ::testing::Test {
 public:
  StanInterfaceCallbacksCsvWriter()
      : ss(), writer(std::unique_ptr<std::stringstream, deleter_noop>(&ss)) {}

  void SetUp() {
    ss.str(std::string());
    ss.clear();
  }

  void TearDown() {}

  std::stringstream ss;
  stan::callbacks::csv_writer<std::stringstream, deleter_noop> writer;
};


TEST_F(StanInterfaceCallbacksCsvWriter, header_values) {
  std::vector<std::string> header = {"mu", "sigma", "theta"};
  std::vector<double> values = {1, 2, 3};
  writer.table_header(header);
  writer.table_row(values);
  EXPECT_EQ("mu, sigma, theta\n1, 2, 3\n", ss.str());
}

TEST_F(StanInterfaceCallbacksCsvWriter, header_only) {
  std::vector<std::string> header = {"mu", "sigma", "theta"};
  writer.table_header(header);
  EXPECT_EQ("mu, sigma, theta\n", ss.str());
}


// bad call sequences - how to handle state?

TEST_F(StanInterfaceCallbacksCsvWriter, mult_header) {
  std::vector<std::string> header = {"mu", "sigma", "theta"};
  writer.table_header(header);
  writer.table_header(header);
  EXPECT_EQ("mu, sigma, theta\n", ss.str());
}

TEST_F(StanInterfaceCallbacksCsvWriter, values_only) {
  std::vector<double> values = {1, 2, 3};
  writer.table_row(values);
  ASSERT_TRUE(ss.str().empty());
}

TEST_F(StanInterfaceCallbacksCsvWriter, values_header) {
  std::vector<std::string> header = {"mu", "sigma", "theta"};
  std::vector<double> values = {1, 2, 3};
  writer.table_row(values);
  writer.table_header(header);
  EXPECT_EQ("mu, sigma, theta\n", ss.str());
}

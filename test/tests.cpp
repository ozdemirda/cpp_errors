/*
MIT License

Copyright (c) 2022 Danis Ozdemir

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <gtest/gtest.h>
#include <cpp_errors.h>
#include <code_location.h>
#include <cpp_results.h>
#include <string>

TEST(TestErrors, TestBasicError) {
  errors::error err;
  EXPECT_EQ(err, nullptr);

  err = errors::make_error("%s - some problem: %s", code_location(), "ops");
  EXPECT_NE(err, nullptr);

  std::string str;
  str = err->message();

  EXPECT_EQ(err->type(), errors::err_type::generic_error);
  EXPECT_TRUE(str.find(" - some problem: ops") != std::string::npos);
}

TEST(TestErrors, TestSizedError) {
  errors::error err;
  err = errors::make_serror(18, "some problem: %s", "ops");

  EXPECT_STREQ(err->cmessage(), "some problem: ops");
  EXPECT_EQ(err->type(), errors::err_type::generic_error);

  err = errors::make_serror(8, "some problem: %s", "ops");
  EXPECT_STREQ(err->cmessage(), "some pr");
  EXPECT_EQ(err->type(), errors::err_type::generic_error);
}

TEST(TestErrors, TestTypedError) {
  errors::error err;
  err = errors::make_terror(errors::err_type::null_pointer, "some problem: %s", "ops");

  EXPECT_STREQ(err->cmessage(), "some problem: ops");
  EXPECT_EQ(err->type(), errors::err_type::null_pointer);

  err = errors::make_terror(errors::err_type::already_exists, "some problem: %s", "blah");
  EXPECT_STREQ(err->cmessage(), "some problem: blah");
  EXPECT_EQ(err->type(), errors::err_type::already_exists);
}

TEST(TestErrors, TestTypedSizedError) {
  errors::error err;
  err = errors::make_tserror(errors::err_type::null_pointer, 18, "some problem: %s", "ops");

  EXPECT_STREQ(err->cmessage(), "some problem: ops");
  EXPECT_EQ(err->type(), errors::err_type::null_pointer);

  err = errors::make_tserror(errors::err_type::already_exists, 8, "some problem: %s", "blah");
  EXPECT_STREQ(err->cmessage(), "some pr");
  EXPECT_EQ(err->type(), errors::err_type::already_exists);
}

TEST(TestErrors, TestBasicAppend) {
  errors::error err;
  err = errors::make_serror(18, "some problem: %s", "ops");
  err->append("hey there %s", "mate");

  auto couples = err->couples();
  EXPECT_STREQ(couples[0].message.c_str(), "some problem: ops");
  EXPECT_STREQ(couples[1].message.c_str(), "hey there mate");
}

TEST(TestErrors, TestSizedAppend) {
  errors::error err;
  err = errors::make_error("some problem: %s", "ops");
  err->sappend(8, "hey there %s", "mate");

  auto couples = err->couples();
  EXPECT_STREQ(couples[0].message.c_str(), "some problem: ops");
  EXPECT_STREQ(couples[1].message.c_str(), "hey the");
}

TEST(TestErrors, TestTypedAppend) {
  errors::error err;
  err = errors::make_error("some problem: %s", "ops");
  err->tappend(errors::err_type::already_exists, "hey there %s", "mate");

  auto couples = err->couples();
  EXPECT_STREQ(couples[0].message.c_str(), "some problem: ops");
  EXPECT_EQ(couples[0].type, errors::err_type::generic_error);

  EXPECT_STREQ(couples[1].message.c_str(), "hey there mate");
  EXPECT_EQ(couples[1].type, errors::err_type::already_exists);
}

TEST(TestErrors, TestTypedSizedAppend) {
  errors::error err;
  err = errors::make_error("some problem: %s", "ops");
  err->tsappend(errors::err_type::already_exists, 8, "hey there %s", "mate");

  auto couples = err->couples();
  EXPECT_STREQ(couples[0].message.c_str(), "some problem: ops");
  EXPECT_EQ(couples[0].type, errors::err_type::generic_error);
  EXPECT_STREQ(couples[1].message.c_str(), "hey the");
  EXPECT_EQ(couples[1].type, errors::err_type::already_exists);
}

TEST(TestErrors, TestErrorTypeStrings) {
  errors::error err;
  err = errors::make_error("some problem: %s", "ops");
  err->tappend(errors::err_type::already_exists, "hey there %s", "mate");
  err->tappend(errors::err_type::null_pointer, "some other error %s", "blah");

  auto couples = err->couples();
  EXPECT_STREQ(errors::c_str((couples[0].type)), "generic_error");
  EXPECT_STREQ(errors::c_str((couples[1].type)), "already_exists");
  EXPECT_STREQ(errors::c_str((couples[2].type)), "null_pointer");
}

TEST(TestResultPairs, TestIntegerResultPair) {
  auto proc = [](bool b, int&& val) -> results::result_pair<int> {
    if (b) {
      return results::result_pair<int>(std::move(val));
    }
    return results::result_pair<int>(errors::make_error("false"));
  };

  results::result_pair<int> r = proc(true, 3);
  EXPECT_EQ(r.first, 3);
  EXPECT_EQ(r.second, nullptr);

  {
    auto [result_pair, err] = proc(true, 3);
    EXPECT_EQ(result_pair, 3);
    EXPECT_EQ(err, nullptr);
  }

  r = proc(false, 3);
  EXPECT_EQ(r.first, 0);
  std::string str = r.second->cmessage();
  EXPECT_TRUE(str.find("false") != std::string::npos);

  {
    auto [result_pair, err] = proc(false, 3);
    EXPECT_EQ(result_pair, 0);
    str = err->cmessage();
    EXPECT_TRUE(str.find("false") != std::string::npos);
  }
}

TEST(TestResultPairs, TestDoubleResultPair) {
  auto proc = [](bool b, double&& val) -> results::result_pair<double> {
    if (b) {
      return results::result_pair<double>(std::move(val));
    }
    return results::result_pair<double>(errors::make_error("false"));
  };

  results::result_pair<double> r = proc(true, 3.14);
  EXPECT_EQ(r.first, 3.14);
  EXPECT_EQ(r.second, nullptr);

  {
    auto [result_pair, err] = proc(true, 2.71);
    EXPECT_EQ(result_pair, 2.71);
    EXPECT_EQ(err, nullptr);
  }

  r = proc(false, 3.14);
  EXPECT_EQ(r.first, 0);
  std::string str = r.second->cmessage();
  EXPECT_TRUE(str.find("false") != std::string::npos);

  {
    auto [result_pair, err] = proc(false, 2.71);
    EXPECT_EQ(result_pair, 0);
    str = err->cmessage();
    EXPECT_TRUE(str.find("false") != std::string::npos);
  }
}

TEST(TestResultPairs, TestStringResultPair) {
  auto proc = [](bool b, std::string&& val) -> results::result_pair<std::string> {
    if (b) {
      return results::result_pair<std::string>(std::move(val));
    }
    return results::result_pair<std::string>(errors::make_error("false"));
  };

  results::result_pair<std::string> r = proc(true, "hello");
  EXPECT_STREQ(r.first.c_str(), "hello");
  EXPECT_EQ(r.second, nullptr);

  {
    auto [result_pair, err] = proc(true, "world");
    EXPECT_STREQ(result_pair.c_str(), "world");
    EXPECT_EQ(err, nullptr);
  }

  r = proc(false, "3.14");
  EXPECT_STREQ(r.first.c_str(), "");
  std::string str = r.second->cmessage();
  EXPECT_TRUE(str.find("false") != std::string::npos);

  {
    auto [result_pair, err] = proc(false, "2.71");
    EXPECT_STREQ(result_pair.c_str(), "");
    str = err->cmessage();
    EXPECT_TRUE(str.find("false") != std::string::npos);
  }
}

TEST(TestResultPairs, TestCompositeResultPair) {
  typedef struct composite_t {
    int a;
    double b;
    std::string c;
  } composite_t;

  auto proc = [](bool b, composite_t&& val) -> results::result_pair<composite_t> {
    if (b) {
      return results::result_pair<composite_t>(std::move(val));
    }
    return results::result_pair<composite_t>(errors::make_error("false"));
  };

  results::result_pair<composite_t> r = proc(true, composite_t{.a = 3, .b = 3.14, .c = "hello"});

  EXPECT_EQ(r.first.a, 3);
  EXPECT_EQ(r.first.b, 3.14);
  EXPECT_STREQ(r.first.c.c_str(), "hello");
  EXPECT_EQ(r.second, nullptr);

  {
    auto [result_pair, err] = proc(true, composite_t{.a = 3, .b = 3.14, .c = "hello"});
    EXPECT_EQ(result_pair.a, 3);
    EXPECT_EQ(result_pair.b, 3.14);
    EXPECT_STREQ(result_pair.c.c_str(), "hello");
    EXPECT_EQ(err, nullptr);
  }

  r = proc(false, composite_t{.a = 3, .b = 3.14, .c = "hello"});
  EXPECT_EQ(r.first.a, 0);
  EXPECT_EQ(r.first.b, 0);
  EXPECT_STREQ(r.first.c.c_str(), "");
  std::string str = r.second->cmessage();
  EXPECT_TRUE(str.find("false") != std::string::npos);

  {
    auto [result_pair, err] = proc(false, composite_t{.a = 3, .b = 3.14, .c = "hello"});
    EXPECT_EQ(result_pair.a, 0);
    EXPECT_EQ(result_pair.b, 0);
    EXPECT_STREQ(result_pair.c.c_str(), "");
    str = err->cmessage();
    EXPECT_TRUE(str.find("false") != std::string::npos);
  }
}

TEST(TestResults, TestIntegerResult) {
  auto proc = [](bool b, int&& val) -> results::result<int> {
    if (b) {
      return results::result<int>(std::move(val));
    }
    return results::result<int>(errors::make_error("false"));
  };

  auto r = proc(true, 3);
  EXPECT_EQ(r.error(), nullptr);
  EXPECT_EQ(r.value(), 3);

  r = proc(false, 3);
  auto e = r.error();
  EXPECT_NE(e, nullptr);
  std::string str = e->cmessage();
  EXPECT_TRUE(str.find("false") != std::string::npos);
  EXPECT_DEATH(r.value(), "");
}

TEST(TestResults, TestDoubleResult) {
  auto proc = [](bool b, double&& val) -> results::result<double> {
    if (b) {
      return results::result<double>(std::move(val));
    }
    return results::result<double>(errors::make_error("false"));
  };

  auto r = proc(true, 3.14);
  EXPECT_EQ(r.error(), nullptr);
  EXPECT_EQ(r.value(), 3.14);

  r = proc(false, 3.14);
  auto e = r.error();
  EXPECT_NE(e, nullptr);
  std::string str = e->cmessage();
  EXPECT_TRUE(str.find("false") != std::string::npos);
  EXPECT_DEATH(r.value(), "");
}

TEST(TestResults, TestStringResult) {
  auto proc = [](bool b, std::string&& val) -> results::result<std::string> {
    if (b) {
      return results::result<std::string>(std::move(val));
    }
    return results::result<std::string>(errors::make_error("false"));
  };

  auto r = proc(true, "hello");
  EXPECT_EQ(r.error(), nullptr);
  EXPECT_EQ(r.value(), "hello");

  r = proc(false, "hello");
  auto e = r.error();
  EXPECT_NE(e, nullptr);
  std::string str = e->cmessage();
  EXPECT_TRUE(str.find("false") != std::string::npos);
  EXPECT_DEATH(r.value(), "");
}

TEST(TestResults, TestCompositeResult) {
  typedef struct composite_t {
    int a;
    double b;
    std::string c;
  } composite_t;

  auto proc = [](bool b, composite_t&& val) -> results::result<composite_t> {
    if (b) {
      return results::result<composite_t>(std::move(val));
    }
    return results::result<composite_t>(errors::make_error("false"));
  };

  auto r = proc(true, composite_t{.a = 3, .b = 3.14, .c = "hello"});

  EXPECT_EQ(r.error(), nullptr);
  auto value = r.value();
  EXPECT_EQ(value.a, 3);
  EXPECT_EQ(value.b, 3.14);
  EXPECT_STREQ(value.c.c_str(), "hello");

  r = proc(false, composite_t{.a = 3, .b = 3.14, .c = "hello"});
  auto e = r.error();
  EXPECT_NE(e, nullptr);
  std::string str = e->cmessage();
  EXPECT_TRUE(str.find("false") != std::string::npos);
  EXPECT_DEATH(r.value(), "");
}

int main_(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  (void)(::testing::GTEST_FLAG(death_test_style) = "threadsafe");
  return RUN_ALL_TESTS();
}

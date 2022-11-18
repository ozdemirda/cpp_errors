#include <gtest/gtest.h>
#include <cpp_errors.h>
#include <code_location.h>
#include <string>

TEST(TestErrors, TestBasicError) {
	errors::error err;
	EXPECT_EQ(err, nullptr);

	err = errors::make_error("%s - some problem: %s", code_locationc(), "ops");
	EXPECT_NE(err, nullptr);

	std::string str;
	str = err->message();

	EXPECT_EQ(err->type(), errors::err_type::generic_error);
	EXPECT_TRUE(str.find("TestErrors") != std::string::npos);
	EXPECT_TRUE(str.find("TestBasicError") != std::string::npos);
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
	err = errors::make_terror(errors::err_type::null_pointer,
			"some problem: %s", "ops");

	EXPECT_STREQ(err->cmessage(), "some problem: ops");
	EXPECT_EQ(err->type(), errors::err_type::null_pointer);

	err = errors::make_terror(errors::err_type::already_exists,
			"some problem: %s", "blah");
	EXPECT_STREQ(err->cmessage(), "some problem: blah");
	EXPECT_EQ(err->type(), errors::err_type::already_exists);
}

TEST(TestErrors, TestTypedSizedError) {
	errors::error err;
	err = errors::make_tserror(errors::err_type::null_pointer, 18,
			"some problem: %s", "ops");

	EXPECT_STREQ(err->cmessage(), "some problem: ops");
	EXPECT_EQ(err->type(), errors::err_type::null_pointer);

	err = errors::make_tserror(errors::err_type::already_exists,
			8, "some problem: %s", "blah");
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
	err->tappend(errors::err_type::already_exists,
			"hey there %s", "mate");

	auto couples = err->couples();
	EXPECT_STREQ(couples[0].message.c_str(), "some problem: ops");
	EXPECT_EQ(couples[0].type, errors::err_type::generic_error);

	EXPECT_STREQ(couples[1].message.c_str(), "hey there mate");
	EXPECT_EQ(couples[1].type, errors::err_type::already_exists);
}

TEST(TestErrors, TestTypedSizedAppend) {
	errors::error err;
	err = errors::make_error("some problem: %s", "ops");
	err->tsappend(errors::err_type::already_exists,
			8, "hey there %s", "mate");

	auto couples = err->couples();
	EXPECT_STREQ(couples[0].message.c_str(), "some problem: ops");
	EXPECT_EQ(couples[0].type, errors::err_type::generic_error);
	EXPECT_STREQ(couples[1].message.c_str(), "hey the");
	EXPECT_EQ(couples[1].type, errors::err_type::already_exists);
}

TEST(TestErrors, TestErrorTypeStrings) {
	errors::error err;
	err = errors::make_error("some problem: %s", "ops");
	err->tappend(errors::err_type::already_exists,
			"hey there %s", "mate");
	err->tappend(errors::err_type::null_pointer,
			"some other error %s", "blah");

	auto couples = err->couples();
	EXPECT_STREQ(errors::c_str((couples[0].type)), "generic_error");
	EXPECT_STREQ(errors::c_str((couples[1].type)), "already_exists");
	EXPECT_STREQ(errors::c_str((couples[2].type)), "null_pointer");
}

int main_(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

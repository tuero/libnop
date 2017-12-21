#include <gtest/gtest.h>

#include <cstdint>
#include <iostream>
#include <string>

#include <nop/base/members.h>
#include <nop/base/utility.h>

using nop::EnableIfHasMemberList;
using nop::EnableIfNotHasMemberList;
using nop::HasExternalMemberList;
using nop::HasMemberList;
using nop::IsIntegral;
using nop::IsSame;
using nop::IsUnique;

namespace {

struct TestMemberList {
  int a;
  std::string b;

 private:
  NOP_MEMBERS(TestMemberList, a, b);
};

struct TestNoMemberList {
  int a;
  std::string b;
};

struct TestExternalMemberList {
  int a;
  std::string b;
};
NOP_STRUCTURE(TestExternalMemberList, a, b);

struct TestExternalMemberList2 {
  float a;
  uint64_t b;
};
NOP_STRUCTURE(TestExternalMemberList2, a, b);

template <typename T>
EnableIfHasMemberList<T, bool> CheckHasMemberList(const T& value) {
  return true;
}

template <typename T>
EnableIfNotHasMemberList<T, bool> CheckHasMemberList(const T& value) {
  return false;
}

}  // anonymous namespace

TEST(Utility, HasMemberList) {
  EXPECT_TRUE(HasMemberList<TestMemberList>::value);
  EXPECT_TRUE(HasMemberList<TestExternalMemberList>::value);
  EXPECT_TRUE(HasMemberList<TestExternalMemberList2>::value);
  EXPECT_FALSE(HasMemberList<TestNoMemberList>::value);

  EXPECT_TRUE(CheckHasMemberList(TestMemberList{10, "foo"}));
  EXPECT_TRUE(CheckHasMemberList(TestExternalMemberList{10, "foo"}));
  EXPECT_TRUE(CheckHasMemberList(TestExternalMemberList2{10, 20u}));
  EXPECT_FALSE(CheckHasMemberList(TestNoMemberList{10, "foo"}));
}

TEST(Utility, IsIntegral) {
  EXPECT_TRUE(IsIntegral<std::uint8_t>::value);
  EXPECT_TRUE(IsIntegral<std::int8_t>::value);
  EXPECT_TRUE(IsIntegral<std::uint16_t>::value);
  EXPECT_TRUE(IsIntegral<std::int16_t>::value);
  EXPECT_TRUE(IsIntegral<std::uint32_t>::value);
  EXPECT_TRUE(IsIntegral<std::int32_t>::value);
  EXPECT_TRUE(IsIntegral<std::uint64_t>::value);
  EXPECT_TRUE(IsIntegral<std::int64_t>::value);
  EXPECT_TRUE(IsIntegral<char>::value);
  EXPECT_TRUE(IsIntegral<std::size_t>::value);

  EXPECT_FALSE(IsIntegral<void>::value);
  EXPECT_FALSE(IsIntegral<float>::value);
  EXPECT_FALSE(IsIntegral<double>::value);

  EXPECT_TRUE((IsIntegral<int, short>::value));
  EXPECT_TRUE((IsIntegral<int, short, char>::value));

  EXPECT_FALSE((IsIntegral<float, short, char>::value));
  EXPECT_FALSE((IsIntegral<int, float, char>::value));
  EXPECT_FALSE((IsIntegral<int, short, float>::value));
  EXPECT_FALSE((IsIntegral<int, float, float>::value));
  EXPECT_FALSE((IsIntegral<float, float, char>::value));
  EXPECT_FALSE((IsIntegral<float, float, float>::value));
}

namespace {

template <typename A, typename B>
using SameUnderlyingType = std::is_same<std::decay_t<A>, std::decay_t<B>>;

}  // anonymous namespace

TEST(Utility, IsUnique) {
  EXPECT_TRUE((IsUnique<std::is_same>::value));
  EXPECT_TRUE((IsUnique<std::is_same, int>::value));
  EXPECT_TRUE((IsUnique<std::is_same, int, float>::value));
  EXPECT_TRUE((IsUnique<std::is_same, int, float, char>::value));
  EXPECT_TRUE((IsUnique<std::is_same, int, float, char, short>::value));

  EXPECT_FALSE((IsUnique<std::is_same, int, int>::value));
  EXPECT_FALSE((IsUnique<std::is_same, int, float, float>::value));
  EXPECT_FALSE((IsUnique<std::is_same, int, int, float>::value));
  EXPECT_FALSE((IsUnique<std::is_same, float, int, float>::value));
  EXPECT_FALSE((IsUnique<std::is_same, int, float, short, short>::value));

  EXPECT_FALSE((IsUnique<SameUnderlyingType, int, const int>::value));
  EXPECT_FALSE((IsUnique<SameUnderlyingType, int, float, const float>::value));
  EXPECT_FALSE((IsUnique<SameUnderlyingType, int, volatile int>::value));
}

TEST(Utility, IsSame) {
  EXPECT_TRUE((IsSame<std::is_same>::value));
  EXPECT_TRUE((IsSame<std::is_same, int>::value));
  EXPECT_TRUE((IsSame<std::is_same, int, int>::value));
  EXPECT_TRUE((IsSame<std::is_same, int, int, int>::value));

  EXPECT_FALSE((IsSame<std::is_same, int, float>::value));
  EXPECT_FALSE((IsSame<std::is_same, float, int, int>::value));
  EXPECT_FALSE((IsSame<std::is_same, int, float, int>::value));
  EXPECT_FALSE((IsSame<std::is_same, int, int, float>::value));
}

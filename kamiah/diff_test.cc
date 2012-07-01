/**
 * @file diff_test.cc
 * @brief Unit tests for a Diff.
 *
 * @author Victor Marmol (vmarmol@gmail.com)
 */

#include "diff.h"

#include "gtest/gtest.h"

namespace kamiah {

TEST(DiffTest, CreateInsert) {
  string inserted_text = "papaya";
  Diff diff(12, inserted_text);

  EXPECT_EQ(Diff::INSERT, diff.type());
  EXPECT_EQ(12, diff.index());
  EXPECT_EQ(inserted_text, diff.text());
}

TEST(DiffTest, CreateDelete) {
  Diff diff(12, 10);

  EXPECT_EQ(Diff::DELETE, diff.type());
  EXPECT_EQ(12, diff.index());
  EXPECT_EQ(10, diff.length());
}

TEST(DiffTest, VersionInitiallyEmpty) {
  Diff insert_diff(12, "papaya");
  Diff delete_diff(12, 10);

  EXPECT_EQ(-1, insert_diff.version());
  EXPECT_EQ(-1, delete_diff.version());
}

TEST(DiffTest, SetVersion) {
  Diff insert_diff(12, "papaya");
  Diff delete_diff(12, 10);
  insert_diff.set_version(17);
  delete_diff.set_version(18);

  EXPECT_EQ(17, insert_diff.version());
  EXPECT_EQ(18, delete_diff.version());
}

}  // namespace kamiah

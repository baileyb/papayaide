/**
 * @file document_test.cc
 * @brief Unit tests for a Document.
 *
 * @author Victor Marmol (vmarmol@gmail.com)
 */

#include "document.h"

#include "gtest/gtest.h"

namespace kamiah {

TEST(DocumentTest, InitialDocument) {
  Document doc(13);

  EXPECT_EQ(13, doc.doc_id());

  // Version 0
  EXPECT_EQ(0, doc.version());

  // Empty doc
  string data;
  doc.GetData(&data);
  EXPECT_TRUE(data.empty());

  // No updates
  list<Diff> updates;
  EXPECT_FALSE(doc.GetUpdates(0, &updates));
  EXPECT_TRUE(updates.empty());
  EXPECT_FALSE(doc.GetUpdates(1, &updates));
  EXPECT_TRUE(updates.empty());
}

TEST(DocumentTest, OneInsert) {
  Document doc(1);

  string content = "papaya";
  Diff diff(0, content);

  EXPECT_TRUE(doc.ApplyDiff(&diff));
  EXPECT_EQ(1, diff.version());
  EXPECT_EQ(1, doc.version());

  // Check data
  string data;
  doc.GetData(&data);
  EXPECT_EQ(content, data);

  // Check diffs
  list<Diff> diffs;
  EXPECT_TRUE(doc.GetUpdates(1, &diffs));
  EXPECT_EQ(1U, diffs.size());
  EXPECT_EQ(1, diffs.begin()->version());
}

TEST(DocumentTest, InsertThenDelete) {
  Document doc(1);

  // Add "papaya" to the doc
  string content = "papaya";
  Diff diff(0, content);
  EXPECT_TRUE(doc.ApplyDiff(&diff));
  EXPECT_EQ(1, diff.version());
  EXPECT_EQ(1, doc.version());

  // Delete all of "papaya"
  Diff delete_diff(0, content.size());
  EXPECT_TRUE(doc.ApplyDiff(&delete_diff));
  EXPECT_EQ(2, delete_diff.version());
  EXPECT_EQ(2, doc.version());

  // Check data
  string data;
  doc.GetData(&data);
  EXPECT_TRUE(data.empty());

  // Check diffs
  list<Diff> diffs;
  EXPECT_TRUE(doc.GetUpdates(1, &diffs));
  EXPECT_EQ(2U, diffs.size());
  EXPECT_EQ(1, diffs.front().version());
  EXPECT_EQ(2, diffs.back().version());
}

TEST(DocumentTest, ApplyDiffTestIndexValues) {
  Document doc(1);

  // Add "papaya" to the doc
  string content = "papaya";
  Diff diff1(0, content);
  EXPECT_TRUE(doc.ApplyDiff(&diff1));

  // Try to insert and delete at the end of the doc
  Diff diff2(content.size(), 10);
  Diff diff3(content.size(), "papaya");
  EXPECT_TRUE(doc.ApplyDiff(&diff2));
  EXPECT_TRUE(doc.ApplyDiff(&diff3));

  // Try to insert and delete past the end of the doc
  Length doc_size = content.size() * 2;
  Diff diff4(doc_size + 1, 10);
  Diff diff5(doc_size + 1, content);
  EXPECT_FALSE(doc.ApplyDiff(&diff4));
  EXPECT_FALSE(doc.ApplyDiff(&diff5));

  // Try to insert and delete past the beginning of the doc
  Diff diff6(-1, 10);
  Diff diff7(-1, content);
  EXPECT_FALSE(doc.ApplyDiff(&diff6));
  EXPECT_FALSE(doc.ApplyDiff(&diff7));
}

TEST(DocumentTest, MultipleInserts) {
  Document doc(1);

  // Add "bef_papaya_aft" to the file
  Diff diff1(0, "papaya");
  Diff diff2(0, "bef_");
  Diff diff3(strlen("bef_papaya"), "_aft");
  EXPECT_TRUE(doc.ApplyDiff(&diff1));
  EXPECT_TRUE(doc.ApplyDiff(&diff2));
  EXPECT_TRUE(doc.ApplyDiff(&diff3));
  EXPECT_EQ(1, diff1.version());
  EXPECT_EQ(2, diff2.version());
  EXPECT_EQ(3, diff3.version());
  EXPECT_EQ(3, doc.version());

  // Check data
  string data;
  doc.GetData(&data);
  EXPECT_EQ("bef_papaya_aft", data);

  // Check diffs
  list<Diff> diffs;
  EXPECT_TRUE(doc.GetUpdates(1, &diffs));
  EXPECT_EQ(3U, diffs.size());
}

TEST(DocumentTest, MultipleDeletes) {
  Document doc(1);

  // Add "bef_papaya_aft" to the file
  Diff diff1(0, "papaya-papaya");
  Diff diff2(strlen("papaya-"), strlen("papaya")); // remove the second papaya
  Diff diff3(0, strlen("papa")); // remove the first "papa"
  EXPECT_TRUE(doc.ApplyDiff(&diff1));
  EXPECT_TRUE(doc.ApplyDiff(&diff2));
  EXPECT_TRUE(doc.ApplyDiff(&diff3));
  EXPECT_EQ(1, diff1.version());
  EXPECT_EQ(2, diff2.version());
  EXPECT_EQ(3, diff3.version());
  EXPECT_EQ(3, doc.version());

  // Check data
  string data;
  doc.GetData(&data);
  EXPECT_EQ("ya-", data);

  // Check diffs
  list<Diff> diffs;
  EXPECT_TRUE(doc.GetUpdates(1, &diffs));
  EXPECT_EQ(3U, diffs.size());
}

TEST(DocumentTest, DiffCache) {
  Document doc(1);

  // Add "papaya-papaya" to the file
  Diff diff1(0, "papaya-papaya");
  Diff diff2(0, strlen("papaya"));
  Diff diff3(0, "papaya");
  EXPECT_TRUE(doc.ApplyDiff(&diff1));
  EXPECT_TRUE(doc.ApplyDiff(&diff2));
  EXPECT_TRUE(doc.ApplyDiff(&diff3));
  EXPECT_EQ(3, doc.version());

  // Check "newer" version
  list<Diff> diffs;
  EXPECT_TRUE(doc.GetUpdates(4, &diffs));
  EXPECT_EQ(0U, diffs.size());

  // Check latest addition
  diffs.clear();
  EXPECT_TRUE(doc.GetUpdates(3, &diffs));
  EXPECT_EQ(1U, diffs.size());
  EXPECT_EQ(3, diffs.front().version());

  // Check latest 2 additions
  diffs.clear();
  EXPECT_TRUE(doc.GetUpdates(2, &diffs));
  EXPECT_EQ(2U, diffs.size());
  EXPECT_EQ(3, diffs.back().version());
  EXPECT_EQ(2, diffs.front().version());

  // Check all
  diffs.clear();
  EXPECT_TRUE(doc.GetUpdates(1, &diffs));
  EXPECT_EQ(3U, diffs.size());
  EXPECT_EQ(3, diffs.back().version());
  EXPECT_EQ(1, diffs.front().version());

  // Check too early
  diffs.clear();
  EXPECT_FALSE(doc.GetUpdates(0, &diffs));
  EXPECT_EQ(0U, diffs.size());
}

TEST(DocumentTest, DiffCacheMaxSize) {
  Document doc(1);

  // Add just as many "papaya" as there are spots in the cache
  Version max_cache = Document::kMaxCacheSize;
  for (Version i = 0; i < max_cache; ++i) {
    Diff diff(0, "papaya");
    EXPECT_TRUE(doc.ApplyDiff(&diff));
  }
  EXPECT_EQ(max_cache, doc.version());

  // Check "newer" version
  list<Diff> diffs;
  EXPECT_TRUE(doc.GetUpdates(max_cache + 1, &diffs));
  EXPECT_EQ(0U, diffs.size());

  // Check latest addition
  diffs.clear();
  EXPECT_TRUE(doc.GetUpdates(max_cache, &diffs));
  EXPECT_EQ(1U, diffs.size());
  EXPECT_EQ(max_cache, diffs.front().version());

  // Check latest 2 additions
  diffs.clear();
  EXPECT_TRUE(doc.GetUpdates(max_cache - 1, &diffs));
  EXPECT_EQ(2U, diffs.size());
  EXPECT_EQ(max_cache, diffs.back().version());
  EXPECT_EQ(max_cache - 1, diffs.front().version());

  // Check all
  diffs.clear();
  EXPECT_TRUE(doc.GetUpdates(1, &diffs));
  EXPECT_EQ((size_t) max_cache, diffs.size());
  EXPECT_EQ(max_cache, diffs.back().version());
  EXPECT_EQ(1, diffs.front().version());

  // Check too early
  diffs.clear();
  EXPECT_FALSE(doc.GetUpdates(0, &diffs));
  EXPECT_EQ(0U, diffs.size());
}

TEST(DocumentTest, DiffCacheLargerThanMaxSize) {
  Document doc(1);

  // Add one more "papaya" than there is room for in the diff cache
  Version num_diffs = Document::kMaxCacheSize + 1;
  for (Version i = 0; i < num_diffs; ++i) {
    Diff diff(0, "papaya");
    EXPECT_TRUE(doc.ApplyDiff(&diff));
  }
  EXPECT_EQ(num_diffs, doc.version());

  // Check "newer" version
  list<Diff> diffs;
  EXPECT_TRUE(doc.GetUpdates(num_diffs + 1, &diffs));
  EXPECT_EQ(0U, diffs.size());

  // Check latest addition
  diffs.clear();
  EXPECT_TRUE(doc.GetUpdates(num_diffs, &diffs));
  EXPECT_EQ(1U, diffs.size());
  EXPECT_EQ(num_diffs, diffs.front().version());

  // Check latest 2 additions
  diffs.clear();
  EXPECT_TRUE(doc.GetUpdates(num_diffs - 1, &diffs));
  EXPECT_EQ(2U, diffs.size());
  EXPECT_EQ(num_diffs, diffs.back().version());
  EXPECT_EQ(num_diffs - 1, diffs.front().version());

  // Check for the last one in the cache
  diffs.clear();
  size_t max_cache = Document::kMaxCacheSize;
  Version last_in_cache = num_diffs - max_cache + 1;
  EXPECT_TRUE(doc.GetUpdates(last_in_cache, &diffs));
  EXPECT_EQ(max_cache, diffs.size());
  EXPECT_EQ(num_diffs, diffs.back().version());
  EXPECT_EQ(last_in_cache, diffs.front().version());

  // Check too early
  diffs.clear();
  EXPECT_FALSE(doc.GetUpdates(1, &diffs));
  EXPECT_EQ(0U, diffs.size());
  diffs.clear();
  EXPECT_FALSE(doc.GetUpdates(0, &diffs));
  EXPECT_EQ(0U, diffs.size());
}

}  // namespace kamiah

/**
 * @file document.h
 * @brief Definition of a Document.
 *
 * @author Victor Marmol (vmarmol@gmail.com)
 */

#include <list>
#include <string>

#include "diff.h"
#include "types.h"

using std::list;
using std::string;

namespace kamiah {

// TODO(vmarmol): Optimizations: 
//   - diffs_: array-backed circular buffer.
//   - data_: Rope.
//   - We copy a lot of data both here and in Diff.

/**
 * @brief A Document is the datastructure that backs a file that is being
 *     concurrently edited in PapayaIDE.
 *
 * A Document keeps a cache of the last kMaxCacheSize diffs that have been
 * applied to the document. It also keeps the full text of the file.
 *
 * This class is thread-compatible.
 */
class Document {
 public:
  // Max size of the diff cache.
  static const size_t kMaxCacheSize = 10;

  /**
   * @brief Default constructor of a Document.
   */
  Document();

  /**
   * @brief Applies the specified diff to the document.
   *
   * @param diff The diff to apply to the document.
   * @return True iff the diff was applied successfully. One possible failure
   *     scenario is a diff with an out of bounds index.
   */
  bool ApplyDiff(Diff *diff);

  /**
   * @brief Gets a list of diffs from the specified version to the latest
   *     Document version (the value returned by version()).
   *
   * This function allows a user to query for any updates to the Document
   * starting at a specific version number.
   *
   * @param from_version The version from which to start getting updates.
   * @param updates List in which to write the outputted diffs.
   * @return True if the updates were populated or no updates are necessary (if
   *     the specified version is greater than the Document's current version).
   *     False otherwise, this indicates that the diffs were not available so
   *     the full data should be requested instead.
   */
  bool GetUpdates(Version from_version, list<Diff> *updates) const;

  /**
   * @brief Get a Document's underlying data (the file contents).
   *
   * @param data String to write the Document data to.
   */
  void GetData(string *data) const;

  /**
   * @brief Gets the current Version of the Document.
   *
   * @return The current Version of the Document.
   */
  Version version() const;

 private:
  Version version_;
  string data_;
  list<Diff> diffs_;
  Version last_cached_diff_;
};

}  // namespace kamiah

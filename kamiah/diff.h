/**
 * @file diff.h
 * @brief Defines a Diff.
 *
 * @author Victor Marmol (vmarmol@gmail.com)
 */

#include <string>

#include "types.h"

using std::string;

namespace kamiah {

/**
 * @brief Defines a Diff which is a unit of change applied to an existing
 *     document.
 *
 * Diffs can be insertions or deletions.
 *   Insertions: Have an index to start inserting at, and what to insert.
 *   Deletions: Have an index to start deleting at, and the number of characters
 *     to delete.
 */
class Diff {
 public:
  enum Type { INSERT, DELETE };

  /**
   * @brief Constructs an INSERT diff.
   *
   * @param index The index at which to insert.
   * @param text The text to insert into the document.
   */
  Diff(Index index, const string& text);

  /**
   * @brief Constructs a DELETE diff.
   *
   * @param index The index at which to delete.
   * @param text The number of characters to delete.
   */
  Diff(Index index, Length length);

  /**
   * @brief Sets the version associated with this diff.
   *
   * @param version The version to set.
   */
  void set_version(Version version);

  /**
   * @brief Gets the version associated with this diff.
   *
   * @return The version associated with this diff.
   */
  Version version() const;

  /**
   * @brief Gets the type of this diff.
   *
   * @return The type of this diff.
   */
  Type type() const;

  /**
   * @brief Gets the index of this diff. Used by both INSERT and DELETE diffs.
   *
   * @return The index of this diff.
   */
  Index index() const;

  /**
   * @brief Gets the length of this diff. Only used for DELETE diffs, this field
   *     is undefined for INSERT diffs..
   *
   * @return The length of this diff.
   */
  Length length() const;

  /**
   * @brief Gets the text of this diff. Only used for INSERT diffs, this field
   *     is undefined for DELETE diffs.
   *
   * @return The text of this diff.
   */
  const string& text() const;

 private:
  Version version_;
  Type type_;
  Index index_;
  Length length_;
  string text_;
};

}  // namespace kamiah

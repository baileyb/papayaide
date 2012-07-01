/**
 * @file diff.cc
 * @brief Implementation of a Diff.
 *
 * @author Victor Marmol (vmarmol@gmail.com)
 */

#include "diff.h"

namespace kamiah {

Diff::Diff(Index index, const string& text)
  : version_(-1), type_(INSERT), index_(index), text_(text) {
}

Diff::Diff(Index index, Length length)
  : version_(-1), type_(DELETE), index_(index), length_(length) {
}

void Diff::set_version(Version version) {
  version_ = version;
}

Version Diff::version() const {
  return version_;
}

Diff::Type Diff::type() const {
  return type_;
}

Index Diff::index() const {
  return index_;
}

Length Diff::length() const {
  return length_;
}

const string& Diff::text() const {
  return text_;
}

}  // namespace kamiah

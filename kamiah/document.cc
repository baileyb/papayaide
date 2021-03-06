/**
 * @file document.cc
 * @brief Implementation of a Document.
 *
 * @author Victor Marmol (vmarmol@gmail.com)
 */

#include "document.h"

namespace kamiah {

Document::Document(DocID doc_id)
    : doc_id_(doc_id), version_(0), last_cached_diff_(-1) {
}

bool Document::ApplyDiff(Diff *diff) {
  // Check for invalid index.
  if ((diff->index() < 0) || (diff->index() > (Length) data_.size())) {
    return false;
  }

  // Increment and set version
  ++version_;
  diff->set_version(version_);

  // Add to our cache 
  diffs_.push_back(*diff);

  // Remove from the cache if it is over size
  if (diffs_.size() > kMaxCacheSize) {
    diffs_.pop_front();
    ++last_cached_diff_;
  } else if (diffs_.size() == 1) {
    // This is the first cached item, set it as the last cached
    last_cached_diff_ = version_;
  }

  // Apply to the document
  switch (diff->type()) {
    case Diff::INSERT:
      data_.insert(diff->index(), diff->text());
      break;
    case Diff::DELETE:
      data_.erase(diff->index(), diff->length());
      break;
  }

  return true;
}

bool Document::GetUpdates(Version from_version, list<Diff> *updates) const {
  // Check if the requested updates are no longer cached or we don't have any
  // diffs.
  if ((from_version < last_cached_diff_) || (last_cached_diff_ == -1)) {
    return false;
  } else if (from_version > version_) {
    // You have a version greater than this doc, no updates available
    return true;
  }

  // Find the first update that was requested
  list<Diff>::const_iterator it = diffs_.begin();
  while (it->version() != from_version) {
    ++it;
  }

  // Add all the diffs to the output list
  for (Version v = from_version; v <= version_; ++v) {
    updates->push_back(*it);
    ++it;
  }

  return true;
}

void Document::GetData(string *data) const {
  *data = data_;
}

DocID Document::doc_id() const {
  return doc_id_;
}

Version Document::version() const {
  return version_;
}

}  // namespace kamiah

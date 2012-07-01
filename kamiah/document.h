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

class Document {
 public:
  // Max size of the diff cache.
  static const size_t kMaxCacheSize = 10;

  Document();

  bool ApplyDiff(Diff *diff);

  bool GetUpdates(Version from_version, list<Diff> *updates) const;

  void GetData(string *data) const;

  Version version() const;

 private:
  Version version_;
  string data_;
  list<Diff> diffs_;
  Version last_cached_diff_;
};

}  // namespace kamiah

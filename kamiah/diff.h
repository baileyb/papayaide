#include <string>

#include "types.h"

using std::string;

namespace kamiah {

class Diff {
 public:
  enum Type { INSERT, DELETE };

  Diff(Index index, const string& text);

  Diff(Index index, Length length);

  void set_version(Version version);

  Version version() const;

  Type type() const;

  Index index() const;

  Length length() const;

  const string& text() const;

 private:
  Version version_;
  Type type_;
  Index index_;
  Length length_;
  string text_;
};

}  // namespace kamiah

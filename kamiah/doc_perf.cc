#include <string>

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "document.h"

using std::string;
using kamiah::Diff;
using kamiah::Document;

class ScopedTimer {
 public:
   static const int64_t kMicrosecondsPerSecond = 1000000;

  explicit ScopedTimer(const string& name) : name_(name) {
    // Store start time
    struct timeval start;
    gettimeofday(&start, NULL);
    start_time_ = start.tv_sec * kMicrosecondsPerSecond + start.tv_usec;
  }

  ~ScopedTimer() {
    // Store end time
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    int64_t end = end_time.tv_sec * kMicrosecondsPerSecond + end_time.tv_usec;

    printf("Timer %s - %ld microseconds\n", name_.c_str(), (end - start_time_));
  }

 private:
  string name_;
  int64_t start_time_;
};

int main() {
  int ops = 20000;

  // We only test insertions and deletions at the beginning since that is the 
  // worst-case for us

  // Test insertions only
  {
    // Make a 1MB file
    Document doc(1);
    string big_string;
    for (int i = 0; i < ((1 << 20) / 10); ++i) {
      big_string.append("aaaaaaaaaa");
    }
    Diff first(0, big_string);
    doc.ApplyDiff(&first);

    ScopedTimer timer("Insertions");
    for (int i = 0; i < ops; ++i) {
      Diff diff(0, "papaya-papaya-papaya");

      if (!doc.ApplyDiff(&diff)) {
        printf("Failed to apply diff\n");
      }
    }
  }

  // Test insertions and deletions
  {
    // Make a 1MB file
    Document doc(1);
    string big_string;
    for (int i = 0; i < ((1 << 20) / 10); ++i) {
      big_string.append("aaaaaaaaaa");
    }
    Diff first(0, big_string);
    doc.ApplyDiff(&first);

    ScopedTimer timer("Insertions/Deletions 50/50");
    for (int i = 0; i < ops; ++i) {
      bool applied_ok = false;

      // Half the time insert, the other half delete
      if (i & 0x1) {
        Diff diff(0, "papaya-papaya-papaya");
        applied_ok = doc.ApplyDiff(&diff);
      } else {
        Diff diff(0, strlen("papaya"));
        applied_ok = doc.ApplyDiff(&diff);
      }

      if (!applied_ok) {
        printf("Failed to apply diff\n");
      }
    }
  }

  return 0;
}

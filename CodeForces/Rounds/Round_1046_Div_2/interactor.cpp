//===----------------------------------------------------------------------===//
/**
 * @file: interactor.cpp
 * @brief Interactor for F1/F2 - Simulates judge behavior
 * @author: Costantino Lombardi
 *
 */
//===----------------------------------------------------------------------===//
/* Included library */

#include "algorithms/PCH.h"
#include "../../Algorithms/libs/testlib.h"

using namespace std;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

class Interactor {
private:
  int       width;
  bool      is_f2_mode;
  int       queries_count         = 0;
  long long total_words_submitted = 0;

  static constexpr int MAX_QUERIES        = 2;
  static constexpr int MAX_TOTAL_WORDS_F2 = 25000;

  // Calculates the number of lines required for a given article.
  // Returns 0 if the article cannot be displayed (a word is wider than width).
  [[nodiscard]] int calculate_lines(const vector<int>& article) const {
    int max_word_length = 0;
    for (int word_length : article) {
      if (word_length > max_word_length) {
        max_word_length = word_length;
      }
    }

    if (max_word_length > width) {
      return 0;
    }

    if (article.empty()) {
      return 1;
    }

    int       lines            = 1;
    long long current_line_sum = 0;
    for (int word_length : article) {
      if (current_line_sum + word_length <= width) {
        current_line_sum += word_length;
      } else {
        lines++;
        current_line_sum = word_length;
      }
    }
    return lines;
  }

public:
  Interactor(int w, bool f2_mode) : width(w), is_f2_mode(f2_mode) {}

  // Main interaction loop for a single test case.
  void run() {
    while (true) {
      // Read the prefix ('?' or '!') from the solution's output stream.
      string prefix = ouf.readToken();

      if (prefix == "!") {
        // Handle the final answer.
        int guessed_width = ouf.readInt();
        if (guessed_width != width) {
          quitf(_wa, "Wrong answer: expected %d, got %d", width, guessed_width);
        }
        // Correct answer for this test case.
        return; // Exit the loop to process the next test case.
      }

      if (prefix == "?") {
        // Handle a query.
        queries_count++;
        if (queries_count > MAX_QUERIES) {
          quitf(_wa, "Too many queries: %d > %d", queries_count, MAX_QUERIES);
        }

        int n = ouf.readInt(1, 100000, "n");

        total_words_submitted += n;
        if (is_f2_mode && total_words_submitted > MAX_TOTAL_WORDS_F2) {
          quitf(_wa, "Total words limit exceeded in F2 mode: %lld > %d", total_words_submitted, MAX_TOTAL_WORDS_F2);
        }

        vector<int> article(n);
        for (int i = 0; i < n; ++i) {
          article[i] = ouf.readInt(1, 100000, "a_i");
        }

        int lines = calculate_lines(article);

        // Print the response to the solution. This goes to stdout, which is
        // piped to the solution's stdin.
        cout << lines << endl;
      } else {
        // The solution printed something unexpected.
        quitf(_pe, "Unexpected token: expected '?' or '!', got '%s'", prefix.c_str());
      }
    }
  }
};

//===----------------------------------------------------------------------===//
/* Main function */

auto main(int argc, char* argv[]) -> int {
  // Initialize testlib for interaction.
  registerInteraction(argc, argv);

  // Check for the --f2 flag among the arguments.
  bool is_f2_mode = false;
  for (int i = 1; i < argc; ++i) {
    if (string_view(argv[i]) == "--f2") {
      is_f2_mode = true;
    }
  }

  // Read the number of test cases from the judge's input file (`inf`).
  int t = inf.readInt(1, 10, "t");

  // The interactor must forward the number of test cases to the solution.
  cout << t << endl;

  for (int i = 1; i <= t; ++i) {
    // Set the test case number for more informative verdicts.
    setTestCase(i);

    // Read the secret width `W` for the current test case.
    int w = inf.readInt(1, 100000, "W");

    Interactor interactor(w, is_f2_mode);
    interactor.run();
  }

  // After all test cases, ensure the solution has terminated.
  ouf.readEof();

  // If we reach here, all test cases passed.
  quitf(_ok, "%d test case(s) passed.", t);

  return 0;
}

//===----------------------------------------------------------------------===//

#include "split.hpp"

#include <string>
#include <vector>
#include <cassert>
#include <regex>


namespace {

void test_split_str() {
  std::string text = "This is some text";
  std::vector<std::string> answer = {"This", "is", "some", "text"};

  auto splitted = cls01::split(text, cls01::StringDelimiter(" "));
  assert(splitted.size() == answer.size());
  for (size_t i = 0; i < answer.size(); i++) {
    assert(splitted[i] == answer[i]);
  }
}

void test_split_regex() {
  std::string text = "This3843is347384some7text";
  std::vector<std::string> answer = {"This", "is", "some", "text"};
  auto splitted = cls01::split(text, cls01::RegexDelimiter(std::regex("(\\d+)")));

  assert(splitted.size() == answer.size());
  for (size_t i = 0; i < answer.size(); i++) {
    assert(splitted[i] == answer[i]);
  }
}

void test_longest_sentence() {
  const std::string text = "This. Text. Has. Loongest. Sentence. Really!";
  std::vector<std::string> answer = {"Loongest", "Sentence"};
  std::vector<std::string_view> longest_sentences = cls01::get_longest_sentences(text);

  assert(longest_sentences.size() == answer.size());
  for (size_t i = 0; i < answer.size(); i++) {
    assert(longest_sentences[i] == answer[i]);
  }
}

void test_longest_sentence_custom_delimiter() {
  const std::string text = "This. Text. Has. Loongest! Sentence. Really!";
  std::vector<std::string> answer = {"Loongest", "Sentence"};
  std::vector<std::string_view> longest_sentences = cls01::get_longest_sentences(
          text, cls01::RegexDelimiter(std::regex("[.!]\\s*")));

  assert(longest_sentences.size() == answer.size());
  for (size_t i = 0; i < answer.size(); i++) {
    assert(longest_sentences[i] == answer[i]);
  }
}

}


int main() {
  test_split_str();
  test_split_regex();
  test_longest_sentence();
  test_longest_sentence_custom_delimiter();
  return 0;
}

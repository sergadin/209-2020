#include "text_processor.h"

#include <vector>

std::string TextProcessor::Show() const {
  std::string text;
  for (const auto& data : _data) (text += data) += '\n';
  return text;
}

void TextProcessor::Add(int i, const std::string& text) {
  --i;  // Tranform 1, 2, 3, ... to 0, 1, 2, ...
  std::lock_guard<std::mutex> lock(_mutex);
  if (i > _data.size() || i < 0)
    throw std::out_of_range("Block index is greater than max=" +
                            std::to_string(_data.size() + 1));
  auto it = std::next(_data.begin(), i);
  _data.insert(it, text);
}

void TextProcessor::Search(const std::string& word, size_t N,
                           std::ostream& os) const {
  size_t block_number = 1;
  for (auto& data : _data) {
    std::vector<std::string> contexts;
    size_t i = data.find(word);
    while (i != std::string::npos) {
      int begin = i - N / 2;
      if (begin < 0) begin = 0;
      size_t end = i + word.size() + N / 2;
      if (end > data.size()) end = data.size();
      contexts.push_back(std::string(data.begin() + begin, data.begin() + end));
      i = data.find(word, i + word.size());
    }
    for (int i = 0; i < contexts.size(); ++i)
      os << block_number << ' ' << contexts.size() << i + 1 << contexts[i]
         << std::endl;
    ++block_number;
  }
}

bool TextProcessor::Delete(std::set<int> ids) {
  std::lock_guard<std::mutex> lock(_mutex);
  if (*ids.begin() < 1 || *ids.rbegin() > _data.size()) return false;
  int i = 1;
  auto it = _data.begin();
  for (auto id : ids) {
    for (; i != id; ++i) ++it;
    it = _data.erase(it);
    ++i;
  }
  return true;
}

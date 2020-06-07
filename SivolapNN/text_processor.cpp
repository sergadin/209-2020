#include "text_processor.h"

#include <vector>

std::string TextProcessor::Show() const {
  std::string text;
  for (const auto& data : _data) (text += data) += '\n';
  return text;
}

void TextProcessor::Add(int i, const std::string& text,std::ostream& os) {
  std::lock_guard<std::mutex> lock(_mutex);
  auto it = _data.begin();
   if(i > _data.size())
  {
     os << "No";
     return;
  
  for (; i>= 0 && it != _data.end(); i--) 
  it++;
  _data.insert(it, text);
}

void TextProcessor::Search(const std::string& word, size_t N,
                           std::ostream& os) const {
  size_t block_number = 0;
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
      os << block_number + 1 << ' ' << contexts.size()<<' ' << i <<' '<< contexts[i]
         << std::endl;
    ++block_number;
  }
}

bool TextProcessor::Delete(std::set<int> ids) {
  std::lock_guard<std::mutex> lock(_mutex);
 int i = 1;
auto it = _data.begin();
for(auto& id : ids) {
for(;i != id; ++i) 
{
  ++it;
if(it == _data.end()) return 1;
}
it = _data.erase(it);
++i;
}
return true;
}

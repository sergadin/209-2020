#include <list>
#include <mutex>
#include <ostream>
#include <set>
#include <string>


class TextProcessor {
 public:
  std::string Show() const;
  void Add(int i, const std::string& text,std::ostream& os);
  void Search(const std::string& word, size_t N, std::ostream& os) const;
  bool Delete(std::set<int> ids);

 private:
  std::list<std::string> _data;
  std::mutex _mutex;
};

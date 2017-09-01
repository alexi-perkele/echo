#include "simple_protocol.hpp"

SimpleProtocol::SimpleProtocol() : sum_(0), minmax_(0, 0) {
}

SimpleProtocol::SimpleProtocol(SimpleProtocol &&obj) :
    sum_(std::move(sum_)), data_(std::move(data_)), minmax_(std::move(minmax_)), storage_(std::move(storage_)) {
}

SimpleProtocol &SimpleProtocol::operator=(SimpleProtocol &&obj) {
  if (this != &obj) {
    sum_ = std::move(obj.sum_);
    data_ = std::move(obj.data_);
    minmax_ = std::move(obj.minmax_);
    storage_ = std::move(obj.storage_);
  }
  return *this;
}

void SimpleProtocol::process_data(const std::string &data) {
  storage_.clear();
  // if nothing to process- pass
  if (data.empty()) {
      std::cout << "nothing to process" << std::endl;
      return;
  }
  // extract digits from string to storage_
  for (auto i : data) {
    if (std::isdigit(i))
      storage_.push_back(i - '0');
  }

  if (!storage_.empty()) {
    sort_desc();
    sum_ = sum();
    minmax_ = minmax();
  }
}

std::ostream& SimpleProtocol::print(std::ostream& os) const {
  if (!storage_empty()) {
    for (auto &n : storage_)
      os << " " << n;
    os << " sum: " << sum_;
    os << " min: " << minmax_.first;
    os << " max: " << minmax_.second;
    os << std::endl;

  }
  else {
    os << "empty object" << std::endl;
  }
   
  return os;

}

int SimpleProtocol::sum() const {
  auto s = 0;

  if (!storage_.empty()) {
    for (auto &n : storage_)
      s += n;
  }
  return s;
}

// return MIN and MAX values;
std::pair<int, int> SimpleProtocol::minmax() const {
  if (!storage_empty())
    return std::make_pair(storage_.back(), *storage_.begin());
  return std::make_pair(0, 0);
}

// sort storage in descending order
void SimpleProtocol::sort_desc() {
  if (!storage_empty())
    std::sort(storage_.begin(), storage_.end(), std::greater<int>());
  return;
}


bool SimpleProtocol::storage_empty() const {
  return storage_.empty();
}

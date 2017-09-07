#ifndef _PROTOCOL_IMPL_H
#define _PROTOCOL_IMPL_H


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "iprotocol.hpp"

class SimpleProtocol: public IProtocol {

 public:
  SimpleProtocol();

  SimpleProtocol(SimpleProtocol &&o);

  ~SimpleProtocol() {};

  void process_data(const std::string& data) override;
  
  std::ostream& print(std::ostream &os) const override;
    
  SimpleProtocol& operator=(SimpleProtocol obj);

 private:
  int sum() const;
  std::pair<int, int> minmax() const;
  void sort_desc();
  bool storage_empty() const;
  
  int sum_;
  std::string data_;
  std::pair<int, int> minmax_;
  std::vector<int> storage_;

};



//#include "simpleprotocol.tpp"
#endif

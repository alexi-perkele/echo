/*
 * 
 */
#ifndef IPROTOCOL_HPP
#define IPROTOCOL_HPP



class IProtocol {

 public:
  IProtocol() {};
  virtual ~IProtocol() { };

  virtual void process_data(const std::string& data) = 0;

  virtual std::ostream& print(std::ostream& os) const = 0;
  
  friend std::ostream &operator<<(std::ostream& os, const IProtocol &proto_obj) {
      return proto_obj.print(os);
   };
private:
     // Non copyable:
        IProtocol ( const IProtocol& );
        IProtocol& operator= ( const IProtocol& );
};

#endif


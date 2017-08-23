/*
 * Copyright (c) 2017 alexi kulmametiev@mail.com
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
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

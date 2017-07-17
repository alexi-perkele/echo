#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


class Protocol
    {

    public:
        Protocol( const std::string &data );
        ~Protocol();
        
        void run();

        int sum() const;
        std::pair<int, int> minmax() const;
        
        void sort_desc();
        
        bool storage_empty() const;

        friend std::ostream& operator<< ( std::ostream& os, const Protocol& proto );
    private:
        std::string data_;
        std::vector<int> storage_;
        int sum_;
        std::pair<int, int> minmax_;

    };



#include "protocol.tpp"
#endif

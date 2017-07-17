#include "protocol.hpp"

Protocol::Protocol ( const std::string &data ) : data_ ( data ), sum_ ( 0 ), minmax_ ( 0, 0 )
    {
    std::cout << "Proto! " << data_ <<std::endl;
    // if nothing to process- pass
    if ( data_.empty() ) return;

    // extract digits from string
    for ( auto i : data_ )
        {
        if ( std::isdigit ( i ) )
            storage_.push_back ( i -'0' );  //TODO atoi mayb?
        }
    }

Protocol::~Protocol()
    {
    std::cout << "Bye Proto!" << std::endl;
    }

bool Protocol::storage_empty() const
    {
    return storage_.empty() ? true : false;
    }

int Protocol::sum() const
    {
    auto s = 0;

    if ( !storage_empty() )
        {
        for ( auto& n : storage_ )
            s += n;
        }
    return s;
    }

void Protocol::run()
    {
    if ( !storage_empty() )
        {
        sort_desc();
        sum_ = sum();
        minmax_ = minmax();
        }

    }

// return MIN and MAX falues;
std::pair<int, int> Protocol::minmax() const
    {
    //auto minmax = std::minmax_element ( storage_.begin(), storage_.end() );
    //auto result = std::make_pair ( *minmax.first, *minmax.second );
    if ( !storage_empty() )
        return std::make_pair ( storage_.back(), *storage_.begin() );
    return std::make_pair ( 0,0 );
    }

// sort storage in descending order
void Protocol::sort_desc()
    {
    if ( !storage_empty() )
        std::sort ( storage_.begin(), storage_.end(), std::greater<int>() );
    return;
    }


std::ostream& operator<< ( std::ostream& os, const Protocol& proto )
    {
    if ( !proto.storage_empty() )
        {
        for ( auto& n : proto.storage_ )
            os << " " <<n;
        os << " sum: " << proto.sum_;
        os << " min: " << proto.minmax_.first;
        os << " max: " << proto.minmax_.second;
        os << std::endl;

        }
    else
        {
        os << "nothing to process" << std::endl;
        }
    return os;
    }

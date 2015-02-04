#include "StdAfx.h"
#include "ProtocolParser.h"


bool ProtocolParser::can_parser( std::string & cmd )
{
    int index = raw_data_.find_first_of('\n');
    if (index > 0)
    {
        cmd = raw_data_.substr(0, index);
        raw_data_ = raw_data_.substr(index+1, -1);
        return true;
    }
    return false;
}

void ProtocolParser::push_new_data( const char * pdata, int len )
{
    while (len > 0)
    {
        raw_data_.push_back(*pdata);
        ++pdata;
        --len;
    }
}

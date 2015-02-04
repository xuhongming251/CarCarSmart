//=========================================================================
/**
 *  \file    ProtocolParser.h
 *  \brief   创建文档
 *
 *
 *  \author xuminglong
 *  \date   2015年2月4日
 */
//=========================================================================

#ifndef ProtocolParser_h__
#define ProtocolParser_h__

#include <string>

class ProtocolParser
{
public:
    ProtocolParser() {}
    ~ProtocolParser() {}

    bool can_parser(std::string & cmd);

    void push_new_data(const char * pdata, int len);

private:

    std::string raw_data_;

};



#endif // ProtocolParser_h__

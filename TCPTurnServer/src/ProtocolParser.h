//=========================================================================
/**
 *  \file    ProtocolParser.h
 *  \brief   �����ĵ�
 *
 *
 *  \author xuminglong
 *  \date   2015��2��4��
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

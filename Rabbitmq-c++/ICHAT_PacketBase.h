#pragma once
#include<string>
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;
typedef unsigned char BYTE;
enum PACKETVER
{
    SERVER_PACEKTVER = 2,
    SERVER_PACEKTVER_NEW = 3,
    SERVER_SUBPACKETVER = 1,
    SERVER_SUBPACKETVER2 = 2,
    SERVER_SUBPACKETVER3 = 3,    // 压缩版协议ZIP
    SERVER_SUBPACKETVER4 = 4    // 完全压缩协议 收发都经过zip压缩

};
//发送字节映射表
const static BYTE SendByteMap[256] =
{
    0x70, 0x2F, 0x40, 0x5F, 0x44, 0x8E, 0x6E, 0x45, 0x7E, 0xAB, 0x2C, 0x1F, 0xB4, 0xAC, 0x9D, 0x91,
    0x0D, 0x36, 0x9B, 0x0B, 0xD4, 0xC4, 0x39, 0x74, 0xBF, 0x23, 0x16, 0x14, 0x06, 0xEB, 0x04, 0x3E,
    0x12, 0x5C, 0x8B, 0xBC, 0x61, 0x63, 0xF6, 0xA5, 0xE1, 0x65, 0xD8, 0xF5, 0x5A, 0x07, 0xF0, 0x13,
    0xF2, 0x20, 0x6B, 0x4A, 0x24, 0x59, 0x89, 0x64, 0xD7, 0x42, 0x6A, 0x5E, 0x3D, 0x0A, 0x77, 0xE0,
    0x80, 0x27, 0xB8, 0xC5, 0x8C, 0x0E, 0xFA, 0x8A, 0xD5, 0x29, 0x56, 0x57, 0x6C, 0x53, 0x67, 0x41,
    0xE8, 0x00, 0x1A, 0xCE, 0x86, 0x83, 0xB0, 0x22, 0x28, 0x4D, 0x3F, 0x26, 0x46, 0x4F, 0x6F, 0x2B,
    0x72, 0x3A, 0xF1, 0x8D, 0x97, 0x95, 0x49, 0x84, 0xE5, 0xE3, 0x79, 0x8F, 0x51, 0x10, 0xA8, 0x82,
    0xC6, 0xDD, 0xFF, 0xFC, 0xE4, 0xCF, 0xB3, 0x09, 0x5D, 0xEA, 0x9C, 0x34, 0xF9, 0x17, 0x9F, 0xDA,
    0x87, 0xF8, 0x15, 0x05, 0x3C, 0xD3, 0xA4, 0x85, 0x2E, 0xFB, 0xEE, 0x47, 0x3B, 0xEF, 0x37, 0x7F,
    0x93, 0xAF, 0x69, 0x0C, 0x71, 0x31, 0xDE, 0x21, 0x75, 0xA0, 0xAA, 0xBA, 0x7C, 0x38, 0x02, 0xB7,
    0x81, 0x01, 0xFD, 0xE7, 0x1D, 0xCC, 0xCD, 0xBD, 0x1B, 0x7A, 0x2A, 0xAD, 0x66, 0xBE, 0x55, 0x33,
    0x03, 0xDB, 0x88, 0xB2, 0x1E, 0x4E, 0xB9, 0xE6, 0xC2, 0xF7, 0xCB, 0x7D, 0xC9, 0x62, 0xC3, 0xA6,
    0xDC, 0xA7, 0x50, 0xB5, 0x4B, 0x94, 0xC0, 0x92, 0x4C, 0x11, 0x5B, 0x78, 0xD9, 0xB1, 0xED, 0x19,
    0xE9, 0xA1, 0x1C, 0xB6, 0x32, 0x99, 0xA3, 0x76, 0x9E, 0x7B, 0x6D, 0x9A, 0x30, 0xD6, 0xA9, 0x25,
    0xC7, 0xAE, 0x96, 0x35, 0xD0, 0xBB, 0xD2, 0xC8, 0xA2, 0x08, 0xF3, 0xD1, 0x73, 0xF4, 0x48, 0x2D,
    0x90, 0xCA, 0xE2, 0x58, 0xC1, 0x18, 0x52, 0xFE, 0xDF, 0x68, 0x98, 0x54, 0xEC, 0x60, 0x43, 0x0F
};

//接收字节映射表
const static BYTE RecvByteMap[256] =
{
    0x51, 0xA1, 0x9E, 0xB0, 0x1E, 0x83, 0x1C, 0x2D, 0xE9, 0x77, 0x3D, 0x13, 0x93, 0x10, 0x45, 0xFF,
    0x6D, 0xC9, 0x20, 0x2F, 0x1B, 0x82, 0x1A, 0x7D, 0xF5, 0xCF, 0x52, 0xA8, 0xD2, 0xA4, 0xB4, 0x0B,
    0x31, 0x97, 0x57, 0x19, 0x34, 0xDF, 0x5B, 0x41, 0x58, 0x49, 0xAA, 0x5F, 0x0A, 0xEF, 0x88, 0x01,
    0xDC, 0x95, 0xD4, 0xAF, 0x7B, 0xE3, 0x11, 0x8E, 0x9D, 0x16, 0x61, 0x8C, 0x84, 0x3C, 0x1F, 0x5A,
    0x02, 0x4F, 0x39, 0xFE, 0x04, 0x07, 0x5C, 0x8B, 0xEE, 0x66, 0x33, 0xC4, 0xC8, 0x59, 0xB5, 0x5D,
    0xC2, 0x6C, 0xF6, 0x4D, 0xFB, 0xAE, 0x4A, 0x4B, 0xF3, 0x35, 0x2C, 0xCA, 0x21, 0x78, 0x3B, 0x03,
    0xFD, 0x24, 0xBD, 0x25, 0x37, 0x29, 0xAC, 0x4E, 0xF9, 0x92, 0x3A, 0x32, 0x4C, 0xDA, 0x06, 0x5E,
    0x00, 0x94, 0x60, 0xEC, 0x17, 0x98, 0xD7, 0x3E, 0xCB, 0x6A, 0xA9, 0xD9, 0x9C, 0xBB, 0x08, 0x8F,
    0x40, 0xA0, 0x6F, 0x55, 0x67, 0x87, 0x54, 0x80, 0xB2, 0x36, 0x47, 0x22, 0x44, 0x63, 0x05, 0x6B,
    0xF0, 0x0F, 0xC7, 0x90, 0xC5, 0x65, 0xE2, 0x64, 0xFA, 0xD5, 0xDB, 0x12, 0x7A, 0x0E, 0xD8, 0x7E,
    0x99, 0xD1, 0xE8, 0xD6, 0x86, 0x27, 0xBF, 0xC1, 0x6E, 0xDE, 0x9A, 0x09, 0x0D, 0xAB, 0xE1, 0x91,
    0x56, 0xCD, 0xB3, 0x76, 0x0C, 0xC3, 0xD3, 0x9F, 0x42, 0xB6, 0x9B, 0xE5, 0x23, 0xA7, 0xAD, 0x18,
    0xC6, 0xF4, 0xB8, 0xBE, 0x15, 0x43, 0x70, 0xE0, 0xE7, 0xBC, 0xF1, 0xBA, 0xA5, 0xA6, 0x53, 0x75,
    0xE4, 0xEB, 0xE6, 0x85, 0x14, 0x48, 0xDD, 0x38, 0x2A, 0xCC, 0x7F, 0xB1, 0xC0, 0x71, 0x96, 0xF8,
    0x3F, 0x28, 0xF2, 0x69, 0x74, 0x68, 0xB7, 0xA3, 0x50, 0xD0, 0x79, 0x1D, 0xFC, 0xCE, 0x8A, 0x8D,
    0x2E, 0x62, 0x30, 0xEA, 0xED, 0x2B, 0x26, 0xB9, 0x81, 0x7C, 0x46, 0x89, 0x73, 0xA2, 0xF7, 0x72
};

template <int _buffer_size, int _header_size>
class PacketBase
{
public:
    PacketBase():m_rtail(false),m_wrerr(0)
    {
        is_extern = false;
        m_capacity = PACKET_BUFFER_SIZE;
        m_strBuf = (char*)malloc(m_capacity);
    }
    PacketBase(void* ext, int len)
    {
        m_capacity = len;
        _reset();
        is_extern = true;
        m_strBuf = (char*)ext;
        m_nPacketSize = len;
    }
    virtual ~PacketBase()
    {
        if (is_extern)
            m_strBuf = NULL;
        free(m_strBuf);
    }
    char* return_buf()
    {
        char* temp = m_strBuf;
        m_strBuf = NULL;
        m_nPacketSize = 0;
        return temp;
    }
    bool is_extern;
    char *packet_buf(void)    {return m_strBuf;}
    int packet_size(void)    {return m_nPacketSize;}
    enum
    {
        PACKET_HEADER_SIZE = _header_size,
        PACKET_BUFFER_SIZE = _buffer_size
    };
    
    bool IsWriteSequence(void)
    {
        return _header_size == 13 ;
    }
private:
    char* m_strBuf;    // 报文包缓存
    int m_nPacketSize ;    // 实际报文总长度
    int m_nBufPos;
    int m_capacity; //报文容量

    bool m_rtail;  //是否读到队尾了
    int  m_wrerr;  //发生读写错误的位置,暂时只有读
protected:
    ////////////////////////////////////////////////////////////////////////////////
    bool _copy(const void *pInBuf, int nLen)
    {
        if (nLen > m_capacity)
            return false;

        _reset();
        memcpy(m_strBuf, pInBuf, nLen);
        m_nPacketSize = nLen;
        //assert(m_nPacketSize>PACKET_HEADER_SIZE);
        return true;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void _begin(short nCmdType, char cVersion, char cSubVersion)
    {
        _reset();
        _writeHeader("IC", sizeof(char)*2, 0);// 命令码
        _writeHeader((char*)&nCmdType, sizeof(short), 2);// 命令码
        _writeHeader(&cVersion, sizeof(char), 4);        // 主版本号
        _writeHeader(&cSubVersion, sizeof(char), 5);    // 子版本号
    }
    void _SetBegin(short nCmdType)
    {
        _writeHeader((char*)&nCmdType, sizeof(short), 2);// 命令码
    }
public:
    short GetCmdType(void)
    {
        short nCmdType;
        _readHeader((char*)&nCmdType, sizeof(short), 2);// 命令码
        return nCmdType;
    }
    char GetVersion(void)
    {
        char c;
        _readHeader(&c, sizeof(char), 4);    // 主版本号
        return c;
    }
    char GetSubVersion(void)
    {
        char c;
        _readHeader(&c, sizeof(char), 5);    // 子版本号
        return c;
    }
    short GetBodyLength(void)
    {
        short nLen;
        _readHeader((char*)&nLen, sizeof(short), 6);// 包正文长度
        return nLen;
    }
    BYTE GetcbCheckCode(void) //编码
    {
        BYTE code;
        _readHeader((char*)&code, sizeof(BYTE), 8);// 包正文长度
        return code;
    }
    int GetSequence(void) //序列
    {
        if(!IsWriteSequence())
        {
            return 0;
        }
        int sequence;
        _readHeader((char*)&sequence, sizeof(int), 9);// 包正文长度
        return sequence;
    }
protected:
    const bool is_empty()
    {
        return m_nBufPos >= m_nPacketSize;
    }
    const bool _rtail(void)
    {
        return m_rtail;
    }
    const int _wrerrpos(void)
    {
        return m_wrerr;
    }
    void _end(int sequence = 0)
    {
        short nBody = m_nPacketSize - PACKET_HEADER_SIZE;
        _writeHeader((char*)&nBody, sizeof(short), 6);    // 包正文长度
        BYTE code = 0;
        _writeHeader((char*)&code, sizeof(BYTE), 8); //效验码
        if(this->IsWriteSequence())
        {
            _writeHeader((char*)&sequence, sizeof(int), 9); //序列
        }
    }
    void _oldend()
    {
        short nBody = m_nPacketSize - PACKET_HEADER_SIZE;
        _writeHeader((char*)&nBody, sizeof(short), 6);    // 包正文长度
    }
    /////////////////////////////////////////////////////////////////////////////////
    void _reset(void)
    {
        if (m_strBuf != NULL)
            memset(m_strBuf, 0, m_capacity);
        m_nBufPos = PACKET_HEADER_SIZE;
        m_nPacketSize = PACKET_HEADER_SIZE;
        m_rtail = false;
        m_wrerr = 0;
    }
    // 取出一个变量
    bool _Read(char *pOut, int nLen)
    {
        if ((nLen + m_nBufPos) > m_nPacketSize || (nLen + m_nBufPos) > m_capacity){
            m_wrerr = m_nBufPos;
            return false ;
        }
        memcpy(pOut, m_strBuf + m_nBufPos, nLen);
        m_nBufPos += nLen;
        if(m_nBufPos == m_nPacketSize)
            m_rtail = true;
        return true;
    }
    //取出变量并从包中移除
    bool _ReadDel(char *pOut, int nLen)
    {
        if(!_Read(pOut, nLen))
            return false;
        memcpy(m_strBuf + m_nBufPos - nLen, m_strBuf + m_nBufPos, m_capacity - m_nBufPos);
        m_nBufPos -= nLen;
        m_nPacketSize -= nLen;
        _end();
        return true;
    }
    //读撤消
    void _readundo(int nLen)
    {
        m_nBufPos -= nLen;
    }
    //读出当前POS位置的BUFFER指针
    char *_readpoint(int nLen) //注意返回的是指针 请慎重使用string
    {
        if((nLen + m_nBufPos) > m_nPacketSize || nLen<=0)
            return NULL; 
        char *p = &m_strBuf[m_nBufPos];
        m_nBufPos += nLen;
        return p;

    }
    // 写入一个变量
    bool _Write(const char *pIn, int nLen)
    {
        if ((m_nPacketSize < 0) || ((nLen + m_nPacketSize) > m_capacity))
            return false ;
        memcpy(m_strBuf+m_nPacketSize, pIn, nLen);
        m_nPacketSize += nLen;
        return true;
    }
    //插入一个变量
    bool _Insert(const char *pIn, int nLen)
    {
        if ((nLen + m_nPacketSize) > m_capacity)
            return false;
        memcpy(m_strBuf+PACKET_HEADER_SIZE+nLen, m_strBuf+PACKET_HEADER_SIZE, m_nPacketSize-PACKET_HEADER_SIZE);
        memcpy(m_strBuf+PACKET_HEADER_SIZE, pIn, nLen);
        m_nPacketSize += nLen;
        _end();
        return true;
    }
    // 写入一个变量
    bool _writezero(void)
    {
        if ((m_nPacketSize + 1) > m_capacity)
            return false ;
        memset(m_strBuf+m_nPacketSize, '\0', sizeof(char)) ;
        m_nPacketSize ++;
        return true;
    }
    // readHeader
    void _readHeader(char *pOut, int nLen, int nPos)
    {
        if(nPos > 0 || nPos+nLen < PACKET_HEADER_SIZE)
        {
            memcpy(pOut, m_strBuf+nPos, nLen) ;
        }
    }
    // writeHeader
    void _writeHeader(const char *pIn, int nLen, int nPos)
    {
        if(nPos > 0 || nPos+nLen < PACKET_HEADER_SIZE)
        {
            memcpy(m_strBuf+nPos, pIn, nLen) ;
        }
    }
    
};

template <int BUFFER_SIZE, int HEADER_SIZE=13>
class InputPacket: public PacketBase<BUFFER_SIZE, HEADER_SIZE>
{
public:
    typedef PacketBase<BUFFER_SIZE, HEADER_SIZE> base;
    InputPacket(void* ext, int len) :base(ext, len){}
    InputPacket(){}
    int ReadInt(void)        {int nValue = -1; base::_Read((char*)&nValue, sizeof(int)); return nValue;} //这里必需初始化
    unsigned long ReadULong(void) {unsigned long nValue = -1; base::_Read((char*)&nValue, sizeof(unsigned long)); return nValue;}
    int ReadIntDel(void)    {int nValue = -1; base::_ReadDel((char*)&nValue, sizeof(int)); return nValue;} 
    short ReadShort(void)    {short nValue = -1; base::_Read((char*)&nValue, sizeof(short)); return nValue;}
    BYTE ReadByte(void)        {BYTE nValue = -1; base::_Read((char*)&nValue, sizeof(BYTE)); return nValue;}
    int64_t ReadInt64(void) {int64_t nValue = -1; base::_Read((char*)&nValue, sizeof(int64_t)); return nValue;}
    float ReadFloat(void)    {float nValue = -1; base::_Read((char*)&nValue, sizeof(float)); return nValue;}
    double ReadFloat64(void) {double nValue = -1; base::_Read((char*)&nValue, sizeof(double)); return nValue;} //add by jim
    bool IsEmpty()
    {
        return base::is_empty();
    }

    bool ReadString(char *pOutString, int nMaxLen)
    {
        int nLen = ReadInt();
        if(nLen == -1)  //这里必需判断
            return false;
        if(nLen > nMaxLen)
        {
            base::_readundo(sizeof(short));
            return false;
        }
        return base::_Read(pOutString, nLen);
    }

    char *ReadChar(void)
    {
        int nLen = ReadInt();
        if(nLen == -1) 
            return NULL;
        return base::_readpoint(nLen);
    }

    string ReadString(void)
    {
        char *p = ReadChar();
        return (p == NULL ? "" : p);
    }

    int ReadBinary(char *pBuf, int nMaxLen)
    {
        int nLen = ReadInt();
        if(nLen == -1) 
            return -1;
        if(nLen > nMaxLen)
        {
            base::_readundo(sizeof(int));
            return -1;
        }
        if(base::_Read(pBuf, nLen))
            return nLen ;
        return 0;
    }
    void Reset(void)
    {
        base::_reset();
    }
    bool Copy(const void *pInBuf, int nLen)
    {
        return base::_copy(pInBuf, nLen);
    }
    bool WriteBody(const char *pIn, int nLen)
    {
        return base::_Write(pIn, nLen);
    }
    ////用于伪造接收包
    void Begin(short nCommand, char cVersion = SERVER_PACEKTVER, char cSubVersion = SERVER_SUBPACKETVER)
    {
        base::_begin(nCommand, cVersion, cSubVersion);
    }
    void End(void)
    {
        base::_end();
    }
    const bool Rtail(void)
    {
        return base::_rtail();
    }
    const int WRErrPos(void)
    {
        return base::_wrerrpos();
    }
    int DecryptBuffer()
    {
        BYTE* buff = (BYTE*)base::packet_buf() + HEADER_SIZE;
        short wDataSize = base::GetBodyLength();
        BYTE cbCheckCode = base::GetcbCheckCode();
        for (short i = 0; i < wDataSize; i++)
        {
            buff[i] = RecvByteMap[buff[i]];
            cbCheckCode += buff[i];
        }
        if (cbCheckCode != 0)
            return -1;
        return wDataSize;
    }
};

template <int BUFFER_SIZE, int HEADER_SIZE=13>
class OutputPacket: public PacketBase<BUFFER_SIZE, HEADER_SIZE>
{
    bool m_isCheckCode;
public:
    OutputPacket(void){m_isCheckCode = false;}
public:
    typedef PacketBase<BUFFER_SIZE, HEADER_SIZE> base;
    bool WriteInt(int nValue)        {return base::_Write((char*)&nValue, sizeof(int));}
    bool WriteInt64(int64_t nValue)        {return base::_Write((char*)&nValue, sizeof(int64_t));}
    bool WriteULong(unsigned long nValue) {return base::_Write((char*)&nValue, sizeof(unsigned long));}
    bool WriteByte(BYTE nValue)        {return base::_Write((char*)&nValue, sizeof(BYTE));}
    bool WriteShort(short nValue)    {return base::_Write((char*)&nValue, sizeof(short));}
    bool WriteFloat(float nValue)    {return base::_Write((char*)&nValue, sizeof(float));}
    bool WriteFloat64(double nValue)    {return base::_Write((char*)&nValue, sizeof(double));} //add by jim
    //在正文首插入数据
    bool InsertInt(int nValue)        {return base::_Insert((char*)&nValue, sizeof(int));}
    bool InsertByte(BYTE nValue)    {return base::_Insert((char*)&nValue, sizeof(BYTE));}
    bool WriteString(const char *pString)
    {
        int nLen = (int)strlen(pString) ;
        WriteInt(nLen + 1) ;
        return base::_Write(pString, nLen) && base::_writezero();
    }

    bool WriteString(const string &strDate)
    {
        int nLen = strDate.size();
        WriteInt(nLen + 1) ;
        return base::_Write(strDate.c_str(), nLen) && base::_writezero();
    }

    bool WriteBinary(const char *pBuf, int nLen)
    {
        WriteInt(nLen) ;
        return base::_Write(pBuf, nLen) ;
    }
    void Reset(void)
    {
        base::_reset();
    }
    bool Copy(const void *pInBuf, int nLen)
    {
        return base::_copy(pInBuf, nLen);
    }
    void Begin(short nCommand, char cVersion = SERVER_PACEKTVER_NEW, char cSubVersion = SERVER_SUBPACKETVER2)
    {
        base::_begin(nCommand, cVersion, cSubVersion);
        m_isCheckCode = false;
    }
    void End(void)
    {
        m_isCheckCode = false;
        base::_end();
    }
    void oldEnd(void)
    {
        m_isCheckCode = false;
        base::_oldend();
    }
    //增加
    void SetBegin(short nCommand)
    {
        base::_SetBegin(nCommand);
    }
    //效验码
    void WritecbCheckCode(BYTE nValue)
    {
        base::_writeHeader((char*)&nValue, sizeof(BYTE), 8); //效验码
        m_isCheckCode = true;
    }

    bool IsWritecbCheckCode(void)
    {
        return m_isCheckCode;
    }

    //效验码
    void WriteSequence(int nValue)
    {
        base::_writeHeader((char*)&nValue, sizeof(int), 9); //序列
    }
    void EncryptBuffer()
    {
        BYTE* buff = (BYTE*)base::packet_buf() + HEADER_SIZE;
        short wDataSize = base::GetBodyLength();
        BYTE cbCheckCode = 0;
        for (short i = 0; i<wDataSize; i++)
        {
            cbCheckCode += buff[i];
            buff[i] = SendByteMap[buff[i]];
        }
        WritecbCheckCode(~cbCheckCode + 1);
    }
};



#define    ICHAT_TCP_MAX_BUFFER    1024*16

template class InputPacket<ICHAT_TCP_MAX_BUFFER>;
template class OutputPacket<ICHAT_TCP_MAX_BUFFER>;

typedef InputPacket<ICHAT_TCP_MAX_BUFFER>    NETInputPacket;
typedef OutputPacket<ICHAT_TCP_MAX_BUFFER>    NETOutputPacket;




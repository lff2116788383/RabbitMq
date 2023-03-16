

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#elif __linux__
#include <unistd.h>
#elif __APPLE__
#endif

#include <iostream>
#include "ICHAT_PacketBase.h"


//worker
#if 1
#include "RabbitmqClient.h"


struct NowDate
{
    char tmp0[16]; //年月日
    char tmp1[16]; //时分秒
    char tmp2[32]; //年月日时分秒
};

NowDate getTime()
{
    time_t timep;
    time(&timep);
    NowDate date;

    strftime(date.tmp0, sizeof(date.tmp0), "%Y_%m_%d", localtime(&timep));
    strftime(date.tmp1, sizeof(date.tmp1), "%H_%M_%S", localtime(&timep));
    strftime(date.tmp2, sizeof(date.tmp2), "%Y/%m/%d %H:%M:%S", localtime(&timep));

    return date;

}

//消费者处理
int main()
{


    std::string strIP = "localhost";
    int iPort = 5672;
    std::string strUser = "guest";
    std::string strPasswd = "guest";
    std::string err;

  /*  std::string strExchange = "ExchangeLff";
    std::string strRoutekey = "routekeyLff";
    std::string strQueuename = "queueLff";*/

    std::string strExchange = "Exchange6_1";
    std::string strRoutekey = "routekey6_1";
    std::string strQueuename = "queue6_1";

    std::string msg;

    CRabbitmqClient work;

    int iRet = work.Connect(strIP, iPort, strUser, strPasswd);
    if (iRet<0)
    {
        printf("Rabbitmq Connect iRet: %d\n", iRet);
        return 0;
    }
    
    /*CExchange exchange(strExchange, "direct", true);
    iRet = work.ExchangeDeclare(exchange);
    if (iRet < 0)
    {
        printf("Rabbitmq ExchangeDeclare  iRet: %d\n", iRet);
        return 0;
    }*/

    CQueue que(strQueuename, // name
        true,         // durable
        false,        // delete when unused
        false,        // exclusive
        false       // no-wait
                );
   iRet = work.QueueDeclare(que);
    if (iRet < 0)
    {
        printf("Rabbitmq DeclareQueue iRet: %d\n", iRet);
        return 0;
    }
    
  /*  iRet = work.QueueBind(que, exchange, strRoutekey);
    if (iRet < 0)
    {
        printf("Rabbitmq Connect iRet: %d\n", iRet);
        return 0;
    }*/


    while (true) {
        iRet = work.ConsumerMessage(que.m_name, msg);
        if (iRet < 0)
        {
            printf("Rabbitmq ConsumerMessage iRet: %d\n", iRet);
            return 0;
        }
        NowDate current_time = getTime();

        printf("%s  Rabbitmq ConsumerMessage msg: %s\n", current_time.tmp2,msg.c_str());

        NETInputPacket pack;
        pack.Copy(msg.c_str(), msg.size());

        if (pack.DecryptBuffer() == -1)
        {
            std::cout << "NETInputPacket DecryptBuffer error" << std::endl;
            return 0;
        }

        short cmd = pack.GetCmdType();

        std::cout << "Recv NETInputPacket cmd:0x" << hex << cmd << " packet size:" << pack.packet_size() << std::endl;

        Sleep(5000);
    }

    iRet = work.Disconnect();
    if (iRet < 0)
    {
        printf("Rabbitmq DisConnect iRet: %d\n", iRet);
        return 0;
    }
}
#endif // 1



//task
#if 1
#include "RabbitmqClient.h"
#include <time.h>
#include "ICHAT_PacketBase.h"

#ifdef _WIN32
#elif __linux__
#include <unistd.h>
#elif __APPLE__
#endif

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

string GetSendPacket(NETOutputPacket& boyaa_packet)
{
    //skynet_error(NULL, "send pack 0x%x", boyaa_packet.GetCmdType());
    boyaa_packet.EncryptBuffer();
    int size = boyaa_packet.packet_size();
    //CBoyaaRobotAction::send(boyaa_packet.packet_buf(), size);

    string msg = boyaa_packet.return_buf();

    return msg;
}

//消费者处理
int main()
{
#ifdef _WIN32
    std::string strIP = "localhost";
#elif __linux__
    std::string strIP = "172.20.103.13";
#elif __APPLE__
#endif


    int iPort = 5672;
    std::string strUser = "guest";
    std::string strPasswd = "guest";
    std::string err;

    std::string strExchange = "Exchange6_1";
    std::string strRoutekey = "routekey6_1";
    std::string strQueuename = "queue6_1";

    std::string msg;

    CRabbitmqClient task;

    int iRet = task.Connect(strIP, iPort, strUser, strPasswd);
    if (iRet < 0)
    {
        printf("Rabbitmq Connect  iRet: %d\n", iRet);
        return 0;
    }
    
    CExchange exchange(strExchange, "direct",true);

    iRet = task.ExchangeDeclare(exchange);
    if (iRet < 0)
    {
        printf("Rabbitmq ExchangeDeclare  iRet: %d\n", iRet);
        return 0;
    }

    CQueue que(strQueuename, // name
        true,         // durable
        false,        // delete when unused
        false,        // exclusive
        false       // no-wait
    );
    iRet = task.QueueDeclare(que);
    if (iRet < 0)
    {
        printf("Rabbitmq Connect  iRet: %d\n", iRet);
        return 0;
    }

    iRet = task.QueueBind(que, exchange, strRoutekey);
    if (iRet < 0)
    {
        printf("Rabbitmq Connect iRet: %d\n", iRet);
        return 0;
    }


    int cmd = 0x100;
    while (true) {

        cmd++;
        NETOutputPacket packet;
        packet.Begin(cmd);
        packet.End();
        string sendMsg = GetSendPacket(packet);


        //消息属性的设置
        amqp_basic_properties_t props;
        props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
        props.content_type = amqp_cstring_bytes("text/plain");
        props.delivery_mode = 2; // persistent delivery mode  消息持久化，在投递时指定delivery_mode=> 2（1是非持久化）


        CMessage msg(sendMsg, props, strRoutekey);
        iRet = task.PublishMessage(msg, exchange);

        if (iRet < 0)
        {
            printf("Rabbitmq Connect iRet: %d\n", iRet);
            return 0;
        }
        NowDate current_time = getTime();

        printf("%s  Rabbitmq PublishMessage msg: %s\n", current_time.tmp2, msg.m_data.c_str());


//#ifdef _WIN32
//        Sleep(5000);
//#elif __linux__
//        usleep(5000000);
//#elif __APPLE__
//#endif

    }

    iRet = task.Disconnect();
    if (iRet < 0)
    {
        printf("Rabbitmq DisConnect  iRet: %d\n", iRet);
        return 0;
    }
}
#endif // 1

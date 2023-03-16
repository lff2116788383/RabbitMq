#include "RabbitmqClient.h"


#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#elif __APPLE__
#endif


CRabbitmqClient::CRabbitmqClient()
    : m_strHostname("")
    , m_iPort(0)
    , m_strUser("")
    , m_strPasswd("")
    , m_iChannel(1) //默认用1号通道，通道无所谓 
    , m_pSock(NULL)
    , m_pConn(NULL) {

}

CRabbitmqClient::~CRabbitmqClient() {
    if (NULL != m_pConn) {
        Disconnect();
        m_pConn = NULL;
    }
}

int CRabbitmqClient::Connect(const string& strHostname, int iPort, const string& strUser, const string& strPasswd) {
    m_strHostname = strHostname;
    m_iPort = iPort;
    m_strUser = strUser;
    m_strPasswd = strPasswd;

    m_pConn = amqp_new_connection();
    if (NULL == m_pConn) {
        fprintf(stderr, "amqp new connection failed\n");
        return -1;
    }

    m_pSock = amqp_tcp_socket_new(m_pConn);
    if (NULL == m_pSock) {
        fprintf(stderr, "amqp tcp new socket failed\n");
        return -2;
    }

    int status = amqp_socket_open(m_pSock, m_strHostname.c_str(), m_iPort);
    if (status < 0) {
        fprintf(stderr, "amqp socket open failed\n");
        return -3;
    }

    // amqp_login(amqp_connection_state_t state,char const *vhost, int channel_max, int frame_max, int heartbeat, amqp_sasl_method_enum sasl_method, ..)
    if (0 != ErrorMsg(amqp_login(m_pConn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, m_strUser.c_str(), m_strPasswd.c_str()), "Logging in")) {
        return -4;
    }

    return 0;
}

int CRabbitmqClient::Disconnect() {
    if (NULL != m_pConn) {
        if (0 != ErrorMsg(amqp_connection_close(m_pConn, AMQP_REPLY_SUCCESS), "Closing connection"))
            return -1;

        if (amqp_destroy_connection(m_pConn) < 0)
            return -2;

        m_pConn = NULL;
    }

    return 0;
}

int CRabbitmqClient::ExchangeDeclare(const string& strExchange, const string& strType, int _durable) {
    amqp_channel_open(m_pConn, m_iChannel);

    amqp_bytes_t _exchange = amqp_cstring_bytes(strExchange.c_str());
    amqp_bytes_t _type = amqp_cstring_bytes(strType.c_str());
    int _passive = 0;
    //int _durable = 0;      // 交换机是否持久化
    amqp_exchange_declare(m_pConn, m_iChannel, _exchange, _type, _passive, _durable, 0, 0, amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "exchange_declare")) {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -1;
    }

    amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
    return 0;
}

//重载
int CRabbitmqClient::ExchangeDeclare(CExchange& exchange)
{
    amqp_channel_open(m_pConn, m_iChannel);

   
    amqp_exchange_declare(m_pConn, 
        m_iChannel, 
        amqp_cstring_bytes(exchange.m_name.c_str()),
        amqp_cstring_bytes(exchange.m_type.c_str()),
        exchange.m_bPassive,
        exchange.m_bDurable,
        exchange.m_bAutoDelete,
        exchange.m_bInternal,
        amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "exchange_declare")) {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -1;
    }

    amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
    return 0;

}

int CRabbitmqClient::QueueDeclare(const string& strQueueName, int32_t _durable, int32_t _auto_delete) {
    if (NULL == m_pConn) {
        fprintf(stderr, "QueueDeclare m_pConn is null\n");
        return -1;
    }

    amqp_channel_open(m_pConn, m_iChannel);
    amqp_bytes_t _queue = amqp_cstring_bytes(strQueueName.c_str());
    int32_t _passive = 0;
    //int32_t _durable = 0;
    int32_t _exclusive = 0;
    //int32_t _auto_delete = 1;
    amqp_queue_declare(m_pConn, m_iChannel, _queue, _passive, _durable, _exclusive, _auto_delete, amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "queue_declare")) {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -1;
    }

    amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
    return 0;
}

int CRabbitmqClient::QueueDeclare(CQueue& queue)
{
    if (NULL == m_pConn) {
        fprintf(stderr, "QueueDeclare m_pConn is null\n");
        return -1;
    }

    amqp_channel_open(m_pConn, m_iChannel);
 
    amqp_queue_declare(m_pConn, 
        m_iChannel, 
        amqp_cstring_bytes(queue.m_name.c_str()),
        queue.m_bPassive,	/*passive*/ 	//检验队列是否存在（同exchange中的passive属性）
        queue.m_durable,	/*durable*/ 	//队列是否持久化(即使mq服务重启也会存在)
        queue.m_bExclusive,  /*exclusive*/ 	//是否是专用队列（当前连接不在时，队列是否删除）
        queue.m_bAutoDelete,/*auto_delete*/  //是否自动删除（什么时候删除？。。。）
        amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "queue_declare")) {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -1;
    }

    amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
    return 0;
}

int CRabbitmqClient::QueueBind(const string& strQueueName, const string& strExchange, const string& strBindKey) {
    if (NULL == m_pConn) {
        fprintf(stderr, "QueueBind m_pConn is null\n");
        return -1;
    }

    amqp_channel_open(m_pConn, m_iChannel);
    amqp_bytes_t _queue = amqp_cstring_bytes(strQueueName.c_str());
    amqp_bytes_t _exchange = amqp_cstring_bytes(strExchange.c_str());
    amqp_bytes_t _routkey = amqp_cstring_bytes(strBindKey.c_str());
    amqp_queue_bind(m_pConn, m_iChannel, _queue, _exchange, _routkey, amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "queue_bind")) {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -1;
    }

    amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
    return 0;
}


int CRabbitmqClient::QueueBind(CQueue& queue, CExchange& exchange, const std::string bind_key)
{
    if (NULL == m_pConn) {
        fprintf(stderr, "QueueBind m_pConn is null\n");
        return -1;
    }

    amqp_channel_open(m_pConn, m_iChannel);

    amqp_queue_bind(m_pConn,
        m_iChannel,
        amqp_cstring_bytes(queue.m_name.c_str()),	//队列
        amqp_cstring_bytes(exchange.m_name.c_str()),	//交换机
        amqp_cstring_bytes(bind_key.c_str()), //路由
        amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "queue_bind")) {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -1;
    }

    amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
    return 0;
}


int CRabbitmqClient::QueueUnbind(const string& strQueueName, const string& strExchange, const string& strBindKey) {
    if (NULL == m_pConn) {
        fprintf(stderr, "QueueUnbind m_pConn is null\n");
        return -1;
    }

    amqp_channel_open(m_pConn, m_iChannel);
    amqp_bytes_t _queue = amqp_cstring_bytes(strQueueName.c_str());
    amqp_bytes_t _exchange = amqp_cstring_bytes(strExchange.c_str());
    amqp_bytes_t _routkey = amqp_cstring_bytes(strBindKey.c_str());
    amqp_queue_unbind(m_pConn, m_iChannel, _queue, _exchange, _routkey, amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "queue_unbind")) {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -1;
    }

    amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
    return 0;
}

int CRabbitmqClient::QueueDelete(const string& strQueueName, int iIfUnused) {
    if (NULL == m_pConn) {
        fprintf(stderr, "QueueDelete m_pConn is null\n");
        return -1;
    }

    amqp_channel_open(m_pConn, m_iChannel);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "open channel")) {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -2;
    }

    amqp_queue_delete(m_pConn, m_iChannel, amqp_cstring_bytes(strQueueName.c_str()), iIfUnused, 0);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "delete queue")) {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -3;
    }

    amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
    return 0;
}

int CRabbitmqClient::Publish(const string& strMessage, const string& strExchange, const string& strRoutekey) {
    if (NULL == m_pConn) {
        fprintf(stderr, "publish m_pConn is null, publish failed\n");
        return -1;
    }

    amqp_channel_open(m_pConn, m_iChannel);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "open channel")) {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -2;
    }

    amqp_bytes_t message_bytes;
    message_bytes.len = strMessage.length();
    message_bytes.bytes = (void*)(strMessage.c_str());
    //fprintf(stderr, "publish message(%d): %.*s\n", (int)message_bytes.len, (int)message_bytes.len, (char *)message_bytes.bytes);

    /*
    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes(m_type.c_str());
    props.delivery_mode = m_durable;    // persistent delivery mode
    */

    amqp_bytes_t exchange = amqp_cstring_bytes(strExchange.c_str());
    amqp_bytes_t routekey = amqp_cstring_bytes(strRoutekey.c_str());

    //if (0 != amqp_basic_publish(m_pConn, m_iChannel, exchange, routekey, 0, 0, &props, message_bytes)) {
    if (0 != amqp_basic_publish(m_pConn, m_iChannel, exchange, routekey, 0, 0, NULL, message_bytes)) {
        fprintf(stderr, "publish amqp_basic_publish failed\n");
        if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "amqp_basic_publish")) {
            amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
            return -3;
        }
    }

    amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
    return 0;
}


int CRabbitmqClient::PublishMessage(const CMessage& message, CExchange& exchange)
{

    if (NULL == m_pConn) {
        fprintf(stderr, "publish m_pConn is null, publish failed\n");
        return -1;
    }

    amqp_channel_open(m_pConn, m_iChannel);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "open channel")) {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -2;
    }

    amqp_bytes_t message_bytes;
    message_bytes.len = message.m_data.length();
    message_bytes.bytes = (void*)(message.m_data.c_str());
    //fprintf(stderr, "publish message(%d): %.*s\n", (int)message_bytes.len, (int)message_bytes.len, (char *)message_bytes.bytes);

    /*
    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes(m_type.c_str());
    props.delivery_mode = m_durable;    // persistent delivery mode
    */

    amqp_bytes_t exchange_name = amqp_cstring_bytes(exchange.m_name.c_str());
    amqp_bytes_t routekey__keys_name = amqp_cstring_bytes(message.m_routekey.c_str());

    //if (0 != amqp_basic_publish(m_pConn, m_iChannel, exchange, routekey, 0, 0, &props, message_bytes)) {
    if (0 != amqp_basic_publish(m_pConn,
        m_iChannel,
        exchange_name, routekey__keys_name, message.m_bMandatory, message.m_bImmediate, &message.m_properties, message_bytes)) {
        fprintf(stderr, "publish amqp_basic_publish failed\n");
        if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "amqp_basic_publish")) {
            amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
            return -3;
        }
    }

    amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
    return 0;
}


int CRabbitmqClient::PublishMessage(std::vector<CMessage>& messageVec, CExchange& exchange)
{
    int ret = 0;

    for (std::vector<CMessage>::iterator it = messageVec.begin(); it != messageVec.end(); ++it)
    {
        //发送多个消息时，暂时简化为只看发送最后一条消息的结果
        ret = PublishMessage(*it,exchange);
    }

    return ret;
}

int CRabbitmqClient::Consumer(const string& strQueueName, vector<string>& message_array, int GetNum, struct timeval* timeout) {
    if (NULL == m_pConn) {
        fprintf(stderr, "Consumer m_pConn is null, Consumer failed\n");
        return -1;
    }

    amqp_channel_open(m_pConn, m_iChannel);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "open channel")) {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -2;
    }

    amqp_basic_qos(m_pConn, m_iChannel, 0, GetNum, 0);
    int ack = 1; // no_ack    是否需要确认消息后再从队列中删除消息
    amqp_bytes_t queuename = amqp_cstring_bytes(strQueueName.c_str());
    amqp_basic_consume(m_pConn, m_iChannel, queuename, amqp_empty_bytes, 0, ack, 0, amqp_empty_table);

    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "Consuming")) {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -3;
    }

    int hasget = 0;
    amqp_rpc_reply_t res;
    amqp_envelope_t envelope;
    while (GetNum > 0) {
        amqp_maybe_release_buffers(m_pConn);
        res = amqp_consume_message(m_pConn, &envelope, timeout, 0);
        if (AMQP_RESPONSE_NORMAL != res.reply_type) {
            fprintf(stderr, "Consumer amqp_channel_close failed\n");
            amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);

            if (0 == hasget)
                return -res.reply_type;
            else
                return 0;
        }

        string str((char*)envelope.message.body.bytes, (char*)envelope.message.body.bytes + envelope.message.body.len);
        message_array.push_back(str);
        int rtn = amqp_basic_ack(m_pConn, m_iChannel, envelope.delivery_tag, 1);
        amqp_destroy_envelope(&envelope);
        if (rtn != 0) {
            amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
            return -4;
        }

        GetNum--;
        hasget++;


#ifdef _WIN32
        Sleep(1);
#elif __linux__
        usleep(1);
#elif __APPLE__

#endif


    }

    return 0;
}


int CRabbitmqClient::ConsumerMessage(const std::string& queue_name_, std::string& message, timeval* timeout, bool no_ack)
{
    if (NULL == m_pConn) {
        fprintf(stderr, "Consumer m_pConn is null, Consumer failed\n");
        return -1;
    }

    amqp_channel_open(m_pConn, m_iChannel);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "open channel")) {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -2;
    }

    //step 1
    amqp_basic_qos(
        m_pConn,	//MQ连接
        m_iChannel,	//频道号
        0,		//这个参数从rabbitmq中没用到，默认为0
        1,		//设置为rabbitmq流量控制数量，取消息数目
        0		//glotal=true时表示在当前channel上所有的consumer都生效，否则只对设置了之后新建的consumer生效
    );
   

	//int no_ack = 0; // no_ack    是否需要确认消息后再从队列中删除消息
	amqp_bytes_t queuename = amqp_cstring_bytes(queue_name_.c_str());
	amqp_basic_consume(m_pConn, 
        m_iChannel,
        queuename, 
        amqp_empty_bytes, 
        /*no_local*/ 0,
        /*no_ack*/ no_ack, //如果为true，消费消息后直接从队列清除，为false，需要 * 人工调用amqp_basic_ack函数后，消息才会从队列清除。
        /*exclusive*/ 0,
        amqp_empty_table);

	if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "Consuming")) {
		amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
		return -3;
	}


	amqp_rpc_reply_t res;
	amqp_envelope_t envelope;

	amqp_maybe_release_buffers(m_pConn);
	res = amqp_consume_message(m_pConn, &envelope, timeout, 0);
	if (AMQP_RESPONSE_NORMAL != res.reply_type) {
		fprintf(stderr, "Consumer amqp_channel_close failed\n");
		amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
		return -4;

	}

	string str((char*)envelope.message.body.bytes, (char*)envelope.message.body.bytes + envelope.message.body.len);
	message = str;
	int rtn = amqp_basic_ack(m_pConn, m_iChannel, envelope.delivery_tag, 1);
	amqp_destroy_envelope(&envelope);
	if (rtn != 0) {
		amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
		return -5;
	}

    amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
	return 0;

}

int CRabbitmqClient::ErrorMsg(amqp_rpc_reply_t x, char const* context) {
    switch (x.reply_type) {
    case AMQP_RESPONSE_NORMAL:
        return 0;

    case AMQP_RESPONSE_NONE:
        fprintf(stderr, "%s: missing RPC reply type!\n", context);
        break;

    case AMQP_RESPONSE_LIBRARY_EXCEPTION:
        fprintf(stderr, "%s: %s\n", context, amqp_error_string2(x.library_error));
        break;

    case AMQP_RESPONSE_SERVER_EXCEPTION:
        switch (x.reply.id) {
        case AMQP_CONNECTION_CLOSE_METHOD: {
            amqp_connection_close_t* m = (amqp_connection_close_t*)x.reply.decoded;
            fprintf(stderr, "%s: server connection error %uh, message: %.*s\n",
                context, m->reply_code, (int)m->reply_text.len,
                (char*)m->reply_text.bytes);
            break;
        }
        case AMQP_CHANNEL_CLOSE_METHOD: {
            amqp_channel_close_t* m = (amqp_channel_close_t*)x.reply.decoded;
            fprintf(stderr, "%s: server channel error %uh, message: %.*s\n",
                context, m->reply_code, (int)m->reply_text.len,
                (char*)m->reply_text.bytes);
            break;
        }
        default:
            fprintf(stderr, "%s: unknown server error, method id 0x%08X\n",
                context, x.reply.id);
            break;
        }
        break;
    }

    return -1;
}
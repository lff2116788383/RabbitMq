#pragma once
#include <string>
#include <vector>
#include <rabbitmq-c/tcp_socket.h>
#include "CExchange.h"
#include "CQueue.h"
#include "CMessage.h"

#ifdef _WIN32
#include <winsock.h>
#elif __linux__
#include <sys/time.h>
#elif __APPLE__
#endif

using std::string;
using std::vector;

class CRabbitmqClient {
public:
	CRabbitmqClient();
	~CRabbitmqClient();


	int Connect(const string& strHostname, int iPort, const string& strUser, const string& strPasswd);
	int Disconnect();

	/**
	*   @brief       ExchangeDeclare    声明exchange
	*	@param       [in]               strExchange
	*   @param       [in]               strType
	*   @return 等于0值代表成功创建exchange，小于0代表错误
	*/
	int ExchangeDeclare(const string& strExchange, const string& strType, int _durable = 0);

	int ExchangeDeclare(CExchange& exchange);
	/**
	*   @brief       QueueDeclare                     声明消息队列
	*	@param       [in]               strQueueName  消息队列实例
	*   @param
	*   @return 等于0值代表成功创建queue，小于0代表错误
	*/
	int QueueDeclare(const string& strQueueName, int32_t  _durable = 0, int32_t _auto_delete =1);

	int QueueDeclare(CQueue& queue);
	/**
	*   @brief       QueueBind                        将队列，交换机和绑定规则绑定起来形成一个路由表
	*	@param       [in]               strQueueName  消息队列
	*	@param       [in]               strExchange   交换机名称
	*	@param       [in]               strBindKey    路由名称  “msg.#” “msg.weather.**”
	*   @return 等于0值代表成功绑定，小于0代表错误
	*/
	int QueueBind(const string& strQueueName, const string& strExchange, const string& strBindKey);

	int QueueBind(CQueue& queue, CExchange& exchange, const std::string bind_key);
	/**
	*   @brief       QueueUnbind                      将队列，交换机和绑定规则绑定解除
	*	@param       [in]               strQueueName  消息队列
	*	@param       [in]               strExchange   交换机名称
	*	@param       [in]               strBindKey    路由名称  “msg.#” “msg.weather.**”
	*   @return 等于0值代表成功绑定，小于0代表错误
	*/
	int QueueUnbind(const string& strQueueName, const string& strExchange, const string& strBindKey);

	/**
	*   @brief       QueueDelete                      删除消息队列。
	*	@param       [in]               strQueueName  消息队列名称
	*	@param       [in]               iIfUnused     消息队列是否在用，1 则论是否在用都删除
	*   @return 等于0值代表成功删除queue，小于0代表错误
	*/
	int QueueDelete(const string& strQueueName, int iIfUnused);

	/**
	* @brief Publish  发布消息
	* @param [in] strMessage        消息实体
	* @param [in] strExchange       交换器
	* @param [in] strRoutekey       路由规则
	*   1.Direct Exchange – 处理路由键。需要将一个队列绑定到交换机上，要求该消息与一个特定的路由键完全匹配。
	*   2.Fanout Exchange – 不处理路由键。将队列绑定到交换机上。一个发送到交换机的消息都会被转发到与该交换机绑定的所有队列上。
	*   3.Topic Exchange – 将路由键和某模式进行匹配。此时队列需要绑定要一个模式上。符号“#”匹配一个或多个词，符号“*”匹配不多不少一个词。
	*      因此“audit.#”能够匹配到“audit.irs.corporate”，但是“audit.*” 只会匹配到“audit.irs”
	* @return 等于0值代表成功发送消息实体，小于0代表发送错误
	*/
	int Publish(const string& strMessage, const string& strExchange, const string& strRoutekey);

	int PublishMessage(const CMessage& message, CExchange& exchange);

	int PublishMessage(std::vector<CMessage>& messageVec, CExchange& exchange);
	/**
	* @brief consumer  消费消息
	* @param [in]  strQueueName         队列名称
	* @param [out] message_array        获取的消息实体
	* @param [int] GetNum               需要取得的消息个数
	* @param [int] timeout              取得的消息是延迟，若为NULL，表示持续取，无延迟，阻塞状态
	* @return 等于0值代表成功，小于0代表错误，错误信息从ErrorReturn返回
	*/
	int Consumer(const string& strQueueName, vector<string>& message_array, int GetNum = 1, struct timeval* timeout = NULL);

	/**
	* 从指定队列中取单个消息（同步操作）
	*
	* @param[in]    queue_name_		队列名称
	* *@param[out]   message			消息内容
	* @param[in]    timeout			超时时间，为NULL有可能导致阻塞
	* @param[in]   no_ack		确认方式，true代表自动确认，false表示手动确认
	* @return 等于0值代表成功，小于0代表错误
	*/
	int ConsumerMessage(const std::string& queue_name_, std::string& message,  timeval* timeout = NULL, bool no_ack = false);
private:
	CRabbitmqClient(const CRabbitmqClient& rh);
	void operator=(const CRabbitmqClient& rh);

	int ErrorMsg(amqp_rpc_reply_t x, char const* context);


	string                      m_strHostname;      // amqp主机
	int                         m_iPort;            // amqp端口
	string					    m_strUser;
	string					    m_strPasswd;
	int                         m_iChannel;

	amqp_socket_t* m_pSock;
	amqp_connection_state_t     m_pConn;
};

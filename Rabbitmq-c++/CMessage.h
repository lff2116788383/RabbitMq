#pragma once
#include <string>
#include <rabbitmq-c/tcp_socket.h>
/**
*   @brief 消息结构
**/
class CMessage
{
public:
	CMessage(std::string data, amqp_basic_properties_t properties, std::string  routekey,
		bool bMandatory = true, bool bImmediate = false)
		:m_data(data),
		m_properties(properties),
		m_routekey(routekey),
		m_bMandatory(bMandatory),
		m_bImmediate(bImmediate)
	{

	}

	CMessage(const CMessage& other)
	{
		this->m_data = other.m_data;
		this->m_properties = other.m_properties;
		this->m_routekey = other.m_routekey;
		this->m_bMandatory = other.m_bMandatory;
		this->m_bImmediate = other.m_bImmediate;
	}

	CMessage operator=(const CMessage& other)
	{
		if (this == &other)
			return *this;
		this->m_data = other.m_data;
		this->m_properties = other.m_properties;
		this->m_routekey = other.m_routekey;
		this->m_bMandatory = other.m_bMandatory;
		this->m_bImmediate = other.m_bImmediate;
		return *this;
	}

	/**
	* m_bMandatory 消息必须路由到队列，由消费者来取
	*
	**/
	bool m_bMandatory;

	/**
	* m_bImmediate 消息是否要立即发送到消费者
	*
	**/
	bool m_bImmediate;

	/**
	* m_routekey 消息路由 默认所有消息的路由都以 “msg.”开头
	*
	**/
	std::string  m_routekey;

	/**
	* m_data 消息内容（目前RabbitMq只支持字符串，如要传json，只需写json格式即可，{\"test\":\"hello\"}）
	*
	**/
	std::string  m_data;

	/**
	* m_properties 消息的属性(包括消息头)
	*
	**/
	amqp_basic_properties_t m_properties;
};


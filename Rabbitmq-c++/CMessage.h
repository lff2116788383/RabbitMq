#pragma once
#include <string>
#include <rabbitmq-c/tcp_socket.h>
/**
*   @brief ��Ϣ�ṹ
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
	* m_bMandatory ��Ϣ����·�ɵ����У�����������ȡ
	*
	**/
	bool m_bMandatory;

	/**
	* m_bImmediate ��Ϣ�Ƿ�Ҫ�������͵�������
	*
	**/
	bool m_bImmediate;

	/**
	* m_routekey ��Ϣ·�� Ĭ��������Ϣ��·�ɶ��� ��msg.����ͷ
	*
	**/
	std::string  m_routekey;

	/**
	* m_data ��Ϣ���ݣ�ĿǰRabbitMqֻ֧���ַ�������Ҫ��json��ֻ��дjson��ʽ���ɣ�{\"test\":\"hello\"}��
	*
	**/
	std::string  m_data;

	/**
	* m_properties ��Ϣ������(������Ϣͷ)
	*
	**/
	amqp_basic_properties_t m_properties;
};


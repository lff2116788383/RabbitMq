#pragma once
#include <string>
/**
*   @brief ������
**/
class CExchange
{
public:
	CExchange(std::string name, std::string type = "direct",
		bool durable = true, bool passive = false, bool auto_delete = false, bool internal = false)
		:m_name(name),
		m_type(type),
		m_bDurable(durable),
		m_bPassive(passive),
		m_bAutoDelete(auto_delete),
		m_bInternal(internal)
	{

	}

	CExchange(const CExchange& other)
	{
		this->m_name = other.m_name;
		this->m_bDurable = other.m_bDurable;
		this->m_type = other.m_type;
		this->m_bAutoDelete = other.m_bAutoDelete;
		this->m_bInternal = other.m_bInternal;
		this->m_bPassive = other.m_bPassive;
	}

	CExchange operator=(const CExchange& other)
	{
		if (this == &other)
			return *this;
		this->m_name = other.m_name;
		this->m_bDurable = other.m_bDurable;
		this->m_type = other.m_type;
		this->m_bAutoDelete = other.m_bAutoDelete;
		this->m_bInternal = other.m_bInternal;
		this->m_bPassive = other.m_bPassive;
		return *this;
	}

	/**
	* m_name ����������
	*
	**/
	std::string   m_name;

	/**
	* m_type ָ��exchange���ͣ�"fanout"  "direct" "topic"��ѡһ
	*	"fanout" �㲥�ķ�ʽ�����͵���exchange�����ж�����(����Ҫ����bind����)
	*	"direct" ͨ��·�ɼ����͵�ָ���Ķ�����(����Ϣ���͵�ƥ��routing key�Ķ����С�)
	*	"topic" ͨ��ƥ��·�ɼ��ķ�ʽ��ȡ��ʹ��ͨ���*��#
	**/
	std::string   m_type;

	/**
	* m_durable �������Ƿ�־û�(��mq����˶Ͽ������󣬽������Ƿ񻹴���)
	**/
	bool		  m_bDurable;

	/**
	* m_auto_delete ���ӶϿ�ʱ���������Ƿ��Զ�ɾ��
	*
	**/
	bool		  m_bAutoDelete;

	/**
	* m_internal Ĭ��Ϊ0��û��ʹ�õ�
	*
	**/
	bool		  m_bInternal;

	/**
	* passive ���exchange�Ƿ����
	*	��Ϊtrue��
	*		��exchange����������ɹ����أ�����������������Ӱ��exchange���ԣ���
	*		�������ڲ��ᴴ��exchange�����ش���
	*	��Ϊfalse��
	*		���exchange�������򴴽�exchange�����óɹ����ء�
	*		���exchange�Ѿ����ڣ�����ƥ������exchange�Ļ���ɹ����أ������ƥ����exchange����ʧ�ܡ�
	**/
	bool		  m_bPassive;
};


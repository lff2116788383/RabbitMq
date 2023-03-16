#pragma once
#include <string>
/**
*   @brief ��Ϣ����
**/
class CQueue
{
public:
	CQueue(std::string strName, bool nDurable = true, bool nPassive = false,
		bool bAutoDelete = false, bool bExclusive = false)
		:m_name(strName),
		m_durable(nDurable),
		m_bPassive(nPassive),
		m_bAutoDelete(bAutoDelete),
		m_bExclusive(bExclusive)
	{

	}

	CQueue(const CQueue& other)
	{
		this->m_name = other.m_name;
		this->m_durable = other.m_durable;
		this->m_bAutoDelete = other.m_bAutoDelete;
		this->m_bExclusive = other.m_bExclusive;
		this->m_bPassive = other.m_bPassive;
	}

	CQueue operator=(const CQueue& other)
	{
		if (this == &other)
			return *this;
		this->m_name = other.m_name;
		this->m_durable = other.m_durable;
		this->m_bAutoDelete = other.m_bAutoDelete;
		this->m_bExclusive = other.m_bExclusive;
		this->m_bPassive = other.m_bPassive;
		return *this;
	}

	/**
	* m_name ��Ϣ��������
	*
	**/
	std::string   m_name;

	/**
	* m_durable �����Ƿ�־û�(��mq����˶Ͽ������󣬶����Ƿ񻹴���)
	*
	**/
	bool		  m_durable;

	/**
	* m_exclusive �Ƿ���ר�ö��У���ǰ���Ӳ���ʱ�������Ƿ�ɾ����
	*		���m_exclusive = 1����ô����ǰ���ӶϿ�ʱ������Ҳ��ɾ��
	*		���򣬵���ǰ���ӶϿ�ʱ���������ɴ���
	**/
	bool		  m_bExclusive;


	/**
	* m_auto_delete û��consumerʱ�������Ƿ��Զ�ɾ��
	*
	**/
	bool		  m_bAutoDelete;

	/**
	* passive ���queue�Ƿ����
	*	��Ϊtrue��
	*		��queue�����򴴽�����ɹ����أ�����������������Ӱ��queue���ԣ���
	*		�������ڲ��ᴴ��queue�����ش���
	*	��Ϊfalse��
	*		���queue�������򴴽������óɹ����ء�
	*		���queue�Ѿ����ڣ�����ƥ������queue�Ļ���ɹ����أ������ƥ����queue����ʧ�ܡ�
	**/
	bool		  m_bPassive;
};

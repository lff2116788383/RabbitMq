#pragma once
#include <string>
/**
*   @brief 消息队列
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
	* m_name 消息队列名称
	*
	**/
	std::string   m_name;

	/**
	* m_durable 队列是否持久化(当mq服务端断开重启后，队列是否还存在)
	*
	**/
	bool		  m_durable;

	/**
	* m_exclusive 是否是专用队列（当前连接不在时，队列是否删除）
	*		如果m_exclusive = 1，那么当当前连接断开时，队列也被删除
	*		否则，当当前连接断开时，队列依旧存在
	**/
	bool		  m_bExclusive;


	/**
	* m_auto_delete 没有consumer时，队列是否自动删除
	*
	**/
	bool		  m_bAutoDelete;

	/**
	* passive 检测queue是否存在
	*	设为true，
	*		若queue存在则创建命令成功返回（调用其他参数不会影响queue属性），
	*		若不存在不会创建queue，返回错误。
	*	设为false，
	*		如果queue不存在则创建，调用成功返回。
	*		如果queue已经存在，并且匹配现在queue的话则成功返回，如果不匹配则queue声明失败。
	**/
	bool		  m_bPassive;
};

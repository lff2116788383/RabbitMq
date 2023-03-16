#pragma once
#include <string>
/**
*   @brief 交换机
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
	* m_name 交换机名称
	*
	**/
	std::string   m_name;

	/**
	* m_type 指定exchange类型，"fanout"  "direct" "topic"三选一
	*	"fanout" 广播的方式，发送到该exchange的所有队列上(不需要进行bind操作)
	*	"direct" 通过路由键发送到指定的队列上(把消息发送到匹配routing key的队列中。)
	*	"topic" 通过匹配路由键的方式获取，使用通配符*，#
	**/
	std::string   m_type;

	/**
	* m_durable 交换机是否持久化(当mq服务端断开重启后，交换机是否还存在)
	**/
	bool		  m_bDurable;

	/**
	* m_auto_delete 连接断开时，交换机是否自动删除
	*
	**/
	bool		  m_bAutoDelete;

	/**
	* m_internal 默认为0，没有使用到
	*
	**/
	bool		  m_bInternal;

	/**
	* passive 检测exchange是否存在
	*	设为true，
	*		若exchange存在则命令成功返回（调用其他参数不会影响exchange属性），
	*		若不存在不会创建exchange，返回错误。
	*	设为false，
	*		如果exchange不存在则创建exchange，调用成功返回。
	*		如果exchange已经存在，并且匹配现在exchange的话则成功返回，如果不匹配则exchange声明失败。
	**/
	bool		  m_bPassive;
};


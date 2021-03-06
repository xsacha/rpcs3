#pragma once
#include "Emu/GS/GCM.h"
#include "Emu/GS/RSXThread.h"

struct GSRender : public RSXThread
{
	virtual ~GSRender()
	{
	}

	virtual void Close()=0;
};

enum GSLockType
{
	GS_LOCK_NOT_WAIT,
	GS_LOCK_WAIT_FLUSH,
	GS_LOCK_WAIT_FLIP,
};

struct GSLock
{
private:
	GSRender& m_renderer;
	GSLockType m_type;

public:
	GSLock(GSRender& renderer, GSLockType type)
		: m_renderer(renderer)
		, m_type(type)
	{
		switch(m_type)
		{
		case GS_LOCK_NOT_WAIT: m_renderer.m_cs_main.lock(); break;
		case GS_LOCK_WAIT_FLUSH: m_renderer.m_sem_flush.wait(); break;
		case GS_LOCK_WAIT_FLIP: m_renderer.m_sem_flip.wait(); break;
		}
	}

	~GSLock()
	{
		switch(m_type)
		{
		case GS_LOCK_NOT_WAIT: m_renderer.m_cs_main.unlock(); break;
		case GS_LOCK_WAIT_FLUSH: m_renderer.m_sem_flush.post(); break;
		case GS_LOCK_WAIT_FLIP: m_renderer.m_sem_flip.post(); break;
		}
	}
};

struct GSLockCurrent : GSLock
{
	GSLockCurrent(GSLockType type);
};
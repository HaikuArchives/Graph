//UpdateLock.cpp
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/

#include <Messenger.h>
#include "UpdateLock.h"

UpdateLock::UpdateLock(uint32 maxReaders,BMessage* newMsg):MAX_READERS(maxReaders),msg(newMsg)
{
	lockSem = create_sem(MAX_READERS,"View State");
	registerSem = create_sem(1,"Register Sem");
	//should error check sem's here
}

UpdateLock::~UpdateLock(void)
{
	delete_sem(lockSem);
	delete_sem(registerSem);
	delete msg;
}

bool UpdateLock::LockAsReader(void)
{
	if (acquire_sem(lockSem) == B_NO_ERROR)
		return true;
		
	return false;
}

bool UpdateLock::UnlockAsReader(void)
{
	if (release_sem(lockSem) == B_NO_ERROR)
		return true;
		
	return false;
}

bool UpdateLock::LockAsWriter(void)
{
	if (acquire_sem_etc(lockSem, MAX_READERS, 0,0) == B_NO_ERROR)
		return true;
	
	return false;
}

bool UpdateLock::UnlockAsWriter(bool update=true)
{
	if (release_sem_etc(lockSem, MAX_READERS,0) == B_NO_ERROR)
	{
		if (update)
			UpdateHandlers();
		return true;
	}
	return false;
}

status_t UpdateLock::Register(BHandler* handler)
{
	if (acquire_sem(registerSem) == B_NO_ERROR)
	{
		if (handler->Looper() == NULL)
			return B_BAD_VALUE;
		registeredHandlers.insert(handler);
		release_sem(registerSem);
		return B_OK;
	}
	return B_ERROR;
}

status_t UpdateLock::Unregister(BHandler* handler)
{
	if (acquire_sem(registerSem) == B_NO_ERROR)
	{
		set<BHandler*>::iterator iter;
		if ((iter=registeredHandlers.find(handler))==registeredHandlers.end())
			return B_BAD_VALUE;
		registeredHandlers.erase(iter);
		release_sem(registerSem);
		return B_OK;
	}
	return B_ERROR;
}

void UpdateLock::UpdateHandlers(void)
{
	if (acquire_sem(registerSem) == B_NO_ERROR)
	{
		set<BHandler*>::iterator iter;
		for (iter=registeredHandlers.begin(); iter!=registeredHandlers.end();iter++)
		{
			BMessenger(*iter).SendMessage(msg);
		}
		release_sem(registerSem);
	}
}

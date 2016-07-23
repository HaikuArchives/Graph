//UpdateLock.h

//A Class to allow multi-read, single write, and send updates on changes
/*
Graph, a simple graphing calculator. 
Copyright (C) 2000-2001  Daniel S Rogers 

See the readme for the full notice

*/

#ifndef UPDATE_LOCK_H
#define UPDATE_LOCK_H

#include <OS.h>
#include <set.h>
#include <Handler.h>

class UpdateLock
{
	public:
		UpdateLock(uint32 maxReaders,BMessage* msg);
		~UpdateLock(void);
		bool LockAsReader(void);
		bool UnlockAsReader(void);
		bool LockAsWriter(void);
		bool UnlockAsWriter(bool update=true);
		status_t Register(BHandler *handler);
		status_t Unregister(BHandler *handler);
	protected:
		void UpdateHandlers(void);
	private:
		sem_id lockSem;
		sem_id registerSem;
		const uint32 MAX_READERS;
		BMessage* msg;
		set<BHandler*> registeredHandlers;

};

#endif

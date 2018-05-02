/**
	@file		linux/processimpl.cpp
	@copyright	Copyright (C) 2011-2017 AJA Video Systems, Inc.  All rights reserved.
	@brief		Implements the AJAProcessImpl class on the Linux platform.
**/

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#include "ajabase/system/linux/processimpl.h"
#include "ajabase/common/timer.h"


// class AJAProcessImpl

AJAProcessImpl::AJAProcessImpl()
{
}


AJAProcessImpl::~AJAProcessImpl()
{
}

uint64_t
AJAProcessImpl::GetPid()
{
	return getpid();
}

bool
AJAProcessImpl::IsValid(uint64_t pid)
{
	if( kill(pid,0) == 0 )
		return true;
	else
		return false;
}

bool
AJAProcessImpl::Activate(uint64_t handle)
{
    AJA_UNUSED(handle);

	// Dummy placeholder
	return false;
}

bool
AJAProcessImpl::Activate(const char* pWindow)
{
    AJA_UNUSED(pWindow);

	// Dummy placeholder
	return false;
}

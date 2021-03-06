//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include "hal/Device.hpp"
#include "sys/Timer.hpp"
using namespace sys;
using namespace hal;

Device::Device(){}


#ifndef __link
int Device::read(sys::Aio & aio) const {
	aio.m_aio_var.aio_fildes = m_fd;
	return ::aio_read(&(aio.m_aio_var));
}

int Device::write(sys::Aio & aio) const {
	aio.m_aio_var.aio_fildes = m_fd;
	return ::aio_write(&(aio.m_aio_var));
}
#endif

/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#include "sys/File.hpp"
#include "sys/FileInfo.hpp"

namespace sys {

FileInfo::FileInfo() {
	memset(&m_stat, 0, sizeof(m_stat));
#if defined __link
    m_driver = File::default_driver();
#endif
}

int FileInfo::get_info(const char * path){
    int ret;
#if defined __link
    ret = File::stat(path, &m_stat, m_driver);
#else
    ret = File::stat(path, &m_stat);
#endif

    return set_error_number_if_error(ret);
}

int FileInfo::get_info(int file_no){
    int ret;
#if defined __link
    ret = link_fstat(m_driver, file_no, &m_stat);
#else
    ret = ::fstat(file_no, &m_stat);
#endif

    return set_error_number_if_error(ret);
}


bool FileInfo::is_directory() const {
    return (m_stat.st_mode & File::FORMAT) == File::DIRECTORY;
}

bool FileInfo::is_file() const {
    return (m_stat.st_mode & File::FORMAT) == File::REGULAR;
}

bool FileInfo::is_device() const {
	return is_block_device() || is_character_device();

}

bool FileInfo::is_block_device() const {
    return (m_stat.st_mode & (File::FORMAT)) == File::BLOCK;
}

bool FileInfo::is_character_device() const {
    return (m_stat.st_mode & (File::FORMAT)) == File::CHARACTER;
}

bool FileInfo::is_socket() const {
    return (m_stat.st_mode & File::FORMAT) == File::SOCKET;
}


u32 FileInfo::size() const {
	return m_stat.st_size;
}

bool FileInfo::is_executable() const {
	return false;
}

} /* namespace sys */

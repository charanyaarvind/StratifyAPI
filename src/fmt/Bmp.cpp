//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <cstdlib>
#include <cstring>
#include "fmt/Bmp.hpp"
#include "sys/Appfs.hpp"
using namespace fmt;
using namespace sys;


Bmp::Bmp(){
	m_offset = 0;
}

Bmp::Bmp(const char * name){
	open_readonly(name);
}


int Bmp::open_readonly(const char * name){
	return open(name, Bmp::READONLY);
}

int Bmp::open_readwrite(const char * name){
	return open(name, Bmp::RDWR);
}

int Bmp::open(const char * name, int access){
	bmp_header_t hdr;

	m_dib.width = -1;
	m_dib.height = -1;
	m_dib.bits_per_pixel = 0;

	if( File::open(name, access) < 0 ){
		return -1;
	}

	if( read(&hdr, sizeof(hdr)) != sizeof(hdr) ){
		close();
		return -1;
	}

	if( read(&m_dib, sizeof(m_dib)) != sizeof(m_dib) ){
		close();
		return -1;
	}

	if( seek(hdr.offset) != (int)hdr.offset ){
		m_dib.width = -1;
		m_dib.height = -1;
		m_dib.bits_per_pixel = 0;
		close();
		return -1;
	}

	m_offset = hdr.offset;
	return 0;
}

int Bmp::create(const char * name, s32 width, s32 height, u16 planes, u16 bits_per_pixel){

	bmp_header_t hdr;

	if( File::create(name, true) < 0 ){
		return -1;
	}

	hdr.size = sizeof(hdr) + sizeof(m_dib) + (width*height*bits_per_pixel + 7) / 8;
	hdr.offset = sizeof(hdr) + sizeof(m_dib);
	hdr.signature = SIGNATURE;
	hdr.resd1 = 0;
	hdr.resd2 = 0;

	if( write(&hdr, sizeof(hdr)) < 0 ){
		return -1;
	}

	m_dib.bits_per_pixel = bits_per_pixel;
	m_dib.hdr_size = sizeof(m_dib);
	m_dib.height = height;
	m_dib.width = width;
	m_dib.planes = planes;

	if( write(&m_dib, sizeof(m_dib)) < 0 ){
		return -1;
	}

	return 0;
}

int Bmp::create_appfs(const char * name, s32 width, s32 height, u16 planes, u16 bits_per_pixel, char * img, u32 nbyte){
	bmp_header_t hdr;
	bmp_dib_t dib;

	hdr.size = sizeof(hdr) + sizeof(dib) + (width*height*bits_per_pixel + 7) / 8;
	hdr.offset = sizeof(hdr) + sizeof(dib);
	hdr.signature = SIGNATURE;
	hdr.resd1 = 0;
	hdr.resd2 = 0;

	dib.bits_per_pixel = bits_per_pixel;
	dib.hdr_size = sizeof(dib);
	dib.height = height;
	dib.width = width;
	dib.planes = planes;

	memcpy(img, &hdr, sizeof(hdr));
	memcpy(img + sizeof(hdr), &dib, sizeof(dib));


	Appfs::create(name, img, nbyte);



	return 0;
}

unsigned int Bmp::calc_row_size() const{
	return (((m_dib.bits_per_pixel/8)*m_dib.width + 3) / 4) * 4;
}

int Bmp::seek_row(s32 y){
	if( m_dib.height > 0 ){
		//image is upside down -- seek to beginning of row
		return seek(m_offset + calc_row_size() * (m_dib.height - (y + 1)));
	}

	return seek(m_offset + calc_row_size() * y);
}

int Bmp::read_pixel(uint8_t * pixel, u32 pixel_size, bool mono, uint8_t thres){
	u32 avg;
	u32 i;

	if( read(pixel, pixel_size) != (int)pixel_size ){
		return -1;
	}

	if( mono ){
		avg = 0;
		for(i=0; i < pixel_size; i++){
			avg += pixel[i];
		}
		avg = avg / 8;
		if( avg > thres ){
			return 1;
		} else {
			return 0;
		}
	}

	return 0;
}




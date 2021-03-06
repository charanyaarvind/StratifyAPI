/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef RLE_HPP_
#define RLE_HPP_

#include "../sys/Appfs.hpp"
#include "../sys/File.hpp"
#include "../api/CalcObject.hpp"


namespace calc {

/*! \brief Run Length Encoding Class */
/*! \details This class implements Run length encoding and
 * decoding algorithms.
 *
 */
class Rle : public api::CalcInfoObject{
public:
	Rle();

	/*! \details Encodes a block of data using run length encoding.
	 *
	 * @param dest A pointer to the destination data
	 * @param dest_size Pass the max size of dest, this will hold the number of encoded bytes upon return
	 * @param src A pointer to the source data
	 * @param src_size The number of bytes to encode
	 * @return Number of un-encoded bytes that were processed
	 */
	static int encode(void * dest, s32 & dest_size, const void * src, s32 src_size);

	/*! \details Decodes a block of data that has been run length encoded.
	 *
	 * @param dest A pointer to the destination data
	 * @param dest_size Pass the max size of dest, this will hold the number of decoded bytes upon return
	 * @param src A pointer to the encoded data
	 * @param src_size The number of encoded bytes to process
	 * @return Number of encoded bytes that were processed
	 */
	static int decode(void * dest, s32 & dest_size, const void * src, s32 src_size);

	/*! \details Calculates the number of bytes that will be used by the
	 * compressed data.
	 *
	 * @param src A pointer to the source memory
	 * @param nbyte The number of bytes to process
	 * @return The number of bytes the data would occupy after compression
	 */
	static int calc_size(const void * src, int nbyte);

private:
	typedef struct MCU_PACK {
		u8 size;
		u8 data;
	} element_t;

	typedef struct MCU_PACK {
		u32 size;
		u32 data;
	} element32_t;

};

class RleFile : public Rle, public sys::File {
public:
	/*! \details Encodes using run length encoding and writes the data to a file.
	 *
	 * @param buf The source data
	 * @param nbyte The number of bytes to encode and write
	 * @return The number of un-encoded bytes that were written
	 */
	int write(const void * buf, int nbyte);

	/*! \details Reads from a file then decodes data.
	 *
	 * @param buf A pointer to the destination memory
	 * @param nbyte The maximum number of bytes to read
	 * @return The number of bytes read after decoding
	 */
	int read(void * buf, int nbyte);

private:
	enum {
		BUF_SIZE = 256
	};
	char buf[BUF_SIZE];
};

class RleAppfs : public Rle, public sys::Appfs {
public:
	RleAppfs();

	/*! \details Encodes then writes the data to a file.
	 *
	 * @param buf The source data
	 * @param nbyte The number of bytes to encode and write
	 * @return The number of un-encoded bytes that were written
	 */
	int write(const void * buf, int nbyte);


	/*! \details Reads and decodes data from a file.
	 *
	 * @param buf A pointer to the destination memory
	 * @param nbyte The number of bytes to read
	 * @return The number of bytes read after decoding
	 */
	int read(void * buf, int nbyte);

};

};
#endif /* RLE_HPP_ */

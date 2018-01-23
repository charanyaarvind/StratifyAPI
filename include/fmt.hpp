/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef FMT_HPP_
#define FMT_HPP_

/*! \brief Standard Data Formats
 *
 */
namespace fmt {

};

#include "fmt/Bmp.hpp"
#include "fmt/Wav.hpp"
#include "fmt/Son.hpp"

#if !defined __link
#include "fmt/Xml.hpp"
#endif

using namespace fmt;

#endif /* FMT_HPP_ */

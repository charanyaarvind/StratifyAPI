#ifndef FMT_FMT_OBJECT_HPP
#define FMT_FMT_OBJECT_HPP

#include "WorkObject.hpp"
#include "InfoObject.hpp"
#include "../sys/File.hpp"

namespace api {

/*! \brief Format Infomation Object
 *
 * \details
 * \sa fmt namespace
 *
 */
class FmtInfoObject : public virtual InfoObject {

};

/*! \brief Format Work Object Class
 *
 * \details
 * \sa fmt namespace
 *
 */
class FmtWorkObject : public virtual WorkObject {

};

/*! \brief Format File Class
 *
 * \details This class is for format object that inherit sys::File.
 *
 * \sa fmt namespace
 *
 */
class FmtFileObject : public sys::File {

};

}

#endif // FMT_FMT_OBJECT_HPP

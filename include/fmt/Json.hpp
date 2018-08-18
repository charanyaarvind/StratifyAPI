#ifndef JSON_HPP
#define JSON_HPP

#include <jansson/jansson.h>
#include "../api/FmtObject.hpp"
#include "../var/Vector.hpp"
#include "../var/String.hpp"

namespace fmt {

class Json;

class JsonError : public api::FmtInfoObject {
public:
    JsonError(){
        memset(&m_value, 0, sizeof(m_value));
    }
    int line() const { return m_value.line; }
    int column() const { return m_value.column; }
    int position() const { return m_value.position; }
    var::ConstString source() const { return m_value.source; }
    var::ConstString text() const { return m_value.text; }

private:
    friend class Json;
    json_error_t m_value;
};

class JsonObject;
class JsonArray;
class JsonBool;
class JsonReal;
class JsonNull;
class JsonInteger;
class JsonString;

class JsonValue : public api::FmtInfoObject {
public:
    JsonValue(){ m_value = 0; }

    JsonValue(json_t * value){
        m_value = value;
    }

    ~JsonValue(){
        json_decref(m_value);
        m_value = 0;
    }

    bool is_valid() const{ return m_value != 0; }

    enum type {
        OBJECT = JSON_OBJECT,
        ARRAY = JSON_ARRAY,
        STRING = JSON_STRING,
        REAL = JSON_REAL,
        INTEGER = JSON_INTEGER,
        TRUE = JSON_TRUE,
        FALSE = JSON_FALSE,
        ZERO = JSON_NULL
    };

    enum type type() const { return (enum type)json_typeof(m_value); }

    bool is_object() const { return type() == OBJECT; }
    bool is_array() const { return type() == ARRAY; }
    bool is_string() const { return type() == STRING; }
    bool is_real() const { return type() == REAL; }
    bool is_integer() const { return type() == INTEGER; }
    bool is_true() const { return type() == TRUE; }
    bool is_false() const { return type() == FALSE; }
    bool is_null() const { return type() == ZERO; }
    bool is_zero() const { return is_null(); }

    const JsonObject & to_object() const;
    const JsonArray & to_array() const;
    var::String to_string() const;
    float to_real() const;
    int to_integer() const;
    bool to_bool() const;


private:
    friend class Json;
    friend class JsonObject;
    friend class JsonArray;
    friend class JsonBool;
    friend class JsonTrue;
    friend class JsonFalse;
    friend class JsonReal;
    friend class JsonInteger;
    friend class JsonString;
    friend class JsonNull;
    json_t * m_value;

};

class JsonObject : public JsonValue {
public:

    JsonObject();
    int insert(const var::ConstString & key, const JsonValue & value);
    int update(const JsonValue & value);
    int update_existing(const JsonValue & value);
    int update_missing(const JsonValue & value);
    int remove(const var::ConstString & key);
    u32 count() const;
    int clear();

    JsonValue at(const var::ConstString & key) const;

    var::Vector<var::String> keys() const;

};

class JsonArray : public JsonValue {
public:
    JsonArray();
    u32 count() const;
};

class JsonString : public JsonValue {
public:
    JsonString(const var::ConstString & str);
};

class JsonReal : public JsonValue {
public:
    JsonReal(float value);
    JsonReal & operator=(float a);

};

class JsonInteger : public JsonValue {
public:
    JsonInteger(int value);
    JsonInteger & operator=(int a);
};

class JsonNull : public JsonValue {
public:
    JsonNull();
};

class JsonBool : public JsonValue {
public:
    JsonBool(bool value);
};

class JsonTrue : public JsonBool {
public:
    JsonTrue() : JsonBool(true){}
};

class JsonFalse : public JsonBool {
public:
    JsonFalse() : JsonBool(false){}
};

class Json : public JsonValue {
public:
    Json();

    void set_flags(u32 flags){ m_flags = flags; }

    u32 flags() const { return m_flags; }

    //load a JSON object or array from a file?
    /*!
     * \defails Loads a JSON value from a file
     * \param path The path to the file
     * \return Zero on success
     */
    int load(const var::ConstString & path);

    /*!
     * \details Loads a JSON value from a data object
     * \param data A reference to the data object containing the JSON
     * \return
     */
    int load(const var::Data & data);

    /*!
     * \details Loads a JSON value from an already open file
     * \param file A reference to the file containing JSON
     * \return Zero on success
     */
    int load(const sys::File & file);

    /*!
     * \details Loads a JSON value from streaming data
     * \param callback The function to call when more data is available
     * \param context This is passed to \a callback but not used internally
     * \return Zero on success
     */
    int load(json_load_callback_t callback, void * context);

    enum {
        REJECT_DUPLICATES = JSON_REJECT_DUPLICATES,
        DISABLE_EOF_CHECK = JSON_DISABLE_EOF_CHECK,
        DECODE_ANY = JSON_DECODE_ANY,
        DECODE_INT_AS_REAL = JSON_DECODE_INT_AS_REAL,
        ALLOW_NULL = JSON_ALLOW_NUL
    };

    const JsonError & error() const { return m_error; }

private:
    u32 m_flags;
    JsonError m_error;
};

}

#endif // JSON_HPP
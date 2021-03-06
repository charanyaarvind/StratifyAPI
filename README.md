# Stratify API

## Overview

- API Documentation
- Naming Conventions
- Building and Installing the Stratify API

If you want to write programs using the Stratify API, you should read through the naming conventions then dive into the API documentation. Also, please look at the Stratify OS programs that are published on Github as they make use of the API.

- [HelloWorld](https://github.com/StratifyLabs/HelloWorld)
- [gpio](https://github.com/StratifyLabs/gpio)
- [i2cprobe](https://github.com/StratifyLabs/i2cprobe)
- [uartprobe](https://github.com/StratifyLabs/uartprobe)

## API Documenation

If you want to write applications using the Stratify API, please see the [API documentation](https://stratifylabs.co/StratifyAPI/html/).

## Naming Conventions

Stratify API uses the following naming conventions

### Classes and Namespaces

Classes use upper CamelCase with a strong preference for a single word. Namespaces use all lowercase letters and are preferably concise and a single word.

### File and Directory Names

File names follow the class names and namespace names with hpp appended to header files and cpp appended to C++ source files.  Namespaces have their own directories which contain the header files of all the classes in the namespace.  The namespaces include a header file named after the namespace which includes all classes and declares 'using namespace <namespace>'.
	
```c++
/*! \brief Standard Calculation and Software Routines
*
*/
namespace calc {}
#include "calc/Base64.hpp"
#include "calc/Ema.hpp"
#include "calc/Lookup.hpp"
#include "calc/Pid.hpp"
#include "calc/Rle.hpp"

using namespace calc;
```

In an application, #include <sapi/calc.hpp> will allow you to declare Pid objects without using calc::Pid.  If you don't want to use the namespace (that is you need to avoid scope conflicts), just #include <sapi/calc/Pid.hpp> instead.  All library headers (including those in Stratify API) should use this approach and should never include a namespace header (or 'using namespace') in any header file other than the namespace's hpp file. The application developer can then decide to include <sapi/calc.hpp> or the individual classes <sapi/calc/Pid.hpp>.

### Methods and Functions

Methods and functions follow traditional C and C++ naming styles with lowercase and underscores to separate words.  Methods and functions should start with a verb except when directly accessing a variable.

```c++
Class Object {
public:
  int amount() const { return m_amount; } //this just returns the amount (no calculating no fetching)
  int set_amount(int v){ m_amount = v; } //this sets the value of amount
  int calc_amount() const; //this is an action that does something- "calc" is commonly used to denote a calculation
  int get_amount() const; //since this starts with a verb it needs to do something--like load amount from a file
private:
  int m_amount; //member variables have an m_ prefix
}
```

The above code uses set_* and get_* but not perhaps in the traditional way.  If get_ is used, it implies the value is not immediately available and must be loaded from somewhere.  The convention used by the method amount() (no action word) is used if the value is immediately ready or ready with a trivial calculation such as

    area(){ return m_width*m_height; }

#### References and Pointers

Parameters passed to methods and functions should be passed as references unless a pointer is more appropriate.  For example, if the item is a member of an array then a pointer would be appropriate. 
However, if the item will be operated on as a stand alone entity, a reference is preferred.  Also if the object uses only read-only methods, a const reference is best.

	void copy_mem(const char * src, char * dest, int nbytes); //a pointer here is best because src points to the data
	void print_string(const String & str); //this is better as a reference because we are just reading str
	
Pointers are also appropriate if 0 is a valid value even if there is only one item.

### Variables and Member Variables

Variables are named the same way functions are except they don't start with an action verb.  Member variables have m_ prefixed to the description variable name.  See the amount() and m_amount example above.

### Type Definitions

To maintain compatibility and interoperability with C (i.e. Stratify OS), type definitions are declared in .h files and follow C naming conventions.  Whenever typedef is used, _t should be appended to the type name like size_t or pthread_t.

### Enums and Macros

Both enums and macros are written in all caps with words separated by underscores. Enums are preferred to macros when defining constants because this allows them to auto-populate when using code completion software.  Here is an example from the hal::Device class:
```c+
enum {
    RDONLY /*! Open as read-only */ = LINK_O_RDONLY,
    READONLY /*! Open as read-only */ = LINK_O_RDONLY,
    WRONLY /*! Open as write-only */ = LINK_O_WRONLY,
    WRITEONLY /*! Open as write-only */ = LINK_O_WRONLY,
    CREATE /*! Create when opening (files) */ = LINK_O_CREAT,
    CREAT /*! Create when opening (files) */ = LINK_O_CREAT,
    TRUNCATE /*! Truncate when opening (files) */ = LINK_O_TRUNC
};
```

## Building the Stratify API

The latest API is built and distributed with the [Stratify Labs SDK](https://stratifylabs.co/download/). You only need to build and install the source code if you want to debug, contribute new features, or equip your local SDK with a previous version.

Here are the steps to build and install the API. You need to have git, CMake and the Stratify Labs SDK installed before running these commands.

The API builds for running with Stratify OS on embedded platforms. The API can also run on Windows and Mac OS X by using a serialization protocol (called link) to Stratify OS devices.  The cmake scripts check for the suffix "_arm" or "_link" to see which to build.

### Mac OS X

```
git clone https://github.com/StratifyLabs/StratifyAPI.git
cd StratifyAPI
mkdir cmake_arm; mkdir cmake_link
cd cmake_arm
cmake ..; make; make install
cd ../cmake_link
cmake ..; make; make install
```

### Windows

For windows, you need to specify some additional options because "make" typically isn't available in the PATH environment. If you are using CMD or powershell use "-G "MinGW Makefiles. If you are using msys, you can use -G "Unix Makefiles" and use the commands listed above for Mac OS X.

```
git clone https://github.com/StratifyLabs/StratifyAPI.git
cd StratifyAPI
mkdir cmake_arm; mkdir cmake_link
cd cmake_arm
cmake .. -G "MinGW Makefiles"
cmake --build . --target install
cd ../cmake_link
cmake .. -G "MinGW Makefiles"
cmake --build . --target install
```









#if !defined(_CONFIG_HPP)
#define _CONFIG_HPP

#include <miniz/miniz.h>


#define SUFFIX ".zip"
constexpr int BUF_SIZE=(1024 * 1024);

// global variables with their default values 
static bool COMP          = true;                 // by default, it compresses 
static bool REMOVE_ORIGIN = false;                // Does it keep the origin file?
static int  QUITE_MODE    = 1; 					  // 0 silent, 1 error messages, 2 verbose
static bool RECUR         = false;                // do we have to process the contents of subdirs?


#endif // _CONFIG_HPP

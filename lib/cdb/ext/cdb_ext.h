#ifndef _RUBY_EXT_CDB_H
#include "ruby.h"
#include "ruby/io.h"
#include <cdb.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

VALUE mCDB;

void Init_CDB_Reader();
void Init_CDB_Writer();

#endif

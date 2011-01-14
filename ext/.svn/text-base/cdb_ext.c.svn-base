#include "ruby.h"
#include "rubyio.h"
#include <cdb.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h>

#ifndef TINYCDB_VERSION
  #include <cdb_make.h>
  #include <uint32.h>
#else
  #define uint32 unsigned int
#endif /* TINYCDB_VERSION */

#define SETUP_ITERATION \
  cdb *db = NULL; \
  uint32 eod,klen,dlen,pos=0; \
  Data_Get_Struct(self,cdb,db); \
  getnum(db,&eod,&pos); \
  if(cdb_read(db,(void*)&dlen,4,2044) == -1) { rb_sys_fail(0); } \
  pos = 2048;
 
typedef struct cdb_make cdb_make;
typedef struct cdb cdb;

VALUE CDBMaker_alloc(VALUE klass);
VALUE CDBMaker_initialize(VALUE self,VALUE io);
VALUE CDBMaker_store(VALUE self,VALUE key,VALUE value);
VALUE CDBMaker_finish(VALUE self);
VALUE CDBMaker_is_finished(VALUE self);

VALUE CDBReader_alloc(VALUE klass);
VALUE CDBReader_initialize(VALUE self,VALUE io);
VALUE CDBReader_has_key(VALUE self,VALUE key);
VALUE CDBReader_each_for_key(VALUE self,VALUE key);
VALUE CDBReader_each(VALUE self);
VALUE CDBReader_each_key(VALUE self);
VALUE CDBReader_each_value(VALUE self);

void _cdb_read(cdb *db,void *buf,uint32 len,uint32 *pos) {
  if(cdb_read(db,buf,len,*pos) == -1) {
    rb_sys_fail(0);
  }
  *pos+=len;
  return;
}

/* How can we skip unpack on little endian? */
void 
getnum(cdb *db,uint32 *num,uint32 *pos) {
#ifndef TINYCDB_VERSION
  char buf[4];
#else
  unsigned char buf[4];
#endif /* TINYCDB_VERSION */

  _cdb_read(db,buf,4,pos);

#ifndef TINYCDB_VERSION
  uint32_unpack(buf,num); 
#else
  *num=cdb_unpack(buf);
#endif /* TINYDB_VERSION */
}

/* Implements CDB::CDBMaker */
static void 
CDBMaker_free(void *ptr) { free(ptr); }

VALUE 
CDBMaker_alloc(VALUE klass) {
  cdb_make *db = ALLOC_N(cdb_make,1);
  return Data_Wrap_Struct(klass,NULL,CDBMaker_free,db);
}

VALUE 
CDBMaker_initialize(VALUE self,VALUE io) {
  OpenFile *fptr = NULL;
  cdb_make *db = NULL;
  rb_check_type(io,T_FILE);
  GetOpenFile(io, fptr);
  rb_io_check_writable(fptr);
  rb_iv_set(self,"finished",Qfalse);
  Data_Get_Struct(self,cdb_make,db);
  if(cdb_make_start(db,fileno(GetReadFile(fptr))) == -1) 
    rb_sys_fail(0);
  #ifdef TINYCDB_VERSION
  /* tinycdb cdb_make_start does not fail itself on unseekable file descriptor! */
  if(lseek(cdb_fileno(db),0,SEEK_SET) == -1) 
    rb_sys_fail(0);
  #endif
  return self;
}

VALUE
CDBMaker_store(VALUE self,VALUE key,VALUE value) {
  if(rb_iv_get(self,"finished") == Qtrue) 
    rb_raise(rb_eRuntimeError,"cdb is already finished!");
  cdb_make *db = NULL;
  Data_Get_Struct(self,cdb_make,db);
  key = StringValue(key);
  value = StringValue(value);
  if(cdb_make_add(db,RSTRING(key)->ptr,RSTRING(key)->len,RSTRING(value)->ptr,RSTRING(value)->len) == -1) 
    rb_sys_fail(0);
  return Qnil;
}

VALUE 
CDBMaker_finish(VALUE self) {
  cdb_make *db = NULL;
  if(rb_iv_get(self,"finished") == Qtrue) 
    rb_raise(rb_eRuntimeError,"cdb is already finished!");
  rb_iv_set(self,"finished",Qtrue);
  Data_Get_Struct(self,cdb_make,db);
  if(cdb_make_finish(db) == -1) rb_sys_fail(0);
  return Qnil;
}

VALUE 
CDBMaker_is_finished(VALUE self) {
  return rb_iv_get(self,"finished");
}

/* Implements CDB::CDBReader */

static void CDBReader_free(void *ptr) {
  free(ptr);
}

VALUE 
CDBReader_alloc(VALUE klass) {
  cdb *db = ALLOC_N(cdb,1);
  return Data_Wrap_Struct(klass,NULL,CDBReader_free,db);
}

VALUE 
CDBReader_initialize(VALUE self,VALUE io) {
  rb_iv_set(self,"length",Qnil);
  OpenFile *fptr = NULL;
  cdb *db = NULL;
  rb_check_type(io,T_FILE);
  GetOpenFile(io, fptr);
  rb_io_check_readable(fptr);
  Data_Get_Struct(self,cdb,db);
#ifdef TINYCDB_VERSION
  if(cdb_init(db,fileno(GetReadFile(fptr))) == -1) {
    rb_sys_fail(0);
  }
#else
  cdb_init(db,fileno(GetReadFile(fptr)));
  /* cdb cdb_init does not fail itself on unseekable file descriptor or too small file! */
  /* TODO: something helpfull if db->size is uninitialized? (see cdb.c from djb) */
  if(db->size < 2048) {
    errno = EPROTO;
    rb_sys_fail(0);
  }
#endif
  return self;
}

VALUE 
CDBReader_has_key(VALUE self,VALUE key) {
  cdb *db = NULL;
  Data_Get_Struct(self,cdb,db);
  key = StringValue(key);
  return cdb_find(db,RSTRING(key)->ptr,RSTRING(key)->len) ? Qtrue : Qfalse;
}



VALUE
CDBReader_each_key(VALUE self) {
  VALUE key;
  SETUP_ITERATION

  while(pos < eod) {
    getnum(db,&klen,&pos);
    getnum(db,&dlen,&pos);
    key = rb_str_buf_new(klen);
    RSTRING(key)->len = klen;
    _cdb_read(db,RSTRING(key)->ptr,klen,&pos);
    pos+=dlen;
    rb_yield(key);
  }

  return Qnil;
}

VALUE 
CDBReader_each_value(VALUE self) {
  VALUE value;
  SETUP_ITERATION

  while(pos < eod) {
    getnum(db,&klen,&pos);
    getnum(db,&dlen,&pos);
    value = rb_str_buf_new(dlen);
    RSTRING(value)->len = dlen;
    pos+=klen;
    _cdb_read(db,RSTRING(value)->ptr,dlen,&pos);
    rb_yield(value);
  }
  return Qnil;
}

VALUE CDBReader_count(VALUE self) {
  SETUP_ITERATION
  int count = -1;
  while(pos < eod) {
    getnum(db,&klen,&pos);
    getnum(db,&dlen,&pos);
    pos+=klen;
    pos+=dlen;
    count++;
  }
  return INT2FIX(count);
}

VALUE 
CDBReader_each(VALUE self) {
  VALUE key,value;
  SETUP_ITERATION

  while(pos < eod) {
    getnum(db,&klen,&pos);
    getnum(db,&dlen,&pos);
    key = rb_str_buf_new(klen);
    RSTRING(key)->len = klen;
    value = rb_str_buf_new(dlen);
    RSTRING(value)->len = dlen;

    _cdb_read(db,RSTRING(key)->ptr,klen,&pos);
    _cdb_read(db,RSTRING(value)->ptr,dlen,&pos);

    rb_yield(rb_ary_new3(2,key,value));
  }
  return Qnil;
}

VALUE 
CDBReader_each_for_key(VALUE self,VALUE key) {
  cdb *db = NULL;
  Data_Get_Struct(self,cdb,db);
  VALUE value;
  key = StringValue(key);
#ifndef TINYCDB_VERSION
  if(cdb_find(db,RSTRING(key)->ptr,RSTRING(key)->len) == -1) rb_sys_fail(0);
  do {
    value = rb_str_buf_new(cdb_datalen(db));
    RSTRING(value)->len = cdb_datalen(db);
    _cdb_read(db,RSTRING(value)->ptr,cdb_datalen(db),&cdb_datapos(db));
    rb_yield(value);
  } while(cdb_findnext(db,RSTRING(key)->ptr,RSTRING(key)->len));
#else 
  struct cdb_find find;
  cdb_findinit(&find,db,RSTRING(key)->ptr,RSTRING(key)->len);
  while(cdb_findnext(&find) > 0) {
    value = rb_str_buf_new(cdb_datalen(db));
    RSTRING(value)->len = cdb_datalen(db);
    _cdb_read(db,RSTRING(value)->ptr,cdb_datalen(db),&cdb_datapos(db));
    rb_yield(value);
  }
#endif /* TINYCDB_VERSION */
  return Qnil;
}

VALUE mCDB;
VALUE cCDBReader;
VALUE cCDBMaker;

void Init_cdb_ext() {
  mCDB = rb_define_module("CDB");

  cCDBReader = rb_define_class_under(mCDB,"CDBReader",rb_cObject);
  rb_define_alloc_func(cCDBReader,CDBReader_alloc);
  rb_define_method(cCDBReader,"initialize",CDBReader_initialize,1);
  rb_define_method(cCDBReader,"has_key?",CDBReader_has_key,1);
  rb_define_method(cCDBReader,"each_for_key",CDBReader_each_for_key,1);
  rb_define_method(cCDBReader,"each_key",CDBReader_each_key,0);
  rb_define_method(cCDBReader,"each_value",CDBReader_each_value,0);
  rb_define_method(cCDBReader,"each",CDBReader_each,0);
  rb_define_method(cCDBReader,"count",CDBReader_count,0);

  cCDBMaker = rb_define_class_under(mCDB,"CDBMaker",rb_cObject);
  rb_define_alloc_func(cCDBMaker,CDBMaker_alloc);
  rb_define_method(cCDBMaker,"initialize",CDBMaker_initialize,1);
  rb_define_method(cCDBMaker,"store",CDBMaker_store,2);
  rb_define_method(cCDBMaker,"finish!",CDBMaker_finish,0);
  rb_define_method(cCDBMaker,"finished?",CDBMaker_is_finished,0);
}

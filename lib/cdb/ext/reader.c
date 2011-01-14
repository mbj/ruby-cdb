#include "cdb_ext.h"

int open_cdb_fd(VALUE reader) {
  VALUE filename = rb_iv_get(reader,"@filename");
  if(filename == Qnil) {
    rb_raise(rb_eRuntimeError,"no filename set?");
  }
  StringValue(filename);
  int fd = open(StringValueCStr(filename), O_RDONLY);
  if(fd == -1) {
    rb_raise(rb_eRuntimeError,"unable to open filename");
  }
  return fd;
}


VALUE mCDB_Reader_get(VALUE self, VALUE key) {
  struct cdb db;
  VALUE value;
  StringValue(key);
  int fd = open_cdb_fd(self);
  size_t vlen;
  cdb_init(&db,fd);
  if(cdb_find(&db,RSTRING_PTR(key),RSTRING_LEN(key)) > 0) {
    vlen = cdb_datalen(&db);
    value = rb_str_buf_new(vlen);
    cdb_read(&db,RSTRING_PTR(value),vlen,cdb_datapos(&db));
    rb_str_set_len(value,vlen);
    return value;
  }
  close(fd);
  return Qnil;
}

VALUE mCDB_Reader_each_for_key(VALUE self,VALUE key) {
  struct cdb db;
  VALUE value;
  struct cdb_find find;
  StringValue(key);
  int fd = open_cdb_fd(self);
  size_t vlen;
  cdb_init(&db,fd);
  cdb_findinit(&find,&db,RSTRING_PTR(key),RSTRING_LEN(key));
  while(cdb_findnext(&find) > 0) {
    vlen = cdb_datalen(&db);
    value = rb_str_buf_new(vlen);
    cdb_read(&db,RSTRING_PTR(value),vlen,cdb_datapos(&db));
    rb_str_set_len(value,vlen);
    rb_yield(value);
  }
  close(fd);
  return Qnil;
}

VALUE mCDB_cReader;

void Init_CDB_Reader() {
  mCDB = rb_define_module("CDB");

  mCDB_cReader = rb_define_class_under(mCDB,"Reader",rb_cObject);
  rb_define_method(mCDB_cReader,"each_for_key",mCDB_Reader_each_for_key,1);
  rb_define_method(mCDB_cReader,"get",mCDB_Reader_get,1);
}

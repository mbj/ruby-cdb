#include "cdb_ext.h"
VALUE mCDB_Writer_alloc(VALUE klass);
VALUE mCDB_Writer_initialize(VALUE self,VALUE io);
VALUE mCDB_Writer_store(VALUE self,VALUE key,VALUE value);
VALUE mCDB_Writer_close(VALUE self);
VALUE mCDB_Writer_is_closed(VALUE self);

static void mCDB_Writer_free(void *ptr) { free(ptr); }

VALUE mCDB_Writer_alloc(VALUE klass) {
  struct cdb_make *db = ALLOC_N(struct cdb_make,1);
  return Data_Wrap_Struct(klass,NULL,mCDB_Writer_free,db);
}

VALUE mCDB_Writer_initialize(VALUE self,VALUE obj) {
  rb_io_t *fptr = NULL;
  struct cdb_make *db = NULL;
  rb_check_type(obj,T_FILE);
  GetOpenFile(obj, fptr);
  rb_io_check_writable(fptr);
  rb_iv_set(self,"closed",Qfalse);
  Data_Get_Struct(self,struct cdb_make,db);
  if(cdb_make_start(db,fptr->fd) == -1) {
    rb_sys_fail(0);
  }
  if(lseek(cdb_fileno(db),0,SEEK_SET) == -1)  {
    rb_sys_fail(0);
  }
  return self;
}

VALUE mCDB_Writer_store(VALUE self,VALUE key,VALUE value) {
  struct cdb_make *db = NULL;
  if(rb_iv_get(self,"closed") == Qtrue) {
    rb_raise(rb_eRuntimeError,"cdb was already closed!");
  }
  Data_Get_Struct(self,struct cdb_make,db);
  key = StringValue(key);
  value = StringValue(value);
  if(cdb_make_add(db,RSTRING_PTR(key),RSTRING_LEN(key),RSTRING_PTR(value),RSTRING_LEN(value)) == -1) {
    rb_sys_fail(0);
  }
  return Qnil;
}

VALUE mCDB_Writer_close(VALUE self) {
  struct cdb_make *db = NULL;
  if(rb_iv_get(self,"closed") == Qtrue)  {
    rb_raise(rb_eRuntimeError,"cdb was already closed!");
  }
  rb_iv_set(self,"closed",Qtrue);
  Data_Get_Struct(self,struct cdb_make,db);
  if(cdb_make_finish(db) == -1) {
    rb_sys_fail(0);
  }
  return Qnil;
}

VALUE mCDB_Writer_is_closed(VALUE self) {
  return rb_iv_get(self,"closed");
}

VALUE mCDB_cWriter;

void Init_CDB_Writer() {
  mCDB = rb_define_module("CDB");
  mCDB_cWriter = rb_define_class_under(mCDB,"Writer",rb_cObject);
  rb_define_alloc_func(mCDB_cWriter,mCDB_Writer_alloc);
  rb_define_method(mCDB_cWriter,"initialize",mCDB_Writer_initialize,1);
  rb_define_method(mCDB_cWriter,"store",mCDB_Writer_store,2);
  rb_define_method(mCDB_cWriter,"close!",mCDB_Writer_close,0);
  rb_define_method(mCDB_cWriter,"closed?",mCDB_Writer_is_closed,0);
}

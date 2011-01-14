require 'mkmf'
$CFLAGS << " -Wall"
$CFLAGS << " -Werror"
have_header("cdb.h") || exit
if macro_defined?("TINYCDB_VERSION",cpp_include("cdb.h"))
  puts "using tinycdb library"
  $LDFLAGS << " -lcdb"
else
  raise "this package currently only builds against tinycdb"
end
create_makefile("cdb")

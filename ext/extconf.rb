require 'mkmf'
have_header("cdb.h") || exit
if macro_defined?("TINYCDB_VERSION",cpp_include("cdb.h"))
  $LDFLAGS << " -lcdb"
else
  %w(cdb unix buffer byte alloc).each do |ld|
    $LDFLAGS << " -l:#{ld}.a"
  end
end
$CFLAGS << " -Wall"
create_makefile("cdb/cdb_ext")

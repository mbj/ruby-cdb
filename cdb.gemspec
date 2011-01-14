require 'rubygems'
spec = Gem::Specification.new do |s|
  s.name = "cdb"
  s.version = "0.0.4"
  s.author = "Markus Schirp"
  s.email = "mbj@seonic.net"
  s.homepage = "http://mbj.seonic.net"
  s.platform = Gem::Platform::RUBY
  s.summary = "Reader and writer for CDB Constant Databases"
  s.has_rdoc = false
  s.rubyforge_project = "cdb"
  files = ["README"]
  %w(lib lib/cdb test).each do |dir|
    files.concat Dir.glob("#{dir}/*")
  end
  s.files = files
  s.extensions = ["lib/cdb/ext/extconf.rb"]
  s.test_file = "test/ts_cdb.rb"
end

if $0 == __FILE__
  Gem::Builder.new(spec).build
end

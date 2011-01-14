require 'rubygems'
spec = Gem::Specification.new do |s|
  s.name = "cdb"
  s.version = "0.0.3"
  s.author = "Markus Schirp"
  s.email = "mbj@seonic.net"
  s.homepage = "http://mbj.seonic.net"
  s.platform = Gem::Platform::RUBY
  s.summary = "Reading and creating CDB Constant Databases"
  s.has_rdoc = false
  s.rubyforge_project = "cdb"
  files = ["README"]
  %w(lib lib/cdb test).each do |dir|
    files.concat Dir.glob("#{dir}/*")
  end
  files.concat Dir.glob("ext/{*.rb,*.c}")
  s.files = files
  s.extensions = ["ext/extconf.rb"]
  s.test_file = "test/ts_cdb.rb"
end

if $0 == __FILE__
  Gem::manage_gems
  Gem::Builder.new(spec).build
end

require 'cdb'
require 'tempfile'
require 'test/unit'

require 'test_data'

class TestCDBMaker < Test::Unit::TestCase
  include ::CDB
  include ::TestData

  def setup
    @tmpfile = Tempfile.new("cdb_maker_test")
    @db = CDBMaker.new(@tmpfile.to_io)
  end

  def test_initialize
    assert_raise(Errno::ESPIPE) do
      CDBMaker.new($stdout)
    end
    assert_raise(TypeError) do
      CDBMaker.new("test")
    end
  end

  def test_store
    Data.each do |key,values|
      values.each do |value|
        @db.store(key,value)
      end
    end
    @db.finish!
    assert(@db.finished?)
  end
end

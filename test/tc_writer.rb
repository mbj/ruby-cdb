require 'cdb'
require 'tempfile'
require 'test/unit'

require 'test_data'

class TestWriter < Test::Unit::TestCase
  include ::CDB
  include ::TestData

  def setup
    @tmpfile = Tempfile.new("cdb_maker_test")
    @db = Writer.new(@tmpfile.to_io)
  end

  def test_initialize
    assert_raise(Errno::ESPIPE) do
      Writer.new($stdout)
    end
    assert_raise(TypeError) do
      Writer.new("test")
    end
  end

  def test_store
    Data.each do |key,values|
      values.each do |value|
        @db.store(key,value)
      end
    end
    @db.close!
    assert(@db.closed?)
  end
end

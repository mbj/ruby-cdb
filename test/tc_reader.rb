#!/usr/bin/ruby
require 'test/unit'
require 'tempfile'
require 'test_data'

require 'cdb'

class TestReader < ::Test::Unit::TestCase
  include ::TestData

  @@file_path = "test.cdb"

  CDB::Writer.create(@@file_path) do |db|
    Data.each do |key,values|
      values.each do |value|
        db.store(key,value)
      end
    end
  end

  def setup
    @db = CDB::Reader.new(@@file_path)
  end

  def test_get
    assert_equal("value1.1",@db["key1"])
  end


  def test_each_for_key
    Data.each do |key,ovalue|
      rvalue = Array.new
      @db.each_for_key(key) do |x|
        rvalue << x
      end
      assert_equal(ovalue,@db.values(key))
      assert_equal(ovalue,rvalue)
    end
  end

  def test_initialize
    assert_raise(ArgumentError) do
      CDB::Reader.new($stdout)
    end

    assert_raise(ArgumentError) do 
      CDB::Reader.new(1)
    end
  end
end

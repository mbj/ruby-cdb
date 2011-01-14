#!/usr/bin/ruby
require 'test/unit'
require 'tempfile'
require 'test_data'

require 'cdb'

class TestCDBReader < ::Test::Unit::TestCase
  include ::TestData

  class Dummy
    def to_str; "lala" end
  end

  class Dummy2
    def to_str; "key1"; end
  end

  db = CDB::CDBMaker.new(file=Tempfile.new("test_cdb").to_io)

  Data.each do |key,values|
    values.each do |value|
      db.store(key,value)
    end
  end

  db.finish!
  @@file_path = file.path
  file.close

  def setup
    @db = CDB::CDBReader.new(@file=File.open(@@file_path))
  end

  def teardown
    @file.close
  end

  def test_empty
    Tempfile.new("empty_test_cdb") do |file|
      assert_raise(Errno::EPROTO,"should fail while reading from empty file!") do
        CDB::CDBReader.new(file.to_io)
      end
    end
  end

  def test_has_key?
    Data.each do |pair|
      key,value = pair
      assert(@db.has_key?(key),"key: #{key} was not found!")
    end
    assert(!@db.has_key?("nonexistend"))
    assert_raise(TypeError) do
      assert(!@db.has_key?(1))
    end
    assert(!@db.has_key?(Dummy.new))
    assert(@db.has_key?(Dummy2.new))
  end

  def test_each
    tmp = Hash.new
    Data.each do |key,value|
      tmp[key] = value.dup
    end
    @db.each do |key,value|
      assert(tmp.has_key?(key))
      values = tmp.fetch(key)
      assert(values.include?(value))
      values.delete(value)
      tmp.delete(key) if values.empty?
    end
    assert(tmp.empty?)
  end

  def test_length
    assert_equal(DataLength,@db.length)
  end

  def test_each_key
    tmp = Hash.new
    @db.each_key do |key|
      assert(Data.has_key?(key))
      if tmp[key] 
        tmp[key]+=1
      else
        tmp[key]=1
      end
    end
    tmp.each do |key,count|
      assert_equal(Data[key].length,count)
    end
  end

  def test_each_value
    values = Data.values.flatten
    @db.each_value do |value|
      assert(values.include?(value),"#{value} was not expected!")
      values.delete(value)
    end
    assert(values.empty?)
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
    assert_raise(IOError) do
      CDB::CDBReader.new($stdout)
    end

    assert_raise(TypeError) do 
      CDB::CDBReader.new(1)
    end

    assert_raise(Errno::EPROTO,"should fail reading from stdin while stdin is not a file") do
      CDB::CDBReader.new($stdin)
    end
  end
end

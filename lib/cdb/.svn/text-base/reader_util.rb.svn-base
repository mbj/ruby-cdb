require 'cdb/cdb_ext'

module CDB
class CDBReader
  def values(key)
    ret = Array.new
    each_for_key(key) do |value|
      ret << value 
    end
    ret
  end

  # computing the number of elements in a cdb is not possible.
  #
  # CDBReader#count internally itereates over the hole data
  # section, so you should call #length since it caches the
  # result of #count.

  def length
    return @length unless @length.nil?
    return @length = count;
  end
end
end

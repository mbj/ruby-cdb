module CDB
class Reader
  alias :[] :get
  def initialize(path)
    unless path.kind_of?(String)
      raise ArgumentError,"argument has to be kind of string"
    end
    unless File.exist?(path)
      raise ArgumentError,"file: #{path.inspect} does not exist"
    end
    @filename = path
  end
  def values(key)
    ret = Array.new
    each_for_key(key) do |value|
      ret << value 
    end
    ret
  end
end # Reader
end # CDB

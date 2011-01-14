module TestData
  LARGE_KEY = "a"*1024
  LARGE_VALUE = "b"*(1024*1024)
  class Dummy
    def to_str
      "lala"
    end
  end

  class Dummy2
    def to_str
      "key1"
    end
  end

  Data = Hash.new

  count = 0
  (1..10).each do |x|
    key = "key#{x}"
    values = Array.new
    Data[key] = values
    1.upto(x) do |i|
      count+=1
      values << value = "value#{x}.#{i}"
    end
  end

  DataLength = count

  Data[LARGE_KEY]=Array(LARGE_VALUE)
end

module TestData
  LARGE_KEY = "a"*1024
  LARGE_VALUE = "b"*(1024*1024)

  Data = Hash.new

  (1..10).each do |x|
    key = "key#{x}"
    values = Array.new
    Data[key] = values
    1.upto(x) do |i|
      values << value = "value#{x}.#{i}"
    end
  end

  Data[LARGE_KEY]=Array(LARGE_VALUE)
end

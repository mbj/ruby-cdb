module CDB
class Writer
  alias :[]= :store
  def self.create(filename)
    File.open(filename,"wb") do |file|
      p file
      writer = self.new(file)
      yield writer
      writer.close!
    end
  end
end
end

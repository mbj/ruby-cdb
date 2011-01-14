module CDB
class CDBMaker
  def self.fill(file)
    maker = self.new(file)
    yield maker
    maker.finish!
  end
end
end

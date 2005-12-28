module DosDisk
  @@ACK = 0x06
  @@SECTORS = 16
  @@TRACKS = 35
  @@TOTAL_SECTORS = @@SECTORS * @@TRACKS
end

class IO
  def bin(*objs)
    objs.each do |obj|
      if obj.kind_of?(Numeric)
        putc obj
      else
        print obj
      end
    end
  end
end

class StringIO
  def bin(*objs)
    objs.each do |obj|
      if obj.kind_of?(Numeric)
        putc obj
      else
        print obj
      end
    end
  end
end
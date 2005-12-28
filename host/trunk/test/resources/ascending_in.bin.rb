#!/usr/bin/env ruby
$VERBOSE = 1;

$:.unshift File.join(File.dirname(__FILE__))

require 'stringio'
require 'dos_disk'

include DosDisk

file = __FILE__.sub(/\.rb/, "");

sector_data = StringIO.new
sector_data.bin 0x00
0x100.times { sector_data.bin(0x01) }
sector_data.bin 0x55, 0x7E

File.open(file, "w") do |file|
  file.bin "file.dsk", 0x00
  file.bin @@ACK
  @@TOTAL_SECTORS.times do
    file.bin sector_data.string
  end
  file.bin 0x00
end

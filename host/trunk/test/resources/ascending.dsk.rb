#!/usr/bin/env ruby
$VERBOSE = 1;

$:.unshift File.join(File.dirname(__FILE__))

require 'stringio'
require 'dos_disk'

include DosDisk

file = __FILE__.sub(/\.rb/, "");

sector_data = StringIO.new
(0 .. 255).each { |b| sector_data.bin(b) }

File.open(file, "w") do |file|
  file.bin sector_data.string * @@TOTAL_SECTORS
end

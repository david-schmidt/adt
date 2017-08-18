#!/usr/bin/env ruby
$VERBOSE = true

require 'readbytes'
require 'stringio'
require 'pp'

CMD = File.basename($0)

class SstDisk
  def initialize(file1, file2)
    @disks = Array.new
    puts "Reading: #{file1}"
    @disks[0]= read_disk(file1)
    puts "Reading #{file2}"
    @disks[1] = read_disk(file2)
  end

  def read_disk(file_name)
    disk = Array.new(35) { |i| Array.new(16) }

    track = 0
    sector = 0
    File.open(file_name) do |f|
      loop do
        data = f.readbytes(256)
        disk[track][sector] = data

        sector += 1
        if (sector == 16)
          sector = 0
          track += 1
        end
        break if track == 35
      end
      return disk
    end
  end

  SECTOR_ORDER = [
    [0x00, 0x0F], [0x00, 0x0E], [0x00, 0x0D], [0x00, 0x0C],
    [0x00, 0x0B], [0x00, 0x0A], [0x00, 0x09], [0x00, 0x08],
    [0x00, 0x07], [0x00, 0x06], [0x00, 0x05], [0x00, 0x04],
    [0x00, 0x03], [0x00, 0x02], [0x00, 0x01], [0x00, 0x00],
    [0x01, 0x0F], [0x01, 0x0E], [0x01, 0x0D], [0x01, 0x0C],
    [0x01, 0x0B], [0x01, 0x0A], [0x01, 0x09], [0x01, 0x08],
    [0x01, 0x07], [0x01, 0x06], [0x01, 0x05], [0x01, 0x04],
  ]
  
  def write_nib_data(nib_data)
    35.times do |nib_track|
      track_data = String.new

      SECTOR_ORDER.each do |pair|
        track_offset = pair[0];
        sector_number = pair[1];
        sst_track = nib_track * 2 + track_offset
        disk_number, track_number = sst_track.divmod(35)
        # puts "#{disk_number} #{track_number} #{sector_number}"
        track_data << @disks[disk_number][track_number][sector_number]
      end

      track_offset, sector_number = 0x01, 0x02
      sst_track = nib_track * 2 + track_offset
      disk_number, track_number = sst_track.divmod(35)
      params = @disks[disk_number][track_number][sector_number]

#      printf("B2C1: %02X B2C2: %02X B2C3: %02X B2C7: %02X B2C8: %02X\n",
#        params[0xC1], params[0xC2], params[0xC3], params[0xC7], params[0xC8])

      track_start_address = (params[0xC2] << 8) | params[0xC1]
      track_length = (params[0xC8] << 8) | params[0xC7]
      track_start_offset = track_start_address - 0x7800
#      printf("Track start: %04X, offset: %04X, length: %04X\n",
#        track_start_address, track_start_offset, track_length)
      
      x = 6656 - track_start_offset
#     printf("%d: %02X %02X\n", nib_track, x, x + 0x7800)
      6656.times do |nib_byte|
        nib_offset = nib_byte + track_start_offset
        offset = (nib_offset) % track_data.length
        byte = (track_data[offset] | 0x80)
        nib_data << byte.chr
      end
    end
  end
end

if ARGV.length != 3
  puts "Usage: #{CMD} <sst_disk_1> <sst_disk_2> <nib_disk>"
  exit 1
end

sst_disk = SstDisk.new(ARGV[0], ARGV[1])
nib_disk = ARGV[2]
File.open(nib_disk, "w") do |f|
  puts "Writing #{nib_disk}"
  sst_disk.write_nib_data(f)
end


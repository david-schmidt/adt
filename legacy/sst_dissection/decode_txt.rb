#!/usr/bin/env ruby
$VERBOSE = true

$message = ""

def flush_message
  puts "        ; Message: <#{$message}>" if !$message.empty?
  $message = ""
end

ARGF.each do |line|
  flush_message if line =~ /^msg_\w+:\s+/
  puts line
  if line =~ /\.byte\s+\$\w\w,\S+\s+;\s+\w\w\w\w(( \w\w)+)/
    matched = $1
    bytes = matched.split
    control_bytes = %w{ FB FC FD }
    bytes.each do |byte|
      next if control_bytes.include?(byte)
      flush_message if (byte == "C0") || (byte == "FF")
      # print "#{byte} -> "
      char = byte.hex
      # print "#{char} -> "
      char &= 0x7f
      # print "#{char} -> "
      # Make ASCII again
      char += 0x40 if char < 0x20
      # print "#{char} -> "
      # puts char.chr
      $message << char.chr
    end
  end
end

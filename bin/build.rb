#!/usr/bin/env ruby

require 'open3'
require 'fileutils'

AVR_BIN = 'C:\Program Files (x86)\Arduino\hardware\tools\avr\bin'

args = %w(-I ihex -O binary ThermalDetonator.ino.arduino_standard.hex build/FIRMWARE.BIN)

puts "Generating FIRMWARE.BIN"
stdout, stderr, status = Open3.capture3(AVR_BIN + "\\avr-objcopy.exe", *args)
puts "EXIT: #{status}"

if ARGV[0]
  target = ARGV[0] + "\\FIRMWARE.BIN"
  puts "Copying to #{target}"
  FileUtils.cp("build/FIRMWARE.BIN", target)
end
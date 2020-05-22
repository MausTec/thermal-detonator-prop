#!/usr/bin/env ruby

require 'open3'
require 'fileutils'
require 'optimist'

AVR_BIN = 'C:\Program Files (x86)\Arduino\hardware\tools\avr\bin'

opts = Optimist::options do
  banner <<-TEXT
Export firmware image and assets to an SD card image.
TEXT

  opt :out, "Target to write files to", type: :string
  opt :assets, "Include assets", default: true
  opt :avr_bin, "Path to avr bin directory", default: AVR_BIN
end

class AVR

  class AVRError < RuntimeError; end

  def initialize(bin_dir = AVR_BIN)
    @bin_dir = bin_dir
  end

  def objcopy(input_file:, output_file:, output_type: "binary", input_type: "ihex")
    args = ["-I", input_type, "-O", output_type, input_file, output_file]

    puts "Generating FIRMWARE.BIN"
    stdout, stderr, status = Open3.capture3(AVR_BIN + "\\avr-objcopy.exe", *args)
    puts stdout
    puts stderr
    puts "EXIT: #{status}"

    if status != 0
      raise AVRError, stderr
    end
  end
end

avr = AVR.new(opts[:avr_bin])
avr.objcopy input_file: "ThermalDetonator.ino.arduino_standard.hex",
            output_file: "build/FIRMWARE.BIN"

if opts[:out]
  target = opts[:out] + "\\FIRMWARE.BIN"
  puts "Copying to #{target}"
  FileUtils.cp("build/FIRMWARE.BIN", target)

  if opts[:assets]
    Dir.glob("assets/**/*").each do |file|
      target = file.gsub("assets", opts[:out])
      puts "CP #{file} -> #{target}"
      if File.directory? file
        FileUtils.mkdir_p(target)
      else
        FileUtils.cp(file, target)
      end
    end
  end
end
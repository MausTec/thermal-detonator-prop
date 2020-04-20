#!/usr/bin/env ruby

err = [];
lc = "";

File.foreach("errors.h") { |l|
    l =~ /(ERR_\w+)\s+(.*)/ && err[$2.to_i(2)] = [$1, lc] ;
    l =~ /^\/\/\s*(.*)$/ && lc = $1
};

1.upto(15) { |x|
    puts "|`0x%X`|`%s`|`%s`|%s|" % [
        x,
        "#{x&8>0?'W':'-'}#{x&4>0?'W':'-'}#{x&2>0?'W':'-'}#{x&1>0?'R':'-'}",
        *(err[x-1] || [' ',' '])
    ]
}
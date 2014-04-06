#! /usr/bin/awk -f
$0!~/^;/  { print $0;}

#{ sub(/^[ \t]+/, ""); print}

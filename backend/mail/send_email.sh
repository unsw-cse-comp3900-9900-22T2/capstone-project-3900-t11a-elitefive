#!/bin/sh

toaddr=$1
title=$2
message=$3

echo "$message" | mutt -s "$title" -e 'set copy=no' -- $toaddr


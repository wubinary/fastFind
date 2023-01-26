#!/bin/bash

echo run | gdb -q backtrace 3 -full --args ./ff main.cpp ~


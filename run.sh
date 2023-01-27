#!/bin/bash

read -r -d '' GDB_CMDS <<EOF
run
bt
EOF

echo "$GDB_CMDS" | gdb -q --full --args ./ff main.cpp ~


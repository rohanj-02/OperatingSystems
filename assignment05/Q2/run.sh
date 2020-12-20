#!/bin/bash
# Shell script to open terminals
# and execute a separate command in each

# Commands to run (one per terminal)
cmds=('./run_editor.o read test.txt' './run_editor.o write test.txt')

# Loop through commands, open terminal, execute command
for i in "${cmds[@]}"
do
    x-terminal-emulator -e "$i" &
    sleep 2
done
#!/bin/bash
	# Name: Rohan Jain
	# Roll No: 2019095 

# Commands to run (Space seperated)
cmds=('./run_editor.o read test.txt' './run_editor.o write test.txt')

# Loop through commands, open terminal, execute command, & for bg process
for i in "${cmds[@]}"
do
    x-terminal-emulator -e "$i" &
    sleep 2
done
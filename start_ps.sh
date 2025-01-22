#!/bin/bash
PROGRAM_PATH="F:/bots/build/Release/bots.exe"
powershell.exe -Command "Start-Process '$PROGRAM_PATH' -Verb RunAs"
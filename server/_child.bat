@echo off
cd out\listenserver_dbg
copy RApiServer.exe child.exe
title Child ....
start child.exe lastchild
cls
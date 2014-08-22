@echo off
cd out\listenserver_dbg
copy RApiServer.exe listen.exe
title listen ...
start /w listen.exe manualchildserver
cls
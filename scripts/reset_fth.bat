@echo off
:: Reset Windows FTH (Fault Tolerant Heap) to disable crash prevention when testing OpenLRR.
:: This must be run as an administrator.
:: <https://docs.microsoft.com/en-us/windows/win32/win7appqual/fault-tolerant-heap>

:: If you see the following message while debugging in Visual Studio,
:: then FTH has been activated and must be disabled again:
::   "Fault tolerant heap shim applied to current process. This is usually due to previous crashes"

Rundll32.exe fthsvc.dll,FthSysprepSpecialize

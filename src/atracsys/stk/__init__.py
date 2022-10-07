import os

if os.name == 'nt':
    import winreg
    import ctypes
    from ctypes import wintypes
    from contextlib import contextmanager

    registry_key = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r"SOFTWARE\Atracsys\spryTrack" , 0,
                                    winreg.KEY_READ)
    sdk_path, regtype = winreg.QueryValueEx(registry_key, "Root")
    sdk_path=os.path.join(sdk_path,'bin')
    print("Using SDK installed in {0}".format(sdk_path))
    kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)
    kernel32.SetDllDirectoryW(sdk_path)

from _atracsys_stk import *
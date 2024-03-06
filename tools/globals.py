ENGINE_NAME = "jage"
PROJECT_NAME = "jageeditor"
TOOLS_DIR = "tools"

V_MINOR = 1
V_MAJOR = 0

import sys, platform

PLATFORM = sys.platform

for x in platform.uname():
    if "microsoft" in x.lower():
        PLATFORM = "windows"
        break
      
def IsWindows():
    return PLATFORM == "windows"
  
def IsLinux():
    return PLATFORM == "linux"
  
def IsMac():
    return PLATFORM == "darwin"
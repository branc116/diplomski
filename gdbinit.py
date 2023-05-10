import gdb
import os

if not os.getcwd().endswith("/build/src"):
    gdb.execute("cd build/src")
if len(gdb.connections()) == 0:
    gdb.execute("target remote localhost:4242")

gdb.execute("delete")
gdb.execute("file stm32-bt.elf")
gdb.execute("load stm32-bt.elf")
gdb.execute("monitor reset")
gdb.execute("delete")
gdb.execute("break main")

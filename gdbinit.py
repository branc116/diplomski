import gdb
import os

if not os.getcwd().endswith("/build/src"):
    gdb.execute("cd build/src")
if len(gdb.connections()) == 0:
    gdb.execute("target extended-remote localhost:3333")
else:
    gdb.execute("disconnect")
    gdb.execute("target extended-remote localhost:3333")

def RESET():
    gdb.execute("monitor reset halt")
    pass
#    gdb.execute(f"monitor reset")
#    for i in range(6):
#        gdb.execute(f"monitor reset {i}")

gdb.execute("set confirm off")
gdb.execute("delete")
for i in range(2):
    RESET()
    gdb.execute("file stm32-bt.elf")
    gdb.execute("load stm32-bt.elf")
    RESET()
gdb.execute("break main")
#gdb.execute("break user_notify")
#gdb.execute("b EXTI9_5_IRQHandler")
#gdb.execute("b hci_tl.c:174")
gdb.execute("continue")

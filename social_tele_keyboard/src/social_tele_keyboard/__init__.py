from social_tele_keyboard import HookManager
import time
import sys

def main():
    hm = HookManager()
    hm.HookKeyboard()
    hm.HookMouse()
    hm.KeyDown = hm.keyPressed
    #hm.KeyUp = hm.printevent
    hm.MouseAllButtonsDown = hm.mouseClicked
    #hm.MouseAllButtonsUp = hm.printevent
    #hm.MouseMovement = hm.printevent
    hm.start()
    # time.sleep(10)
    # hm.cancel()


import threading
import time
import random

ui_Size = 100
i_Buffer1 = [0] * ui_Size
i_Buffer2 = [0] * ui_Size

tu_Mutex = threading.Lock()
tu_Semaphore1 = threading.Semaphore(0)
tu_Semaphore2 = threading.Semaphore(0)

def sr_Thread1():
    ui_J = 0
    ui_Loopcounter = 0

    while True:
        i_Array1 = [ui_Loopcounter] * 4

        with tu_Mutex:
            for ui_I in range(4):
                i_Buffer1[ui_J] = i_Array1[ui_I]
                ui_J += 1

        if ui_J >= ui_Size:
            ui_J = 0

        print(f"LoopCounter of thread A: {ui_Loopcounter}")
        ui_Loopcounter += 1

        time.sleep(0.25)
        tu_Semaphore1.release()

def sr_Thread2():
    ui_J = 0
    ui_Loopcounter = 0

    while True:
        tu_Semaphore1.acquire()

        with tu_Mutex:
            i_Array2 = [i_Buffer1[ui_J] for _ in range(4)]
            for ui_I in range(4):
                i_Buffer2[ui_J] = i_Array2[ui_I]
                ui_J += 1

        if ui_J >= ui_Size:
            ui_J = 0

        for ui_I in range(4):
            print(f"| Array from A to B: {i_Array2[ui_I]} |")

        print(f"LoopCounter of thread B: {ui_Loopcounter}")
        ui_Loopcounter += 1

        time.sleep(0.25)
        tu_Semaphore2.release()

def sr_Thread3():
    ui_J = 0
    ui_Loopcounter = 0

    while True:
        tu_Semaphore2.acquire()

        with tu_Mutex:
            i_Array3 = [i_Buffer2[ui_J] for _ in range(4)]
            ui_J += 1

        if ui_J >= ui_Size:
            ui_J = 0

        for ui_I in range(4):
            print(f"| Array from B to C: {i_Array3[ui_I]} |")

        print(f"LoopCounter of thread C: {ui_Loopcounter}")
        ui_Loopcounter += 1

        time.sleep(0.25)

if __name__ == "__main__":
    thread1 = threading.Thread(target=sr_Thread1)
    thread2 = threading.Thread(target=sr_Thread2)
    thread3 = threading.Thread(target=sr_Thread3)

    thread1.start()
    thread2.start()
    thread3.start()

    thread1.join()
    thread2.join()
    thread3.join()

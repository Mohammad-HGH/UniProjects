import tkinter as tk
from tkinter import Tk
from algorithms.FIFO import FIFO
from algorithms.LRU import LRU


class Input:
    def __init__(self):
        self.num_frames = None
        self.choice = None
        self.frames = None

    def get_frames(self):
        frame_root = Tk()
        frame_root.title('Input Panel')
        frame_root.geometry("400x300+540+210")
        tk.Label(frame_root, text="frame number").grid(row=0)
        tk.Label(frame_root, text="Second input").grid(row=1)

        text1 = tk.StringVar()
        text2 = tk.StringVar()

        self.num_frames = tk.Entry(frame_root, textvariable=text1, width=30)
        self.num_frames.grid(row=0, column=1)

        self.frames = tk.Entry(frame_root, textvariable=text2, width=30)
        self.frames.grid(row=1, column=1)

        tk.Button(frame_root, text='Enter', command=self.submit).grid(row=3,
                                                                      column=1,
                                                                      sticky=tk.W,
                                                                      pady=4)

        tk.Button(frame_root, text='Close', command=frame_root.destroy).grid(row=4,
                                                                             column=1,
                                                                             sticky=tk.W,
                                                                             pady=4)
        frame_root.mainloop()

    def chose_case(self):
        chose_root = Tk()
        chose_root.title('Chose Panel')
        chose_root.geometry("400x300+540+210")
        tk.Label(chose_root, text="what is ur choice?").grid(row=0)

        tk.Label(chose_root, text="1) FIFO").grid(row=1)
        tk.Label(chose_root, text="2) LRU").grid(row=2)
        text1 = tk.StringVar()

        self.choice = tk.Entry(chose_root, textvariable=text1, width=30)
        self.choice.grid(row=3, column=1)

        tk.Button(chose_root, text='Enter', command=self.submit_choice).grid(row=4, column=1, sticky=tk.W, pady=4)

        chose_root.mainloop()

    def submit(self):
        self.num_frames = int(self.num_frames.get())
        self.frames = list(map(int, self.frames.get().strip().split()))
        self.chose_case()

    def submit_choice(self):
        choice = int(self.choice.get())
        if choice == 1:
            print("FIFO results: ")
            objClass = FIFO(self.num_frames, self.frames, [], 0, 0, 'No')
            objClass.render_fifo()
        else:
            print("LRU results: ")
            objClass = LRU(self.num_frames, self.frames, [], [], 0, 'No')
            objClass.render_lru()

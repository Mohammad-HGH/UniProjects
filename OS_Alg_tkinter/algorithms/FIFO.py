# FIFO page replacement algorithm implementation in python
# Created By: Mohamad-HGH
from tkinter import Tk
from ui.Table import Table


class FIFO:
    def __init__(self, capacity, input_list, *other_inputs):
        self.capacity = capacity
        self.inputlist = input_list
        self.otherinputs = other_inputs

    def render_fifo(self):
        f, fault, top, pf = self.otherinputs
        print("\nString|Frame →\t", end='')
        for i in range(self.capacity):
            print(i, end=' ')  # print frame
        print("Fault\n   ↓\n")
        for i in self.inputlist:
            if i not in f:
                if len(f) < self.capacity:
                    f.append(i)
                else:
                    f[top] = i
                    top = (top + 1) % self.capacity
                fault += 1
                pf = 'Yes'
            else:
                pf = 'No'
            print("   %d\t\t" % i, end='')  # print refrence string
            for x in f:
                print(x, end=' ')  # print pages
            for x in range(self.capacity - len(f)):
                print(' ', end=' ')
            print(" %s" % pf)

        self.divert_to_table([
            ('Reference:', self.inputlist),
            ('Total Requests:', len(self.inputlist)),
            ('Total Page Faults:', fault),
            ('Fault Rate:', (fault / len(self.inputlist)) * 100),
        ])

    def divert_to_table(self, lst):
        tableObj = Table(lst)
        tableObj.render_table()

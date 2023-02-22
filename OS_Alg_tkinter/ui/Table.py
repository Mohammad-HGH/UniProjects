# Python program to create a table
from tkinter import *


class Table:
    def __init__(self, lst):
        self.e = None
        self.row = len(lst)
        self.col = len(lst[0])
        self.lst = lst

    def render_table(self):
        table_root = Tk()
        table_root.title('Hello Python')
        table_root.geometry("1050x400+140+210")
        # table_root.iconify('')

        for i in range(self.row):
            for j in range(self.col):
                self.e = Entry(table_root, width=25, fg='black', font=('Aria', 16, 'italic'))
                self.e.grid(row=i, column=j)
                self.e.insert(END, self.lst[i][j])
        table_root.mainloop()

# take the data
# lst = [
#     ('Input string:', 'Raj', 'Mumbai', 19),
#     ('Pages:', 'Aaryan', 'Pune', 18),
#     ('Total Requests:', 'Aaryan', 'Pune', 18),
#     ('Total page faults:', 'Vaishnavi', 'Mumbai', 20),
#     ('Fault rate:', 'Vaishnavi', 'Mumbai', 20),
# ]
# total_rows = len(lst)
# total_columns = len(lst[0])

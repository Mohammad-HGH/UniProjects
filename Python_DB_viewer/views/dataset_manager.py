import tkinter as tk
from tkinter import filedialog
from random import randrange
from termcolor import cprint
import webbrowser

class DatasetManager:

    def __init__(self, on_file_change=None, domain="http://127.0.0.1") -> None:
        self.on_file_change = on_file_change
        self.used_ports = []
        self.domain = domain
        self.app = tk.Tk()
        self.app.config(bg="white")
        self.app.title("Wordalizer")
        # self.app.minsize(640, 120)
        label = tk.Label(self.app, text="Select your database file:", font=("Calibari", 20), width=64, bg="white") \
            .grid(row=0, column=0, columnspan=10, padx=(10, 10), pady=(20, 20))
        self.lst_datasets = tk.Listbox(self.app, width=48, height=24)
        self.lst_datasets.grid(row=1, column=0, rowspan=9, columnspan=9, sticky='nwse', padx=(10,10), pady=(20,20))
        self.lst_datasets.bind("<<ListboxSelect>>", self.open_dashboard)
        self.list_scrollbar = tk.Scrollbar(self.app)
        self.lst_datasets.config(yscrollcommand=self.list_scrollbar.set)
        self.list_scrollbar.config(command=self.lst_datasets.yview)

        btn_submit = tk.Button(self.app, text="Open", command=self.open_database, font=("Calibari", 14), width=16, height=2) \
            .grid(row=7, column=9 , padx=(10, 10), pady=(20, 1))

        btn_x = tk.Button(self.app, text="Exit", command=self.app.destroy, font=("Calibari", 14), width=16, height=2) \
            .grid(row=8, column=9 , padx=(10, 10), pady=(20, 20))

        self.app.grid_rowconfigure(1, weight=1)
        self.app.grid_columnconfigure(0, weight=1)

    def show(self):
        self.app.mainloop()

    def open_database(self):
        try:
            database_file = filedialog.askopenfile(mode='r')
            short_filename = database_file.name.split('\\')[-1] if '\\' in database_file.name else database_file.name.split('/')[-1]
            if self.on_file_change:
                port = None
                while not port or port in self.used_ports:
                    port = randrange(1000, 65535)
                self.lst_datasets.insert(tk.END, f"{short_filename} @ {self.domain}:{port}")
                self.on_file_change(database_file.name, port, self.domain, short_filename)
                self.used_ports.append(port)
            else:
                self.lst_datasets.insert(tk.END, f"{short_filename} @ ???")
        except Exception as ex:
            cprint(f'something went wrong, maybe the port is in use! please try again... [exact cause: {ex.__str__()}]')
        return database_file

    def open_dashboard(self, event):
        selection = event.widget.curselection()
        if selection:
            index = selection[0]
            if index >= 0 and index < len(self.used_ports):
                port = self.used_ports[index]
                webbrowser.open(f"{self.domain}:{port}")

if __name__ == '__main__':
    DatasetManager().show()

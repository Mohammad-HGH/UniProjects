# Import Module
from bs4 import BeautifulSoup
import requests
from tkinter import *


class MyWindow:
    def __init__(self, win):
        # set labels
        self.lbl1 = Label(win, text='Address link')

        # set input forms
        self.t1 = Entry(bd=3)

        # set buttons
        self.btn1 = Button(win, text='Remove Ads')

        self.lbl1.place(x=100, y=50)
        self.t1.place(x=200, y=50)

        self.b1 = Button(win, text='Add', command=self.adsRemover)
        self.b1.place(x=100, y=150)

    def adsRemover(self):
        # Website URL
        URL = self.t1.get()

        # Page content from Website URL
        page = requests.get(URL).text

        soup = BeautifulSoup(page, 'html.parser')
        for s in soup.select('script'):
            s.extract()

        for s in soup.select('img'):
            s.extract()

        for s in soup.select('video'):
            s.extract()

        with open('readme.html', 'w', encoding='utf-8') as f:
            f.write(str(soup))


window = Tk()
mywin = MyWindow(window)
window.title('Hello Python')
window.geometry("400x300+10+10")
window.mainloop()

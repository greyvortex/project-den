from tkinter import *
from tkinter.messagebox import showinfo
from tkinter.filedialog import askopenfilename , asksaveasfilename
import os


root = Tk()

root.geometry('700x700')
root.title('Untitled-Notepad')


#Edit menu functions

def newfile():
    global file
    root.title('Untitled-Notepad')
    file = None
    text_area.delete(1.0,END)           

def Openfile():
    global file
    file = askopenfilename(defaultextension='.txt',
                           filetypes=[('All Files','*.*'),
                           ('Text documents',"*.txt")])
    if file == "":
                 file = None
    else:
        root.title(os.path.basename(file)+'_Notepad')
    text_area.delete(1.0,END)           
    f = open(file,'r')
    text_area.insert(1.0,f.read())        
    f.close()
        
                        

    
def savefile():
    global file
    if file == None:
        file = asksaveasfilename(initialfile='Untitled.txt',defaultextension='.txt',
                           filetypes=[('All Files','*.*'),
                           ('Text documents',"*.txt")])
    if file == '':
        file = None

    else:
        f = open(file,'w')
        f.write(text_area.get(1.0,END))
        f.close()

        root.title(os.path.basename(file) + '-Notepad')
        print('file saved')
        
def Exitfile():
        
    root.destroy()


#Edit menu functions

def Cutfile():
    text_area.event_generate(("<<Cut>>"))


def Copyfile():
    text_area.event_generate(("<<Copy>>"))


def Pastefile():
    text_area.event_generate(("<<Paste>>"))



#Edit menu functions

def about():
    showinfo('Notepad','Notepad By  Prakhar verma !!!! \nkuch padhai vadhai karo \nIAS YAS bano')


text_area = Text(root,font='lucida 13' ,)
scroll = Scrollbar(root)
scroll.pack(side =RIGHT, fill=Y)
text_area.pack(fill='both' , expand='true', side=LEFT)

scroll.config(command=text_area.yview)
text_area.config(yscrollcommand=scroll.set)

file = None

Menu_bar = Menu(root,)

file_menu = Menu(Menu_bar , tearoff=0)
file_menu.add_command(label='New' , command=newfile)
file_menu.add_command(label='Open' , command=Openfile)
file_menu.add_command(label='Save' , command=savefile)
file_menu.add_separator()
file_menu.add_command(label='Exit' , command=Exitfile)


Menu_bar.add_cascade(label = 'file',  menu = file_menu)

Edit_menu = Menu(Menu_bar ,  tearoff=0)
Edit_menu.add_command(label='Cut' , command=Cutfile)
Edit_menu.add_command(label='Copy' , command=Copyfile)
Edit_menu.add_command(label='Paste' , command=Pastefile)

Menu_bar.add_cascade(label = 'Edit',  menu = Edit_menu)

Help_menu = Menu(Menu_bar ,  tearoff=0)
Help_menu.add_command(label='About Notepad' , command = about)
Menu_bar.add_cascade(label = 'Help',  menu = Help_menu)


root.config(menu=Menu_bar)


root.mainloop()



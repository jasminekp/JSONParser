import tkinter as tk
from ctypes import *
from tkinter import ttk

# load the json DLL
lib = cdll.LoadLibrary('./json.dll') 
lib.getTeamInfo.argtypes = None
lib.getTeamInfo.restype = c_char_p



def clear_click_handler():
    tree.delete(*tree.get_children())


def load_click_handler():
    # get all information from the json DLL and decode 
    data = lib.getTeamInfo()
    all_stats = data.decode("latin1")
    
    #split all information by the comma delimiter
    formatted_stats = all_stats.split(",")
    team_stats = [formatted_stats[i:i+5] for i in range(0, len(formatted_stats), 5)]
   
    for item in team_stats:
        tree.insert("", tk.END, values=item)


if __name__ == "__main__":

    # create the main window
    window = tk.Tk()
    window.title("Welcome to the JSON Parser!")
    window.geometry("750x950") 

    # create a label widget
    label = tk.Label(window, text="Click the button below to load the NFL Team Stats:", font="Arial")
    label.size=25
    label.pack(pady = 10)

    # create a button widget with a button click event handler
    button = tk.Button(window, bg = 'white', text="Click to Load Table", command=load_click_handler, font = "Arial")
    button.pack()

    # create a tree (table) widget
    tree = ttk.Treeview(window, columns=("Team Name", "Team Code", "Win", "Loss", "Tie"), show="headings", height=35)
    tree.pack(padx=20, pady=20)
    for column in tree["columns"]:
        tree.column(column, width=125, anchor=tk.CENTER)
        tree.heading(column, text=column)


    # create label and button widgets to clear data
    label = tk.Label(window, text="Click the button below to clear the data.", font="Arial")
    label.size=25
    label.pack(pady = 10)
    
    button = tk.Button(window, bg = 'white', text="Click to Clear", command=clear_click_handler, font = "Arial")
    button.pack()

    # run the Tkinter event loop
    window.mainloop()

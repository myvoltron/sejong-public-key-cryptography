# key_expansion_page.py

import tkinter as tk
from tkinter import ttk

from helper import word_to_hex_string


class KeyExpansionPage(tk.Frame):
    def __init__(self, parent, controller, subkey_pages):
        super().__init__(parent)
        self.controller = controller
        self.subkey_pages = subkey_pages
        self.current_index = 0

        self.container = tk.Frame(self)
        self.container.pack(pady=10, fill="both", expand=True)

        for page in self.subkey_pages:
            page.place(in_=self.container, x=0, y=0, relwidth=1, relheight=1)

        self.navigation = tk.Frame(self)
        self.navigation.pack(pady=10)

        self.back_button = tk.Button(
            self.navigation, text="⬅ Back to Key HEX", command=self.back_to_key_hex
        )
        self.back_button.grid(row=0, column=0, padx=10)

        self.prev_button = tk.Button(self.navigation, text="⬅", command=self.prev_page)
        self.prev_button.grid(row=0, column=1, padx=10)

        self.page_label = tk.Label(self.navigation, text="SubKey 1/11")
        self.page_label.grid(row=0, column=2)

        self.next_button = tk.Button(self.navigation, text="➡", command=self.next_page)
        self.next_button.grid(row=0, column=3, padx=10)

        self.forward_button = tk.Button(
            self.navigation,
            text="Next → Message Hex",
            command=self.controller.continue_to_message_hex,
        )
        self.forward_button.grid(row=0, column=4, padx=10)

        self.after_idle(lambda: self.show_page(0))

    def back_to_key_hex(self):
        self.controller.show_frame("Step0_KeyToHex")

    def show_page(self, index):
        self.current_index = index
        self.page_label.config(text=f"SubKey {index + 1}/11")
        self.subkey_pages[index].tkraise()

    def next_page(self):
        if self.current_index < len(self.subkey_pages) - 1:
            self.show_page(self.current_index + 1)

    def prev_page(self):
        if self.current_index > 0:
            self.show_page(self.current_index - 1)


class SubKeyPage(tk.Frame):
    def __init__(self, parent, index, words, explanation):
        super().__init__(parent)
        self.index = index
        tk.Label(self, text=f"SubKey {index + 1}", font=("Helvetica", 16)).pack(pady=10)

        word_text = "\n".join(
            [f"W{index * 4 + i}: {word_to_hex_string(w)}" for i, w in enumerate(words)]
        )
        tk.Label(self, text=word_text, font=("Courier", 12), justify="left").pack(
            pady=5
        )

        explanation_box = tk.Text(self, wrap="word", width=80, height=10)
        explanation_box.pack(pady=10)
        explanation_box.insert("1.0", "\n".join(explanation))
        explanation_box.config(state="disabled")


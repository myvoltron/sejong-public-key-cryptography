import tkinter as tk
from tkinter import messagebox


class InputPage(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent)
        self.controller = controller

        tk.Label(
            self,
            text="Input 128-bit plaintext as a 16-digit lowercase hexadecimal string:",
        ).pack(pady=5)
        self.text_entry = tk.Entry(self, width=40)
        self.text_entry.pack(pady=5)

        tk.Label(
            self, text="Input 128-bit key as a 16-digit lowercase hexadecimal string:"
        ).pack(pady=5)
        self.key_entry = tk.Entry(self, width=40)
        self.key_entry.pack(pady=5)

        button_frame = tk.Frame(self)
        button_frame.pack(pady=20)

        tk.Button(button_frame, text="🔒 Encrypt", command=self.encrypt).grid(
            row=0, column=0, padx=10
        )
        tk.Button(button_frame, text="🔓 Decrypt", command=self.decrypt).grid(
            row=0, column=1, padx=10
        )

    def is_valid_lower_cha(self, s: str) -> bool:
        return len(s) == 16 and all(c.islower() and c.isalpha() for c in s)

    def is_valid_hex_string(self, s: str) -> bool:
        return len(s) == 32 and all(c in "0123456789abcdef" for c in s)

    def encrypt(self):
        text = self.text_entry.get()
        key = self.key_entry.get()
        if self.is_valid_lower_cha(text) and self.is_valid_lower_cha(key):
            self.controller.start_process(text, key, encrypt=True)
        else:
            messagebox.showerror(
                "Invalid Input",
                "Please enter 16-digit lowercase hexadecimal strings (0–9, a–f).",
            )

    def decrypt(self):
        text = self.text_entry.get()
        key = self.key_entry.get()
        if self.is_valid_hex_string(text) and self.is_valid_lower_cha(key):
            self.controller.start_process(text, key, encrypt=False)
        else:
            messagebox.showerror(
                "Invalid Input",
                "Enter valid ciphertext and lowercase key.",
            )

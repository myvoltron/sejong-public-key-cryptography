import tkinter as tk
from typing import List

from helper import hex_list_to_string


class KeyToHexPage(tk.Frame):
    def __init__(self, parent, controller, key: str, key_bytes: List[int]):
        super().__init__(parent)
        self.controller = controller
        self.key = key
        self.key_bytes = key_bytes

        tk.Label(self, text="Key to Hex Conversion", font=("Helvetica", 16)).pack(
            pady=10
        )

        self.add_conversion_block("Key")

        tk.Button(
            self,
            text="Next →",
            command=lambda: self.controller.continue_to_key_expansion(),
        ).pack(pady=20)

    def add_conversion_block(self, label: str):
        frame = tk.Frame(self)
        frame.pack(pady=10)

        tk.Label(frame, text=f"{label}: {self.key}", font=("Helvetica", 12)).pack(
            anchor="w"
        )

        tk.Label(
            frame,
            text=f"→ ASCII Hex: {hex_list_to_string(self.key_bytes)}",
            font=("Courier", 10),
            wraplength=700,
            justify="left",
        ).pack(anchor="w")

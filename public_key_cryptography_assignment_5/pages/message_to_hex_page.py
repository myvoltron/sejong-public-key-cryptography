# message_to_hex_page.py

import tkinter as tk
from typing import List

from helper import hex_list_to_string


class MessageToHexPage(tk.Frame):
    def __init__(self, parent, controller, text: str, message_bytes: List[int]):
        super().__init__(parent)
        self.controller = controller
        self.text = text
        self.message_bytes = message_bytes

        tk.Label(self, text="Message to Hex Conversion", font=("Helvetica", 16)).pack(
            pady=10
        )

        self.add_conversion_block("Message", text)

        tk.Button(
            self, text="Next →", command=self.controller.continue_to_round_viewer
        ).pack(pady=20)

    def add_conversion_block(self, label: str, input_str: str):
        frame = tk.Frame(self)
        frame.pack(pady=10)

        tk.Label(frame, text=f"{label}: {self.text}", font=("Helvetica", 12)).pack(
            anchor="w"
        )

        tk.Label(
            frame,
            text=f"→ ASCII Hex: {hex_list_to_string(self.message_bytes)}",
            font=("Courier", 10),
            wraplength=700,
            justify="left",
        ).pack(anchor="w")

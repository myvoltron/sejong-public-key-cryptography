# final_output_page.py

import tkinter as tk


class FinalOutputPage(tk.Frame):
    def __init__(self, parent, controller, ciphertext: str):
        super().__init__(parent)
        self.controller = controller

        tk.Label(self, text="Final Ciphertext Output", font=("Helvetica", 16)).pack(
            pady=10
        )

        output_box = tk.Text(self, height=4, font=("Courier", 14))
        output_box.insert("1.0", ciphertext)
        output_box.config(state="disabled")
        output_box.pack(pady=20, padx=40, fill="x")

        tk.Button(
            self,
            text="Back to Start",
            command=lambda: self.controller.show_frame("InputPage"),
        ).pack(pady=10)

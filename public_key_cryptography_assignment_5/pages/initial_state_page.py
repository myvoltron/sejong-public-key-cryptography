# initial_state_page.py

import tkinter as tk
from typing import List


class InitialStatePage(tk.Frame):
    def __init__(self, parent, controller, state_array: List[List[int]]):
        super().__init__(parent)
        self.controller = controller
        self.state_array = state_array

        tk.Label(self, text="Initial State Matrix", font=("Helvetica", 16)).pack(
            pady=10
        )

        # Convert state_array to hex string matrix
        matrix = [[f"{byte:02X}" for byte in row] for row in self.state_array]

        table_frame = tk.Frame(self)
        table_frame.pack(pady=10)

        for row in matrix:
            row_str = "   ".join(row)
            tk.Label(table_frame, text=row_str, font=("Courier", 14)).pack()

        tk.Button(
            self,
            text="Next → Round Step",
            command=self.controller.continue_to_round_viewer,
            font=("Helvetica", 12),
        ).pack(pady=20)


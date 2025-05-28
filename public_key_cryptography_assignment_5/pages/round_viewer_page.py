# round_viewer_page.py

import tkinter as tk


class RoundViewerPage(tk.Frame):
    def __init__(self, parent, controller, rounds_data):
        super().__init__(parent)
        self.controller = controller
        self.rounds_data = rounds_data
        self.pages = []
        self.current_index = 0

        self.container = tk.Frame(self)
        self.container.pack(pady=10, fill="both", expand=True)

        for i, round_data in enumerate(rounds_data):
            page = RoundPage(self.container, i + 1, round_data)
            page.place(in_=self.container, x=0, y=0, relwidth=1, relheight=1)
            self.pages.append(page)

        nav = tk.Frame(self)
        nav.pack(pady=10)

        self.back_button = tk.Button(
            nav, text="⬅ Back to Initial State", command=self.back_to_initial_state
        )
        self.back_button.grid(row=0, column=0, padx=10)

        self.prev_button = tk.Button(nav, text="⬅", command=self.prev_page)
        self.prev_button.grid(row=0, column=1, padx=10)

        self.page_label = tk.Label(nav, text="Round 1/10")
        self.page_label.grid(row=0, column=2)

        self.next_button = tk.Button(nav, text="➡", command=self.next_page)
        self.next_button.grid(row=0, column=3, padx=10)

        self.finish_button = tk.Button(
            nav,
            text="Next → Final Output",
            command=self.controller.continue_to_final_output,
        )
        self.finish_button.grid(row=0, column=4, padx=10)

        self.after_idle(lambda: self.show_page(0))

    def back_to_initial_state(self):
        self.controller.show_frame("Step3_InitialState")

    def show_page(self, index):
        self.current_index = index
        self.page_label.config(text=f"Round {index + 1}/10")
        self.pages[index].tkraise()

    def next_page(self):
        if self.current_index < len(self.pages) - 1:
            self.show_page(self.current_index + 1)

    def prev_page(self):
        if self.current_index > 0:
            self.show_page(self.current_index - 1)


class RoundPage(tk.Frame):
    def __init__(self, parent, round_number, data):
        super().__init__(parent)

        tk.Label(self, text=f"Round {round_number}", font=("Helvetica", 16)).pack(
            pady=10
        )

        tk.Label(
            self,
            text=f"Input State: {data['state_in']}",
            font=("Courier", 11),
            wraplength=700,
            justify="left",
        ).pack(anchor="w")
        tk.Label(
            self,
            text=f"SubKey: {data['subkey']}",
            font=("Courier", 11),
            wraplength=700,
            justify="left",
        ).pack(anchor="w")
        tk.Label(
            self,
            text=f"Byte Substitution: {data['byte_substitution']}",
            font=("Courier", 11),
            wraplength=700,
            justify="left",
        ).pack(anchor="w")
        tk.Label(
            self,
            text=f"Shift Rows: {data['shift_rows']}",
            font=("Courier", 11),
            wraplength=700,
            justify="left",
        ).pack(anchor="w")
        tk.Label(
            self,
            text=f"Mix Columns: {data['mix_columns']}",
            font=("Courier", 11),
            wraplength=700,
            justify="left",
        ).pack(anchor="w")
        tk.Label(
            self,
            text=f"Add Round Key: {data['round_result']}",
            font=("Courier", 11),
            wraplength=700,
            justify="left",
        ).pack(anchor="w")


# main.py

import tkinter as tk
from pages.input_page import InputPage
from pages.key_to_hex_page import KeyToHexPage
from pages.key_expansion_page import KeyExpansionPage, SubKeyPage
from pages.message_to_hex_page import MessageToHexPage
from pages.initial_state_page import InitialStatePage
from pages.round_viewer_page import RoundViewerPage
from pages.final_output_page import FinalOutputPage
from advanced_encryption_data import (
    aes_decrypt,
    get_state_array,
    key_expansion,
    aes_encrypt,
)
from helper import hex_string_to_int_list


class AppController:
    def __init__(self, root, container):
        self.root = root
        self.container = container
        self.frames = {}
        self.key_bytes = []
        self.message_bytes = []
        self.words = []
        self.word_logs = []
        self.rounds_data = []
        self.final_ciphertext = ""
        self.encrypt_mode = True
        self.input_text = ""
        self.input_key = ""

    def show_frame(self, name):
        frame = self.frames[name]
        frame.tkraise()

    def start_process(self, text: str, key: str, encrypt: bool):
        self.encrypt_mode = encrypt
        self.input_text = text
        self.input_key = key

        key_bytes = [int(f"{ord(c):02x}", 16) for c in self.input_key]
        words, logs = key_expansion(key_bytes)
        self.key_bytes = key_bytes
        self.words = words
        self.word_logs = logs

        if self.encrypt_mode == True:
            self.message_bytes = [int(f"{ord(c):02x}", 16) for c in self.input_text]
        else:
            self.message_bytes = hex_string_to_int_list(self.input_text)

        key_to_hex_page = KeyToHexPage(
            self.container, self, self.input_key, self.key_bytes
        )
        key_to_hex_page.grid(row=0, column=0, sticky="nsew")
        self.frames["Step0_KeyToHex"] = key_to_hex_page

        self.show_frame("Step0_KeyToHex")

    def continue_to_key_expansion(self):
        subkey_pages = [
            SubKeyPage(
                self.container,
                i,
                self.words[i * 4 : i * 4 + 4],
                self.word_logs[i * 4 : (i + 1) * 4],
            )
            for i in range(11)
        ]

        key_expansion_page = KeyExpansionPage(self.container, self, subkey_pages)
        key_expansion_page.grid(row=0, column=0, sticky="nsew")
        self.frames["Step1_KeyExpansion"] = key_expansion_page

        self.show_frame("Step1_KeyExpansion")

    def continue_to_message_hex(self):
        message_to_hex_page = MessageToHexPage(
            self.container, self, self.input_text, self.message_bytes
        )
        message_to_hex_page.grid(row=0, column=0, sticky="nsew")
        self.frames["Step2_MessageToHex"] = message_to_hex_page

        self.show_frame("Step2_MessageToHex")

    def continue_to_round_viewer(self):
        if self.encrypt_mode == True:
            rounds_data, ciphertext = aes_encrypt(self.message_bytes, self.words)
            self.rounds_data = rounds_data
            self.final_ciphertext = ciphertext
        else:
            rounds_data, ciphertext = aes_decrypt(self.message_bytes, self.words)
            self.rounds_data = rounds_data
            self.final_ciphertext = ciphertext

        viewer = RoundViewerPage(self.container, self, rounds_data)
        viewer.grid(row=0, column=0, sticky="nsew")
        self.frames["Step3_RoundViewer"] = viewer

        self.final_ciphertext = ciphertext

        self.show_frame("Step3_RoundViewer")

    def continue_to_final_output(self):
        if self.final_ciphertext is None:
            raise ValueError("Ciphertext not generated yet!")

        final_output_page = FinalOutputPage(
            self.container, self, ciphertext=self.final_ciphertext
        )
        final_output_page.grid(row=0, column=0, sticky="nsew")
        self.frames["Step4_FinalOutput"] = final_output_page

        self.show_frame("Step4_FinalOutput")


if __name__ == "__main__":
    root = tk.Tk()
    root.title("AES Visualizer")
    root.geometry("800x600")

    container = tk.Frame(root)
    container.pack(fill="both", expand=True)
    container.grid_rowconfigure(0, weight=1)
    container.grid_columnconfigure(0, weight=1)

    controller = AppController(root, container)

    input_page = InputPage(container, controller)
    input_page.grid(row=0, column=0, sticky="nsew")
    controller.frames["InputPage"] = input_page

    controller.show_frame("InputPage")

    root.mainloop()

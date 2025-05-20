import tkinter as tk
from data_encryption_standard import (
    generate_Cn_Dn,
    generate_round_keys,
    bitlist_to_str,
    apply_initial_permutation,
    apply_final_permutation,
)
from feistel import feistel_function


class DESApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("DES Visualization")
        self.geometry("700x700")
        self.frames = {}

        self.shared_key = ""
        self.shared_text = ""
        self.is_encrypt_mode = True
        self.Cn = []
        self.Dn = []
        self.round_keys = []
        self.L0 = []
        self.R0 = []
        self.L_list = []
        self.R_list = []
        self.result_text = ""

        container = tk.Frame(self)
        container.pack(fill="both", expand=True)

        for F in (
            InputPage,
            Step1_PC1,
            Step2_Shift,
            Step3_PC2,
            Step4_IP,
            Step5_Feistel,
            Step6_Final,
        ):
            frame = F(container, self)
            self.frames[F.__name__] = frame
            frame.grid(row=0, column=0, sticky="nsew")

        self.show_frame("InputPage")

    def show_frame(self, name):
        frame = self.frames[name]
        frame.tkraise()

    def start_process(self, text, key, encrypt=True):
        self.shared_text = text
        self.shared_key = key
        self.is_encrypt_mode = encrypt

        self.Cn, self.Dn = generate_Cn_Dn(key)
        self.round_keys = generate_round_keys(self.Cn, self.Dn)
        if not encrypt:
            self.round_keys = self.round_keys[::-1]  # reverse for decryption

        self.L0, self.R0 = apply_initial_permutation(text)
        self.L_list = [self.L0]
        self.R_list = [self.R0]

        for i in range(16):
            f_output = feistel_function(self.R_list[i], self.round_keys[i])
            new_L = self.R_list[i]
            new_R = [a ^ b for a, b in zip(self.L_list[i], f_output)]
            self.L_list.append(new_L)
            self.R_list.append(new_R)

        self.result_text = apply_final_permutation(self.L_list[-1], self.R_list[-1])

        self.frames["Step1_PC1"].update_display(self.Cn[0], self.Dn[0])
        self.frames["Step2_Shift"].update_display(self.Cn[1:], self.Dn[1:])
        self.frames["Step3_PC2"].update_display(self.round_keys)
        self.frames["Step4_IP"].update_display(
            bitlist_to_str(self.L0), bitlist_to_str(self.R0)
        )
        self.frames["Step5_Feistel"].update_display(self.L_list, self.R_list)
        self.frames["Step6_Final"].update_display(
            self.result_text, self.is_encrypt_mode
        )


class InputPage(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent)
        tk.Label(self, text="Enter 16-digit Hex Text:").pack()
        self.text_entry = tk.Entry(self, width=40)
        self.text_entry.pack()

        tk.Label(self, text="Enter 16-digit Hex Key:").pack()
        self.key_entry = tk.Entry(self, width=40)
        self.key_entry.pack()

        frame = tk.Frame(self)
        frame.pack(pady=20)
        tk.Button(frame, text="🔒 Encrypt", command=self.encrypt).grid(
            row=0, column=0, padx=10
        )
        tk.Button(frame, text="🔓 Decrypt", command=self.decrypt).grid(
            row=0, column=1, padx=10
        )

        self.controller = controller

    def encrypt(self):
        text = self.text_entry.get()
        key = self.key_entry.get()
        if len(text) == 16 and len(key) == 16:
            self.controller.start_process(text, key, encrypt=True)
            self.controller.show_frame("Step1_PC1")

    def decrypt(self):
        text = self.text_entry.get()
        key = self.key_entry.get()
        if len(text) == 16 and len(key) == 16:
            self.controller.start_process(text, key, encrypt=False)
            self.controller.show_frame("Step1_PC1")


class Step1_PC1(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent)
        tk.Label(self, text="🔹 Step 1: PC-1 Permutation → C0 and D0").pack(pady=10)
        self.result_text = tk.Text(self, height=10, width=80)
        self.result_text.pack()
        tk.Button(
            self,
            text="Next ➡ Step 2",
            command=lambda: controller.show_frame("Step2_Shift"),
        ).pack(pady=10)

    def update_display(self, C0, D0):
        self.result_text.delete(1.0, tk.END)
        self.result_text.insert(tk.END, f"C0: {bitlist_to_str(C0)}\n")
        self.result_text.insert(tk.END, f"D0: {bitlist_to_str(D0)}\n")


class Step2_Shift(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent)
        tk.Label(self, text="🔹 Step 2: Left Shifts → C1~C16, D1~D16").pack(pady=10)
        self.result_text = tk.Text(self, height=20, width=80)
        self.result_text.pack()
        tk.Button(
            self,
            text="Next ➡ Step 3",
            command=lambda: controller.show_frame("Step3_PC2"),
        ).pack(pady=10)

    def update_display(self, Cn_list, Dn_list):
        self.result_text.delete(1.0, tk.END)
        for i, (c, d) in enumerate(zip(Cn_list, Dn_list), 1):
            self.result_text.insert(tk.END, f"C{i}: {bitlist_to_str(c)}\n")
            self.result_text.insert(tk.END, f"D{i}: {bitlist_to_str(d)}\n\n")


class Step3_PC2(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent)
        tk.Label(self, text="🔹 Step 3: PC-2 Permutation → K1~K16").pack(pady=10)
        self.result_text = tk.Text(self, height=20, width=80)
        self.result_text.pack()
        tk.Button(
            self,
            text="Next ➡ Step 4 (IP)",
            command=lambda: controller.show_frame("Step4_IP"),
        ).pack(pady=10)

    def update_display(self, round_keys):
        self.result_text.delete(1.0, tk.END)
        for i, key in enumerate(round_keys, 1):
            self.result_text.insert(tk.END, f"K{i}: {bitlist_to_str(key)}\n")


class Step4_IP(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent)
        tk.Label(self, text="🔹 Step 4: Initial Permutation (IP) → L0 and R0").pack(
            pady=10
        )
        self.result_text = tk.Text(self, height=10, width=80)
        self.result_text.pack()
        tk.Button(
            self,
            text="Next ➡ Step 5 (Feistel)",
            command=lambda: controller.show_frame("Step5_Feistel"),
        ).pack(pady=10)

    def update_display(self, L0_text, R0_text):
        self.result_text.delete(1.0, tk.END)
        self.result_text.insert(tk.END, f"L0: {L0_text}\n")
        self.result_text.insert(tk.END, f"R0: {R0_text}\n")


class Step5_Feistel(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent)
        tk.Label(self, text="🔹 Step 5: Feistel Rounds").pack(pady=10)
        self.result_text = tk.Text(self, height=30, width=80)
        self.result_text.pack()
        tk.Button(
            self,
            text="Next ➡ Final Result",
            command=lambda: controller.show_frame("Step6_Final"),
        ).pack(pady=10)

    def update_display(self, L_list, R_list):
        self.result_text.delete(1.0, tk.END)
        for i in range(17):
            self.result_text.insert(tk.END, f"Round {i}:\n")
            self.result_text.insert(tk.END, f"L{i}: {bitlist_to_str(L_list[i])}\n")
            self.result_text.insert(tk.END, f"R{i}: {bitlist_to_str(R_list[i])}\n\n")


class Step6_Final(tk.Frame):
    def __init__(self, parent, controller):
        super().__init__(parent)
        self.title_label = tk.Label(self, text="", font=("Helvetica", 16))
        self.title_label.pack(pady=10)
        self.result_label = tk.Label(self, text="", font=("Courier", 18), fg="blue")
        self.result_label.pack(pady=20)
        tk.Button(
            self,
            text="⬅ Back to Start",
            command=lambda: controller.show_frame("InputPage"),
        ).pack(pady=10)
        tk.Button(self, text="Exit", command=controller.quit).pack()

    def update_display(self, result, is_encrypt):
        if is_encrypt:
            self.title_label.config(text="🔐 Encrypted Ciphertext")
        else:
            self.title_label.config(text="🔓 Decrypted Plaintext")
        self.result_label.config(text=result)


if __name__ == "__main__":
    app = DESApp()
    app.mainloop()

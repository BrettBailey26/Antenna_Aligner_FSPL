import tkinter as tk
from tkinter import messagebox
import math


def calculate_fspl(distance_m, frequency_hz):
    """Return FSPL in dB for given distance (m) and frequency (Hz)."""
    if distance_m <= 0 or frequency_hz <= 0:
        raise ValueError("Distance and frequency must be positive numbers")
    c = 299792458.0  # speed of light in m/s
    ratio = 4 * math.pi * distance_m * frequency_hz / c
    return 20 * math.log10(ratio)


class FSPLApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("FSPL Calculator")
        self.geometry("300x180")

        tk.Label(self, text="Distance (meters):").pack(pady=(10, 0))
        self.dist_entry = tk.Entry(self)
        self.dist_entry.pack(padx=10)

        tk.Label(self, text="Frequency (Hz):").pack(pady=(8, 0))
        self.freq_entry = tk.Entry(self)
        self.freq_entry.pack(padx=10)

        calc_btn = tk.Button(self, text="Calculate", command=self.on_calculate)
        calc_btn.pack(pady=12)

    def on_calculate(self):
        try:
            d = float(self.dist_entry.get())
            f = float(self.freq_entry.get())
            result = calculate_fspl(d, f)
            messagebox.showinfo("Result", f"Free-space path loss: {result:.2f} dB")
        except ValueError as e:
            messagebox.showerror("Input error", str(e))
        except Exception:
            messagebox.showerror("Error", "Please enter valid numbers.")


if __name__ == "__main__":
    app = FSPLApp()
    app.mainloop()

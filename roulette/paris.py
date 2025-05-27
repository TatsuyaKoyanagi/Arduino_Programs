import tkinter as tk
from tkinter import ttk
import random


class RouletteApp:
    def __init__(self, root):
        self.root = root
        self.root.title("ルーレット")
        self.root.geometry("300x400")

        self.choices = [
            "女子-57kg",
            "女子-70kg",
            "女子+70kg",
            "男子-73kg",
            "男子-90kg",
            "男子+90kg",
        ]
        self.fixed_choice = None
        self.labels = []
        self.create_widgets()
        self.running = False

    def create_widgets(self):
        self.frame = tk.Frame(self.root)
        self.frame.pack(expand=True)

        self.arrow_label = tk.Label(self.frame, text="→", font=("Helvetica", 24))
        self.arrow_label.grid(row=2, column=0, padx=(10, 0))  # 矢印を固定の位置に配置

        for i in range(len(self.choices)):
            label = tk.Label(self.frame, text="", font=("Helvetica", 18))
            label.grid(row=i, column=1, pady=5)
            self.labels.append(label)

        self.start_button = ttk.Button(
            self.root, text="スタート", command=self.start_roulette
        )
        self.start_button.pack(side="left", padx=10, pady=10)

        self.stop_button = ttk.Button(
            self.root, text="ストップ", command=self.stop_roulette, state="disabled"
        )
        self.stop_button.pack(side="right", padx=10, pady=10)

    def start_roulette(self):
        self.running = True
        self.start_button.config(state="disabled")
        self.stop_button.config(state="normal")
        self.scroll_labels()

    def scroll_labels(self):
        if self.running:
            self.choices = (
                self.choices[1:] + self.choices[:1]
            )  # リストのアイテムを回転させる
            for i, label in enumerate(self.labels):
                label.config(text=self.choices[i])

            self.scroll_animation = self.root.after(100, self.scroll_labels)

    def stop_roulette(self):
        if self.running:
            self.running = False
            if self.scroll_animation:
                self.root.after_cancel(self.scroll_animation)
            self.select_result()

    def select_result(self):
        if self.fixed_choice and self.fixed_choice in self.choices:
            # 指定された結果が出るようにリストを調整
            while self.labels[2].cget("text") != self.fixed_choice:
                self.choices = self.choices[1:] + self.choices[:1]
                for i, label in enumerate(self.labels):
                    label.config(text=self.choices[i])
        else:
            # ランダムな結果を表示
            random.shuffle(self.choices)
            for i, label in enumerate(self.labels):
                label.config(text=self.choices[i])

        self.start_button.config(state="normal")
        self.stop_button.config(state="disabled")


if __name__ == "__main__":
    root = tk.Tk()
    app = RouletteApp(root)

    # 固定したい結果を指定する（ランダムにしたい場合はNone）
    app.fixed_choice = "男子+90kg"  # 指定された結果を表示
    # app.fixed_choice = None  # ランダムな結果を表示

    root.mainloop()

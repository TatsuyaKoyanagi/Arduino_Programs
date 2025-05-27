import tkinter as tk
import random

# ルーレットの選択肢
weights = ["+90 kg", "+70 kg", "-70 kg"]
spinning = False  # ルーレットの回転フラグ
speed = 20  # 初期スクロール速度


def spin_roulette():
    """ルーレットを回転させる（縦スクロール風に動かす）"""
    global spinning, speed
    spinning = True
    speed = 20  # 速度リセット
    move_text()


def move_text():
    """縦方向にスクロールするアニメーション"""
    global spinning, speed
    if spinning:
        for item in text_items:
            canvas.move(item, 0, speed)  # すべてのテキストを下に移動
            y_position = canvas.coords(item)[1]  # 現在のy座標

            # 画面外に出たら上に戻し、ランダムな値に更新
            if y_position > 200:
                canvas.move(item, 0, -400)  # 上にリセット
                new_weight = random.choice(weights)
                canvas.itemconfig(item, text=new_weight)

        # 速度を徐々に落とす（減速アニメーション）
        if speed > 1:
            speed *= 0.95

        root.after(50, move_text)


def stop_roulette():
    """ルーレットを止めて「+90 kg」に固定"""
    global spinning
    spinning = False
    canvas.itemconfig(central_text, text="+90 kg")  # 真ん中の結果を固定
    for item in text_items:
        canvas.itemconfig(item, text="")  # 背景のスクロールを消す


# GUI作成
root = tk.Tk()
root.title("柔道ルーレット")

# Canvas作成（背景を青にして再現）
canvas = tk.Canvas(root, width=400, height=200, bg="blue")
canvas.pack(pady=20)

# 流れるテキスト（背景）
text_items = [
    canvas.create_text(
        200, 0, text="+70 kg", font=("Helvetica", 36), fill="white", anchor="center"
    ),
    canvas.create_text(
        200, 80, text="-70 kg", font=("Helvetica", 36), fill="white", anchor="center"
    ),
    canvas.create_text(
        200, 160, text="+90 kg", font=("Helvetica", 36), fill="white", anchor="center"
    ),
]

# 中央の固定テキスト
central_text = canvas.create_text(
    200, 100, text="スタート！", font=("Helvetica", 48, "bold"), fill="white"
)

# スタートボタン
start_button = tk.Button(
    root, text="ルーレットを回す", font=("Helvetica", 16), command=spin_roulette
)
start_button.pack(pady=10)

# ストップボタン
stop_button = tk.Button(
    root, text="ストップ", font=("Helvetica", 16), command=stop_roulette
)
stop_button.pack(pady=10)

# GUI起動
root.mainloop()

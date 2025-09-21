import logging
import multiprocessing
import os
import queue
import signal
import subprocess
import threading
import tkinter as tk
from logging.config import dictConfig
from tkinter import ttk

dictConfig({
    'version': 1,
    'disable_existing_loggers': False,
    'formatters': {
        'default': {
            'format': '[%(asctime)s] %(levelname)s: %(message)s',
        },
    },
    'handlers': {
        'console': {
            'class': 'logging.StreamHandler',
            'formatter': 'default',
        },
    },
    'root': {
        'handlers': ['console'],
        'level': 'INFO',
    },
})

logger = logging.getLogger(__name__)

# 実行ファイルのパス（必要に応じて修正）
CONTROLLER_PATH = '../Controller/build/dist/Controller'
SLIDECAR_PATH = '../SlideCar/build/dist/SlideCar'

STATE_LABELS = {0: '停止', 1: '左移動', 2: '右移動'}

# PyBullet物理シミュレーションプロセス


# サブプロセス用: PyBullet物理シミュレーション
def pybullet_worker(pipe):
    import time

    import pybullet as p
    import pybullet_data
    client = p.connect(p.GUI)
    p.setAdditionalSearchPath(pybullet_data.getDataPath())
    p.setGravity(0, 0, -9.8, physicsClientId=client)
    plane = p.loadURDF("plane.urdf", physicsClientId=client)
    car_id = p.loadURDF(
        "r2d2.urdf", [0, 0, 0.1], useFixedBase=False, globalScaling=0.5, physicsClientId=client
    )
    while True:
        if pipe.poll():
            msg = pipe.recv()
            if msg == 'exit':
                break
            # msgは位置(float)を想定
            try:
                x = float(msg)
                p.resetBasePositionAndOrientation(car_id, [x, 0, 0.1], [0, 0, 0, 1], physicsClientId=client)
            except Exception:
                pass
        p.stepSimulation(physicsClientId=client)
        time.sleep(1. / 240.)
    p.disconnect(physicsClientId=client)


class SlideCarGUI:
    def __init__(self, master):
        self.physics_pipe, bullet_pipe = multiprocessing.Pipe()
        self.bullet_proc = multiprocessing.Process(target=pybullet_worker, args=(bullet_pipe,), daemon=True)
        self.bullet_proc.start()
        self.master = master
        master.title('SlideCar Simulator')

        self.state_var = tk.StringVar(value='状態: 停止')
        self.slider_var = tk.IntVar(value=0)
        self.slider_label_var = tk.StringVar(value='位置: 0')

        self.label = ttk.Label(master, textvariable=self.state_var, font=("Arial", 16))
        self.label.pack(pady=10)

        self.slider = ttk.Scale(master, from_=-100, to=100, orient='horizontal', variable=self.slider_var, state='disabled', length=300)
        self.slider.pack(pady=10)

        self.slider_label = ttk.Label(master, textvariable=self.slider_label_var, font=("Arial", 12))
        self.slider_label.pack(pady=5)

        btn_frame = ttk.Frame(master)
        btn_frame.pack(pady=10)

        self.left_btn = ttk.Button(btn_frame, text='«', width=8)
        self.left_btn.grid(row=0, column=0, padx=10)
        self.right_btn = ttk.Button(btn_frame, text='»', width=8)
        self.right_btn.grid(row=0, column=1, padx=10)

        # ボタン押下/離すイベント
        self.left_btn.bind('<ButtonPress>', lambda e: self.send_controller('L'))
        self.left_btn.bind('<ButtonRelease>', lambda e: self.send_controller('l'))
        self.right_btn.bind('<ButtonPress>', lambda e: self.send_controller('R'))
        self.right_btn.bind('<ButtonRelease>', lambda e: self.send_controller('r'))

        # プロセス起動
        self.controller_proc = subprocess.Popen(
            [CONTROLLER_PATH], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, bufsize=0, text=True, start_new_session=True
        )
        self.slidecar_proc = subprocess.Popen(
            [SLIDECAR_PATH], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, bufsize=0, text=True, start_new_session=True
        )

        # キューとスレッド
        self.controller_to_slidecar_q = queue.Queue()
        self.slidecar_to_gui_q = queue.Queue()

        self.controller_thread = threading.Thread(target=self.controller_stdout_loop, daemon=True)
        self.slidecar_thread = threading.Thread(target=self.slidecar_stdout_loop, daemon=True)
        self.pipe_thread = threading.Thread(target=self.pipe_controller_to_slidecar, daemon=True)
        self.controller_thread.start()
        self.slidecar_thread.start()
        self.pipe_thread.start()

        self.master.after(100, self.update_gui_from_slidecar)

    def send_controller(self, msg):
        try:
            logger.debug(f'Sending to controller: {msg}')
            self.controller_proc.stdin.write(msg + '\n')
            self.controller_proc.stdin.flush()
        except Exception:
            logger.exception('Controller stdin error')

    def controller_stdout_loop(self):
        while True:
            line = self.controller_proc.stdout.readline()
            if not line:
                break
            logger.debug(f'Received from controller: {line.strip()}')
            self.controller_to_slidecar_q.put(line)

    def pipe_controller_to_slidecar(self):
        while True:
            line = self.controller_to_slidecar_q.get()
            logger.debug(f'Sending to SlideCar: {line.strip()}')
            try:
                self.slidecar_proc.stdin.write(line)
                self.slidecar_proc.stdin.flush()
            except Exception:
                logger.exception('SlideCar stdin error')

    def slidecar_stdout_loop(self):
        while True:
            line = self.slidecar_proc.stdout.readline()
            if not line:
                break
            logger.debug(f'Received from SlideCar: {line.strip()}')
            self.slidecar_to_gui_q.put(line)

    def update_gui_from_slidecar(self):
        while not self.slidecar_to_gui_q.empty():
            line = self.slidecar_to_gui_q.get()
            logger.debug(f'Updating GUI from SlideCar: {line.strip()}')
            try:
                state, steps, total_steps = line.strip().split()
                state = int(state)
                total_steps = int(total_steps)
                self.state_var.set(f'状態: {STATE_LABELS.get(state, "?")}')
                self.slider_var.set(total_steps)
                self.slider_label_var.set(f'位置: {total_steps}')
                # PyBullet物理シミュレーションの位置も更新（サブプロセスに送信）
                x = int(total_steps) * 0.01  # [m]
                logger.debug(f'物理シミュ送信: {x}')
                try:
                    self.physics_pipe.send(x)
                except Exception:
                    logger.exception('物理シミュ送信エラー')
            except Exception:
                logger.exception(f'Parse error, line={line}')
        self.master.after(100, self.update_gui_from_slidecar)

    def on_close(self):
        try:
            os.killpg(os.getpgid(self.controller_proc.pid), signal.SIGINT)
        except Exception:
            pass
        try:
            os.killpg(os.getpgid(self.slidecar_proc.pid), signal.SIGINT)
        except Exception:
            pass
        try:
            self.physics_pipe.send('exit')
        except Exception:
            pass
        try:
            self.bullet_proc.terminate()
        except Exception:
            pass
        self.master.destroy()
        os._exit(0)


def main():
    root = tk.Tk()
    app = SlideCarGUI(root)
    root.protocol("WM_DELETE_WINDOW", app.on_close)
    root.mainloop()


if __name__ == '__main__':
    main()

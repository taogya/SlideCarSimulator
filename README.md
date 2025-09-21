# SlideCarController
StateSmithを使用して設計した左右に動くおもちゃ

![demo](/resources/demo.gif)

## 構成
```
[Controller] -Command-> [Slide Car]
```

## 仕様
### Controller
- Slide CarへCommandを送信する。
- 左ボタン・右ボタンを持つ
- 各ボタンは初期状態で解放されている
- 左ボタン押下中は、100msごとにLeft Move CommandをSlide Carへ送信する
- 右ボタン押下中は、100msごとにRight Move CommandをSlide Carへ送信する
- ボタンを解放した場合、Stop CommandをSlide Carへ送信する
- ボタン押下の判定はチャタリング抑制のため、1ms周期でポーリングし、同一状態が5回連続した場合に確定する
- ボタン解放の判定は、同一状態が10回連続した場合か他のボタンが確定した場合とする
- ボタンを同時に押下した場合、先に押下確定したボタンを優先し、後から押下されたボタンは無効とする

### Slide Car
- 受信したCommandを処理する
- デフォルト動作は停止である
- Stop Commandを受け取ると停止する
- Left Move Commandを受け取ると左へ1ステップ移動する
- Right Move Commandを受け取ると右へ1ステップ移動する
- 各方向のステップ数には上限があり、上限に達した場合はMove Commandを破棄する

### Command
- Stop Command：停止コマンド
- Left Move Command：左移動コマンド。移動単位は「ステップ」とし、距離や時間は未規定
- Right Move Command：右移動コマンド。移動単位は「ステップ」とし、距離や時間は未規定

## StateSmith
### Controller
- Buttonのチャタリング除去
![Controller_Button](/Controller/out/src/Button/private/Button/Button.png)
- コマンドの定期送信
![Controller_CommandSender](/Controller/out/src/CommandSender/private/CommandSender/CommandSender.png)

### SlideCar
- モーター制御
![SlideCar_Motor](/SlideCar/out/src/Motor/private/Motor/Motor.png)


## 開発環境準備
```
$ git clone --recursive https://github.com/taogya/SlideCarSimulator.git
$ gcc --version
Apple clang version 16.0.0 (clang-1600.0.26.6)
Target: arm64-apple-darwin24.6.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin

$ cmake --version
cmake version 4.0.3

$ python3 -V
Python 3.13.5

$ python3 -m venv venv
$ . venv/bin/activate
$ pip install -r requirements.txt
```

## ビルド
### Command
```
$ pushd .
$ cd Protobuf
$ python nanopb/generator/nanopb_generator.py -D public SlideCarCommand.proto
$ popd
$ deactivate
```

### StateSmith
```
ss.cli run -hr
```

### Controller
```
$ pushd .
$ cd Controller
$ mkdir build
$ cd build
$ cmake ../src
$ make

# test
$ ./dist/Controller

## enter `L` key -> send LeftMoveCommand
## enter `R` key -> send RightMoveCommand
## enter any key -> send StopCommand

Ctrl+C

$ popd
```

### SlideCar
```
$ pushd .
$ cd SlideCar
$ mkdir build
$ cd build
$ cmake ../src
$ make

# test
$ ./dist/SlideCar

## STOP  : 08 01
## LEFT  : 08 01 10 01 18 01
## RIGHT : 08 01 10 02 18 01

Ctrl+C

$ popd
```

## Simulator動作仕様
### 動作仕様
- アプリ起動時にController用とSlideCar用のプロセスを立ち上げる。
- それぞれにPIPEを使用して、標準入力・標準出力を制御する。
- Controllerの標準入力はGUIから通知する。
- Controllerの標準出力はSlideCarの標準入力に渡す。
- SlideCarの標準出力はGUIが受け取る。
- GUIの制御情報はPyBulletの物理シミュレーションにも渡す。
- 物理シミュレーションに制御情報を渡す際に、ステップ値→移動距離に換算してから渡すこと。（1ステップ0.01m）

### GUI仕様
- 右（->）左（<-）ボタンとスライドバー、制御ラベルがある。
- 左ボタンを押すと、ControllerにLを標準入力に通知する。
- 左ボタンを話すと、Controllerにlを標準入力に通知する。
- 右ボタンを押すと、ControllerにRを標準入力に通知する。
- 右ボタンを話すと、Controllerにrを標準入力に通知する。
- SlideCarから標準出力を受け取りスライドバー、制御ラベルに反映させる。
- SlideCarからの標準出力は以下の文字列である。
    ```
    　  <state> <steps> <total_steps>
    ```
- state 0なら停止、1なら左移動、2なら右移動の文字を制御ラベルに反映。
- steps 使用しない
- total_steps スライドバーの現在位置とする。範囲は-100~100である。右移動は+、左移動は-である。
- toal_stepsは位置ラベルに表示する。

## Simulator実行
```
$ cd Simulator
$ python main.py
```

## 備考
- c_cpp_properties.json browse.path/includePathに記載するディレクトリの抽出
    ```
    $ find . \( -iname "*.h" -o -iname "*.hpp" \) -type f \
    | grep -vE "/build/" \
    | xargs -I{} dirname {} \
    | sort -u \
    | sed 's|^\./|"${workspaceFolder}/|;s|$|",|'
    ```

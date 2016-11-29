# Naming convention of TURIPLiblariesForArduino

本ドキュメントは本ライブラリの命名規則を定義します。

This document defines naming convention of this liblaries.

## 型名

型名は、先頭にスネークケースで記述します。当該変数・クラスがグローバルな場合、
先頭文字は大文字、ローカルの場合は小文字とします。

ただし、ユーザーが利用するクラス・変数については型名表記は省略します。

### 型名表記の一覧

 | 型                    | 表記      |
 |:---------------------|:---------|
 |ブール型(boolean)       | b, B     |
 |整数型(int, long)       | i, I     |
 |小数点型(float, double) | f, F     |
 |文字型(char, string)    | s, S     |
 |クラス(class)           | c, C     |
 |構造体(struct)          | st, St   |
 |列挙体(enum)            | en, En   |
 |共用体(union)           | un, Un   |

### 例

* グローバルなクラス: `C_example`
* ローカルな整数型変数: `i_number`
* 外部に公開するクラス: `example2`

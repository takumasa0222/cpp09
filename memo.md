## std::pair ##
2 つの値をひとまとめにして保持するクラス。
std::map は同じキーを複数持ていないため、insert を実施したあとに以下の情報を返す
first: 挿入された要素、またはすでに存在していた要素のイテレータ
second: 新しく挿入できた場合は true キーがすでに存在した場合は false
コンテナではない

## std::string::npos ##
string::find で値が見つからなかった場合に返す値として定義されている。

## std::lower_bound ##
x  を右辺とする  <  演算が成り立たない最初の要素を指すイテレータを返す
-> x に与えた値以上の数値を返す。
```cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    // 1. Map という名のプレイリストを作成
    std::map<int, std::string> playlist = {
        {10, "Classic Soul"},
        {20, "Funky Disco"},
        {30, "Electro House"},
        {40, "Chill Beats"}
    };

    int target_key = 25; // 25番の曲を探したいが、リストにない
    // 2. lower_bound 発動！ 25「以上」の最初の要素を探す
    auto it = playlist.lower_bound(target_key);
    std::cout << "--- Searching for Key 25 ---" << std::endl;
    // 3. 安全チェック（end() かどうかを確認）
    if (it != playlist.end()) {
        // ここがポイント：25はないので、25より大きい「30」がヒットする
        std::cout << "Found element: [" << it->first << "] -> " << it->second << std::endl;
    } else {
        std::cout << "Nothing found... the party is over!" << std::endl;
    }
    return 0;
}
```

## 逆ポーランド記法 (Reverse Polish Notation) ##
演算子を数字の後ろに書く計算式の表現方法
通常の式では
```
3 + 4
```
と記載するが、逆ポーランド記法では
```
3 4 +
```
と記載する。
その他の例
例 1 
```
(3 + 4) × 2
```
逆ポーランド記法
```
3 4 + 2 *
```
例 2
```
3 + (4 × 2)
```
逆ポーランド記法
```
3 4 2 * + 
```
基本ルールとして下記が成り立つ
1. 数字が来たらスタックに入れる
2. 演算子が来たら数字を2つ取り出す
3. 計算結果をスタックに戻す
4. 最後に残った値が答え
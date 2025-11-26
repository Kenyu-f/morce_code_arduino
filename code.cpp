// !!! 修正点: ピン名を A0 から LED0 などに置き換えます !!!

// 接続したLEDのピン番号 (LED0を使用)
const int LED_PIN = LED0; 

// LED0からLED3の4つのピンをエラー時に光らせるための配列
// 動作確認が取れている定数名を使用します。
const int ERROR_PINS[] = {LED0, LED1, LED2, LED3};
const int NUM_ERROR_PINS = 4; // 使用するエラーピンの数

// モールス符号の基本となる時間単位 (dotの時間 in milliseconds)
const int UNIT_TIME = 100; 

// --- モールス符号のパターン定義 (省略なし) ---
const char* MORSE_CODE[] = {
    ".-",   // A
    "-...", // B
    "-.-.", // C
    "-..",  // D
    ".",    // E
    "..-.", // F
    "--.",  // G
    "....", // H
    "..",   // I
    ".---", // J
    "-.-",  // K
    ".-..", // L
    "--",   // M
    "-.",   // N
    "---",  // O
    ".--.", // P
    "--.-", // Q
    ".-.",  // R
    "...",  // S
    "-",    // T
    "..-",  // U
    "...-", // V
    ".--",  // W
    "-..-", // X
    "-.--", // Y
    "--..", // Z
    "-----", // 0
    ".----", // 1
    "..---", // 2
    "...--", // 3
    "....-", // 4
    ".....", // 5
    "-....", // 6
    "--...", // 7
    "---..", // 8
    "----."  // 9
};

// ... (MORSE_CODE配列の定義は変更なし) ...

// --- 関数定義 ---
// flashSignal, sendMorseChar, errorFlash の中身は変更なし。
// 定義されているLED_PINやERROR_PINSを使っているため、置き換えが反映されます。

/**
 * モールス信号の単一の要素 (ドットまたはダッシュ) を生成します。
 */
void flashSignal(int duration) {
    digitalWrite(LED_PIN, HIGH); 
    delay(duration);             
    digitalWrite(LED_PIN, LOW);  
    delay(UNIT_TIME);            
}

/**
 * 1文字分のモールス符号を解釈し、LEDを点滅させます。
 */
void sendMorseChar(const char* morsePattern) {
    for (int i = 0; morsePattern[i] != '\0'; i++) {
        char element = morsePattern[i];
        if (element == '.') {
            flashSignal(UNIT_TIME);
        } else if (element == '-') {
            flashSignal(UNIT_TIME * 3);
        }
    }
    delay(UNIT_TIME * 2); 
}

/**
 * エラー発生時にすべてのライト (LED0からLED3の4つ) を光らせる (3回点滅)
 */
void errorFlash() {
    Serial.println("!! Error Flash Activated !!");
    for (int i = 0; i < 3; i++) {
        // すべての指定ピンを点灯
        for (int j = 0; j < NUM_ERROR_PINS; j++) {
            digitalWrite(ERROR_PINS[j], HIGH); 
        }
        delay(UNIT_TIME * 5); // 5 UNIT_TIME 点灯

        // すべての指定ピンを消灯
        for (int j = 0; j < NUM_ERROR_PINS; j++) {
            digitalWrite(ERROR_PINS[j], LOW);
        }
        delay(UNIT_TIME * 5); // 5 UNIT_TIME 消灯
    }
}

// --- setup と loop ---

void setup() {
    // モールス信号用のピンを設定
    pinMode(LED_PIN, OUTPUT);
    
    // エラー点滅用のピンも出力に設定
    for (int i = 0; i < NUM_ERROR_PINS; i++) {
        pinMode(ERROR_PINS[i], OUTPUT);
    }
    
    Serial.begin(9600);
    Serial.println("Morse Code Generator Ready (using pin LED0 for signal). Enter text:");
}

void loop() {
    if (Serial.available()) {
        char incomingChar = Serial.read();

        // 文字種を判別し、モールス信号を送信
        if (incomingChar >= 'a' && incomingChar <= 'z') {
            int index = incomingChar - 'a';
            Serial.print(incomingChar);
            Serial.print(" -> ");
            Serial.println(MORSE_CODE[index]);
            sendMorseChar(MORSE_CODE[index]);
            
        } else if (incomingChar >= 'A' && incomingChar <= 'Z') {
            int index = incomingChar - 'A';
            Serial.print(incomingChar);
            Serial.print(" -> ");
            Serial.println(MORSE_CODE[index]);
            sendMorseChar(MORSE_CODE[index]);
            
        } else if (incomingChar >= '0' && incomingChar <= '9') {
            int index = incomingChar - '0' + 26;
            Serial.print(incomingChar);
            Serial.print(" -> ");
            Serial.println(MORSE_CODE[index]);
            sendMorseChar(MORSE_CODE[index]);
            
        } else if (incomingChar == ' ') {
            Serial.println("[Word Space]");
            delay(UNIT_TIME * 4); 
            
        } else if (incomingChar == '\n' || incomingChar == '\r') {
            // 改行やキャリッジリターンは無視
            
        } else {
            // 予期しない文字 (ひらがななど) が検出された場合、エラー処理を実行
            Serial.print("!! Error: Unexpected character '");
            Serial.print(incomingChar);
            Serial.println("' detected. !!");
            errorFlash();
        }
    }
}
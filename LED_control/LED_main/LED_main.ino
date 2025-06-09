#include <Adafruit_NeoPixel.h>

#define PIN1 6
#define PIN2 7
#define PIN3 8
#define PIN4 9
#define PIN5 10
#define PIN6 11
#define PIN7 12

#define SummerButtonPin 20
#define WinterButtonPin 21

#define SummerLEDPin 22
#define WinterLEDPin 23

#define NUMPIXELS_A 262
#define NUMPIXELS_B 98
#define NUMPIXELS_C1 215
#define NUMPIXELS_C2 77
#define NUMPIXELS_D 70
#define NUMPIXELS_E 50
#define NUMPIXELS_F 0

#define NumStrips 7

boolean buttonS = false;
boolean buttonW = false;

const unsigned long keep = 180000UL; // 遅延時間（3分）

#define PUSH_SHORT 100

/***パートAに関する変数定義***/
// 光の塊の数の変数
const int numOfRed1Blobs_A = 8;
const int numOfRed2Blobs_A = 5;
const int numOfBlueBlobs_A = 4;
const int numOfBlue2Blobs_A = 17;
// 各塊の開始と終了位置を格納する配列
int redStart1_A[numOfRed1Blobs_A];
int redEnd1_A[numOfRed1Blobs_A];
int redStart2_A[numOfRed2Blobs_A];
int redEnd2_A[numOfRed2Blobs_A];
int blueStart_A[numOfBlueBlobs_A];
int blueEnd_A[numOfBlueBlobs_A];
int blueStart2_A[numOfBlue2Blobs_A];
int blueEnd2_A[numOfBlue2Blobs_A];
// 光の塊間の間隔の変数
const int spacingBetweenBlobs_A = 5; // 各塊の間隔を5LEDに設定
// その他の必要なグローバル変数
const int blobSize_A = 2; // 各塊の大きさ（LEDの数）

volatile bool summerPressed = false;
volatile bool winterPressed = false;
volatile unsigned long lastInterruptTime = 0; // 最後の割り込み時刻を記録
const unsigned long debounceTime = 200;       // デバウンス時間（ミリ秒）

/***パートBに関する変数定義***/
// B光の塊の数の変数
const int numOfRed1Blobs_B = 11;
const int numOfBlue1Blobs_B = 1;
const int numOfRed2Blobs_B = 1;
const int numOfBlue2Blobs_B = 1;
// 各塊の開始と終了位置を格納する配列
int redStart1_B[numOfRed1Blobs_B];
int redEnd1_B[numOfRed1Blobs_B];
int redStart2_B[numOfRed2Blobs_B];
int redEnd2_B[numOfRed2Blobs_B];
int blueStart1_B[numOfBlue1Blobs_B];
int blueEnd1_B[numOfBlue1Blobs_B];
int blueStart2_B[numOfBlue2Blobs_B];
int blueEnd2_B[numOfBlue2Blobs_B];
// 光の塊の間隔
const int spacingBetweenBlobs_B = 5; // 各塊の間隔を5LEDに設定
// 各塊の大きさ（LEDの数）
const int blobSize_B = 2;

/***パートC1に関する変数定義***/
// 光の塊の数の変数
const int numOfRed1Blobs_C1 = 4;
const int numOfBlueBlobs_C1 = 3;
// 各塊の開始と終了位置を格納する配列
int redStart1_C1[numOfRed1Blobs_C1];
int redEnd1_C1[numOfRed1Blobs_C1];
int blueStart_C1[numOfBlueBlobs_C1];
int blueEnd_C1[numOfBlueBlobs_C1];
// 光の塊間の間隔の変数
const int spacingBetweenRedBlobs_C1 = 5;  // 各赤塊の間隔を4LEDに設定
const int spacingBetweenBlueBlobs_C1 = 5; // 各青塊の間隔を5LEDに設定
// その他の必要なグローバル変数
const int blobSize_C1 = 2; // 各塊の大きさ（LEDの数）

/****C2に関する変数定義**/
// 光の塊の数の変数
const int numOfRed1Blobs_C2 = 5;
const int numOfBlueBlobs_C2 = 6;
// 各塊の開始と終了位置を格納する配列
int redStart1_C2[numOfRed1Blobs_C2];
int redEnd1_C2[numOfRed1Blobs_C2];
int blueStart_C2[numOfBlueBlobs_C2];
int blueEnd_C2[numOfBlueBlobs_C2];
// 光の塊間の間隔の変数
const int spacingBetweenBlobs_C2 = 5; // 各塊の間隔を5LEDに設定
// その他の必要なグローバル変数
const int blobSize_C2 = 2; // 各塊の大きさ（LEDの数）

/****Dに関する変数定義**/
const int numOfRedBlobs_D = 5;
const int numOfBlueBlobs_D = 7;
const int numOfPurpleBlobs_D = 1;

int redStart_D[numOfRedBlobs_D];
int redEnd_D[numOfRedBlobs_D];
int blueStart_D[numOfBlueBlobs_D];
int blueEnd_D[numOfBlueBlobs_D];

int purpleStart1_D[numOfPurpleBlobs_D];
int purpleEnd1_D[numOfPurpleBlobs_D];
int purpleStart2_D[numOfPurpleBlobs_D];
int purpleEnd2_D[numOfPurpleBlobs_D];

const int spacingBetweenBlobs_D = 3; // 各塊の間隔を5LEDに設定

const int blobSize_D = 2; // 各塊の大きさ（LEDの数）

// BLUE(0,0,255) RED(255,0,0) SKYBLUE(150,200,240) ORANGE(255,80,0)
int season = 0;

unsigned long previousMillis = 0;
const int intervalA = 15000;
const int intervalB = 10000;
const int intervalC = 15000;
const int intervalD = 10000;
const int delayTime = 3000;

bool ledOff = false;
// const int intervalE;

// Aパート
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUMPIXELS_A, PIN1, NEO_GRB + NEO_KHZ800);
// Bパート
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUMPIXELS_B, PIN2, NEO_GRB + NEO_KHZ800);
// C1パート
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUMPIXELS_C1, PIN3, NEO_GRB + NEO_KHZ800);
// C2パート
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(NUMPIXELS_C2, PIN4, NEO_GRB + NEO_KHZ800);
// Dパート
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(NUMPIXELS_D, PIN5, NEO_GRB + NEO_KHZ800);
// Eパート
Adafruit_NeoPixel strip6 = Adafruit_NeoPixel(NUMPIXELS_E, PIN6, NEO_GRB + NEO_KHZ800);
// Fパート
Adafruit_NeoPixel strip7 = Adafruit_NeoPixel(NUMPIXELS_F, PIN7, NEO_GRB + NEO_KHZ800);

int pins[NumStrips] = {PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7};
int LEDs[NumStrips] = {NUMPIXELS_A, NUMPIXELS_B, NUMPIXELS_C1, NUMPIXELS_C2, NUMPIXELS_D, NUMPIXELS_E, NUMPIXELS_F};
Adafruit_NeoPixel strips[NumStrips];

void setup()
{
    pinMode(SummerButtonPin, INPUT_PULLUP);
    pinMode(WinterButtonPin, INPUT_PULLUP);
    pinMode(pins[NumStrips], OUTPUT);
    pinMode(SummerLEDPin, OUTPUT);
    pinMode(WinterLEDPin, OUTPUT);
    Serial.begin(9600);

    // if (buttonS || buttonW) {
    attachInterrupt(digitalPinToInterrupt(SummerButtonPin), Summer, RISING);
    attachInterrupt(digitalPinToInterrupt(WinterButtonPin), Winter, RISING);
    //}

    for (int i = 0; i < NumStrips; i++)
    {
        strips[i] = Adafruit_NeoPixel(LEDs[i], pins[i], NEO_RGB + NEO_KHZ800);
        strips[i].begin();
        strips[i].clear();
        strips[i].show();
    }
    // 各塊の開始と終了位置を設定するための配列を初期化
    for (int i = 0; i < numOfRed1Blobs_A; i++)
    {
        redStart1_A[i] = i * (blobSize_A + spacingBetweenBlobs_A);
        redEnd1_A[i] = redStart1_A[i] + blobSize_A - 1;
    }
    for (int i = 0; i < numOfRed2Blobs_A; i++)
    {
        redStart2_A[i] = 77 + i * (blobSize_A + spacingBetweenBlobs_A);
        redEnd2_A[i] = redStart2_A[i] + blobSize_A - 1;
    }
    // 各塊の開始と終了位置を設定するための配列を初期化
    for (int i = 0; i < numOfBlueBlobs_A; i++)
    {
        blueStart_A[i] = 134 - i * (blobSize_A + spacingBetweenBlobs_A);
        blueEnd_A[i] = blueStart_A[i] + blobSize_A - 1;
    }
    for (int i = 0; i < numOfBlue2Blobs_A; i++)
    {
        blueStart2_A[i] = 133 + i * (blobSize_A + spacingBetweenBlobs_A);
        blueEnd2_A[i] = blueStart2_A[i] + blobSize_A - 1;
    }
    // パートB赤 各塊の開始と終了位置を設定するための配列を初期化
    for (int i = 0; i < numOfRed1Blobs_B; i++)
    {
        redStart1_B[i] = 77 - i * (blobSize_B + spacingBetweenBlobs_B);
        redEnd1_B[i] = redStart1_B[i] + blobSize_B - 1;
    }
    // パートB赤 各塊の開始と終了位置を設定するための配列を初期化
    for (int i = 0; i < numOfRed2Blobs_B; i++)
    {
        redStart2_B[i] = 84 - i * (blobSize_B + spacingBetweenBlobs_B);
        redEnd2_B[i] = redStart2_B[i] + blobSize_B - 1;
    }
    // パートB青 各塊の開始と終了位置を設定するための配列を初期化
    for (int i = 0; i < numOfBlue1Blobs_B; i++)
    {
        blueStart1_B[i] = 91 - i * (blobSize_B + spacingBetweenBlobs_B);
        blueEnd1_B[i] = blueStart1_B[i] + blobSize_B - 1;
    }
    // パートB青 各塊の開始と終了位置を設定するための配列を初期化
    for (int i = 0; i < numOfBlue1Blobs_B; i++)
    {
        blueStart2_B[i] = 98 - i * (blobSize_B + spacingBetweenBlobs_B);
        blueEnd2_B[i] = blueStart2_B[i] + blobSize_B - 1;
    }
    // パートC1赤 各塊の開始と終了位置を設定するための配列を初期化
    for (int i = 0; i < numOfRed1Blobs_C1; i++)
    {
        redStart1_C1[i] = i * (blobSize_C1 + spacingBetweenRedBlobs_C1);
        redEnd1_C1[i] = redStart1_C1[i] + blobSize_C1 - 1;
    }
    // パートC1青 各塊の開始と終了位置を設定するための配列を初期化
    for (int i = 0; i < numOfBlueBlobs_C1; i++)
    {
        blueStart_C1[i] = 95 + i * (blobSize_C1 + spacingBetweenBlueBlobs_C1);
        blueEnd_C1[i] = blueStart_C1[i] + blobSize_C1 - 1;
    }
    // パートC2赤 各塊の開始と終了位置を設定するための配列を初期化
    for (int i = 0; i < numOfRed1Blobs_C2; i++)
    {
        redStart1_C2[i] = 43 + i * (blobSize_C2 + spacingBetweenBlobs_C2);
        redEnd1_C2[i] = redStart1_C2[i] + blobSize_C2 - 1;
    }
    // パートC2青 各塊の開始と終了位置を設定するための配列を初期化
    for (int i = 0; i < numOfBlueBlobs_C2; i++)
    {
        blueStart_C2[i] = i * (blobSize_C2 + spacingBetweenBlobs_C2);
        blueEnd_C2[i] = blueStart_C2[i] + blobSize_C2 - 1;
    }
    // パートD 配列の初期化
    for (int i = 0; i < numOfRedBlobs_D; i++)
    {
        redStart_D[i] = i * (blobSize_D + spacingBetweenBlobs_D);
        redEnd_D[i] = redStart_D[i] + blobSize_D - 1;
    }
    for (int i = 0; i < numOfBlueBlobs_D; i++)
    {
        blueStart_D[i] = 71 - i * (blobSize_D + spacingBetweenBlobs_D);
        blueEnd_D[i] = blueStart_D[i] + blobSize_D - 1;
    }
    for (int i = 0; i < numOfPurpleBlobs_D; i++)
    {
        purpleStart1_D[i] = 31 - i * (blobSize_D + spacingBetweenBlobs_D);
        purpleEnd1_D[i] = purpleStart1_D[i] + blobSize_D - 1;
    }
    for (int i = 0; i < numOfPurpleBlobs_D; i++)
    {
        purpleStart2_D[i] = 36 - i * (blobSize_D + spacingBetweenBlobs_D);
        purpleEnd2_D[i] = purpleStart2_D[i] + blobSize_D - 1;
    }
}

void loop()
{
    digitalWrite(SummerLEDPin, LOW);
    digitalWrite(WinterLEDPin, LOW);
    Serial.println("main loop");
    Serial.println(season);
    unsigned long currentMillis = millis();
    Serial.println("change auqa LED");
    if (season == 1)
    {
        strip6.fill(strip6.Color(255, 80, 0)); // orange
        strip6.show();
        digitalWrite(WinterLEDPin, HIGH);
    }
    else if (season == 0)
    {
        strip6.fill(strip6.Color(80, 200, 240)); // blue
        strip6.show();
        digitalWrite(SummerLEDPin, HIGH);
    }
    while (currentMillis - previousMillis <= intervalA)
    {
        Serial.println("partA");
        partA(); // 00:15.00
        currentMillis = millis();
    }
    previousMillis = currentMillis;
    if (season == 0)
    {
        while (currentMillis - previousMillis <= intervalB)
        {
            Serial.println("partB");
            partB(); // 00:10.00
            currentMillis = millis();
        }
        strip2.fill(strip2.Color(0, 0, 0));
        strip2.show();
        previousMillis = currentMillis;
    }
    while (currentMillis - previousMillis <= intervalC)
    {
        Serial.println("partC");
        partC(); // 00:15.00
        currentMillis = millis();
    }
    strip3.fill(strip3.Color(0, 0, 0));
    strip3.show();
    strip4.fill(strip3.Color(0, 0, 0));
    strip4.show();
    previousMillis = currentMillis;

    while (currentMillis - previousMillis <= intervalD)
    {
        Serial.println("partD");
        partD(); // 00:10.00
        currentMillis = millis();
    }
    strip5.fill(strip5.Color(0, 0, 0));
    strip5.show();
    previousMillis = currentMillis;
    strip6.fill(strip6.Color(0, 0, 0));
    strip6.show();
    if (season == 1)
    {
        season = 0;
    }
    else if (season == 0)
    {
        season = 1;
    }
}

// Function to update a single strip
void updateStrip(Adafruit_NeoPixel &strip, uint16_t i)
{
    strip.fill(strip.Color(255, 0, 0), i, 3);
    strip.fill(strip.Color(0, 0, 0), 0, i);
    strip.show();
    delay(10); // Delay for the fill effect
}

void partA()
{
    flowRedToOrange1_A();     // LED番号1から56までを赤色の光の塊を流す
    gradientOrangeToCyan_A(); // LED番号57から76までをオレンジから水色のグラデーションにする
    flowRedToOrange2_A();     // LED番号77から113まで赤色の光の塊を流す
    flowBlueToCyan_A();
    flowBlueToCyan2_A(); // LED番号114から256までを青色の光の塊を流す
}

void flowRedToOrange1_A()
{
    for (int i = 0; i < 57; i++)
    {
        // 現在のLEDがどの塊にも含まれていない場合はオレンジ色に設定
        bool isBlob = false;
        for (int j = 0; j < numOfRed1Blobs_A; j++)
        {
            if (i >= redStart1_A[j] && i <= redEnd1_A[j])
            {
                if (season == 0)
                {
                    strip1.setPixelColor(i, 255, 80, 0); // オレンジ色の光の塊
                }
                else if (season == 1)
                {
                    strip1.setPixelColor(i, 100, 200, 240); // オレンジ色の光の塊
                }
                isBlob = true;
                break;
            }
        }
        if (!isBlob)
        {
            if (season == 0)
            {
                strip1.setPixelColor(i, 255, 0, 0); // 赤色
            }
            else if (season == 1)
            {
                strip1.setPixelColor(i, 0, 0, 255); // 赤色
            }
        }
    }
    strip1.show();
    delay(50);

    // 各塊を移動
    for (int j = 0; j < numOfRed1Blobs_A; j++)
    {
        redStart1_A[j]++;
        redEnd1_A[j]++;
        if (redEnd1_A[j] > 56)
        {
            redStart1_A[j] = 0;
            redEnd1_A[j] = blobSize_A - 1;
        }
    }
}

void gradientOrangeToCyan_A()
{
    int green = 0;
    if (season == 0)
    {
        for (int i = 57; i < 77; i++)
        {
            int red = map(i, 57, 77, 255, 0);
            int blue = map(i, 57, 77, 0, 255);
            strip1.setPixelColor(i, red, green, blue); // グラデーション
        }
    }
    else if (season == 1)
    {
        for (int i = 57; i < 77; i++)
        {
            int red = map(i, 57, 77, 0, 255);
            int blue = map(i, 57, 77, 255, 0);
            strip1.setPixelColor(i, red, green, blue); // グラデーション
        }
    }
    strip1.show();
    delay(50);
}

void flowRedToOrange2_A()
{
    // 各塊を移動
    for (int j = 0; j < numOfRed2Blobs_A; j++)
    {
        redStart2_A[j]++;
        redEnd2_A[j]++;
        if (redEnd2_A[j] > 113)
        {
            redStart2_A[j] = 79;
            redEnd2_A[j] = 79 + blobSize_A - 1;
        }
    }

    for (int i = 77; i < 114; i++)
    {
        // 現在のLEDがどの塊にも含まれていない場合はオレンジ色に設定
        bool isBlob = false;
        for (int j = 0; j < numOfRed2Blobs_A; j++)
        {
            if (i >= redStart2_A[j] && i <= redEnd2_A[j])
            {
                if (season == 0)
                {
                    strip1.setPixelColor(i, 255, 80, 0); // オレンジ色の光の塊
                }
                else if (season == 1)
                {
                    strip1.setPixelColor(i, 150, 200, 240); // オレンジ色の光の塊
                }

                isBlob = true;
                break;
            }
        }
        if (!isBlob)
        {
            if (season == 0)
            {
                strip1.setPixelColor(i, 255, 0, 0); // red
            }
            else if (season == 1)
            {
                strip1.setPixelColor(i, 0, 0, 255); // blue
            }
        }
    }
    strip1.show();
    delay(50);
}

void flowBlueToCyan_A()
{
    // 各塊を移動
    for (int j = 0; j < numOfBlueBlobs_A; j++)
    {
        blueStart_A[j]--;
        blueEnd_A[j]--;
        if (blueEnd_A[j] < 113)
        {
            blueStart_A[j] = 142 - blobSize_A + 1;
            blueEnd_A[j] = 142;
        }
    }

    // LED を設定
    for (int i = 142; i >= 114; i--)
    {
        // 現在のLEDがどの塊にも含まれていない場合は水色に設定
        bool isBlob = false;
        for (int j = 0; j < numOfBlueBlobs_A; j++)
        {
            if (i >= blueStart_A[j] && i <= blueEnd_A[j])
            {
                strip1.setPixelColor(i, 100, 100, 240); // 水色の光の塊
                isBlob = true;
                break;
            }
        }
        if (!isBlob)
        {
            strip1.setPixelColor(i, 0, 0, 255); // blue
        }
    }
    strip1.show();
    delay(50);
}

void flowBlueToCyan2_A()
{
    // 各塊を移動
    for (int j = 0; j < numOfBlue2Blobs_A; j++)
    {
        blueStart2_A[j]++;
        blueEnd2_A[j]++;
        if (blueEnd2_A[j] > 262)
        {
            blueStart2_A[j] = 142;
            blueEnd2_A[j] = 142 + blobSize_A - 1;
        }
    }

    // LED を設定
    for (int i = 143; i < 262; i++)
    {
        // 現在のLEDがどの塊にも含まれていない場合は水色に設定
        bool isBlob = false;
        for (int j = 0; j < numOfBlue2Blobs_A; j++)
        {
            if (i >= blueStart2_A[j] && i <= blueEnd2_A[j])
            {
                if (season == 0)
                {
                    strip1.setPixelColor(i, 100, 100, 240); // 水色の光の塊
                }
                else if (season == 1)
                {
                    strip1.setPixelColor(i, 255, 80, 0); // オレンジ色の光の塊
                }
                isBlob = true;
                break;
            }
        }
        if (!isBlob)
        {
            if (season == 0)
            {
                strip1.setPixelColor(i, 0, 0, 255); // blue
            }
            else if (season == 1)
            {
                strip1.setPixelColor(i, 255, 0, 0); // red
            }
        }
    }
    strip1.show();
    delay(50);
}

void partB()
{
    flowRedToOrange1_B(); // LED番号1から77までを赤色の光の塊を流す
    flowRedToOrange2_B(); // LED番号78から84までを赤色の光の塊を流す
    flowBlueToCyan1_B();  // LED番号85から91までを青色の光の塊を流す
    flowBlueToCyan2_B();  // LED番号92から98までを青色の光の塊を流す
    delay(100);
}

void flowRedToOrange1_B()
{
    for (int i = 76; i >= 0; i--)
    {
        // 現在のLEDがどの塊にも含まれていない場合はオレンジ色に設定
        bool isBlob = false;
        for (int j = 0; j < numOfRed1Blobs_B; j++)
        {
            if (i >= redStart1_B[j] && i <= redEnd1_B[j])
            {
                strip2.setPixelColor(i, 255, 80, 0); // 白色の光の塊
                isBlob = true;
                // break;
            }
        }
        if (!isBlob)
        {
            strip2.setPixelColor(i, 0, 255, 0); // 緑色
        }
    }
    strip2.show();
    // delay(50);
    //  各塊を移動
    for (int j = 0; j < numOfRed1Blobs_B; j++)
    {
        redStart1_B[j]--;
        redEnd1_B[j]--;
        if (redEnd1_B[j] < 0)
        {
            redStart1_B[j] = 77 - blobSize_B + 1;
            redEnd1_B[j] = 77;
        }
    }
    strip2.show();
}

void flowRedToOrange2_B()
{
    for (int i = 83; i >= 77; i--)
    {
        // 現在のLEDがどの塊にも含まれていない場合はオレンジ色に設定
        bool isBlob = false;
        for (int j = 0; j < numOfRed2Blobs_B; j++)
        {
            if (i >= redStart2_B[j] && i <= redEnd2_B[j])
            {
                strip2.setPixelColor(i, 255, 80, 0); // オレンジ色の光の塊
                isBlob = true;
                // break;
            }
        }
        if (!isBlob)
        {
            strip2.setPixelColor(i, 255, 0, 0); // 赤色
        }
    }
    strip2.show();
    // delay(50);
    //  各塊を移動
    for (int j = 0; j < numOfRed2Blobs_B; j++)
    {
        redStart2_B[j]--;
        redEnd2_B[j]--;
        if (redEnd2_B[j] < 78)
        {
            redStart2_B[j] = 84 - blobSize_B + 1;
            redEnd2_B[j] = 84;
        }
    }
    strip2.show();
}

void flowBlueToCyan1_B()
{
    // 各塊を移動
    for (int j = 0; j < numOfBlue1Blobs_B; j++)
    {
        blueStart1_B[j]--;
        blueEnd1_B[j]--;
        if (blueEnd1_B[j] < 85)
        {
            blueStart1_B[j] = 91 - blobSize_B + 1;
            blueEnd1_B[j] = 91;
        }
    }
    // LED を設定
    for (int i = 90; i > 83; i--)
    {
        // 現在のLEDがどの塊にも含まれていない場合は水色に設定
        bool isBlob = false;
        for (int j = 0; j < numOfBlue1Blobs_B; j++)
        {
            if (i >= blueStart1_B[j] && i <= blueEnd1_B[j])
            {
                strip2.setPixelColor(i, 100, 100, 240); // 水色の光の塊
                isBlob = true;
                // break;
            }
        }
        // if (season == 1 && i >= 85 && i < 92) {
        // strip2.setPixelColor(i, 255, 80, 0);  // オレンジ色の光の塊
        // if (season == 1 && >= 85 && i < 92 && !isBlob) {
        // strip2.setPixelColor(i, 255, 0, 0);  //赤色
        if (!isBlob)
        {
            strip2.setPixelColor(i, 0, 0, 255); // 青色
        }
    }
    strip2.show();
    // delay(50);
}

void flowBlueToCyan2_B()
{
    // 各塊を移動
    for (int j = 0; j < numOfBlue2Blobs_B; j++)
    {
        blueStart2_B[j]--;
        blueEnd2_B[j]--;
        if (blueEnd2_B[j] < 92)
        {
            blueStart2_B[j] = 99 - blobSize_B + 1;
            blueEnd2_B[j] = 99;
        }
    }
    // LED を設定
    for (int i = 97; i >= 91; i--)
    {
        // 現在のLEDがどの塊にも含まれていない場合は水色に設定
        bool isBlob = false;
        for (int j = 0; j < numOfBlue2Blobs_B; j++)
        {
            if (i >= blueStart2_B[j] && i <= blueEnd2_B[j])
            {
                strip2.setPixelColor(i, 100, 100, 100); // 白色の光の塊
                isBlob = true;
                // break;
            }
        }
        if (!isBlob)
        {
            strip2.setPixelColor(i, 0, 255, 0); // 緑色
        }
    }
    strip2.show();
    // delay(50);
}

void partC1()
{
    flowRedToOrange1_C1();      // LED番号1から31までを赤色の光の塊を流す
    gradientOrangeToCyan1_C1(); // LED番号32から83までをオレンジから水色のグラデーションにする
    flowBlueToCyan_C1();        // LED番号84から104までを青色の光の塊を流す
    gradientOrangeToCyan2_C1(); // LED番号105から159までをオレンジから水色のグラデーションにする
    gradientCyanToOrange_C1();  // LED番号160から214までを水色からオレンジのグラデーションにする
    delay(75);
}

void flowRedToOrange1_C1()
{
    for (int i = 0; i < 25; i++)
    {
        // 現在のLEDがどの塊にも含まれていない場合はオレンジ色に設定
        bool isBlob = false;
        for (int j = 0; j < numOfRed1Blobs_C1; j++)
        {
            if (i >= redStart1_C1[j] && i <= redEnd1_C1[j])
            {
                if (season == 0)
                {
                    strip3.setPixelColor(i, 255, 80, 0); // オレンジ色の光の塊
                }
                else
                {
                    strip3.setPixelColor(i, 150, 200, 240); // 水色の光の塊
                }
                isBlob = true;
                break;
            }
        }
        if (!isBlob)
        {
            if (season == 0)
            {
                strip3.setPixelColor(i, 255, 0, 0); // 赤色
            }
            else
            {
                strip3.setPixelColor(i, 0, 0, 255); // 赤色
            }
        }
    }
    strip3.show();

    // 各塊を移動
    for (int j = 0; j < numOfRed1Blobs_C1; j++)
    {
        redStart1_C1[j]++;
        redEnd1_C1[j]++;
        if (redEnd1_C1[j] > 24)
        {
            redStart1_C1[j] = 0;
            redEnd1_C1[j] = blobSize_C1 - 1;
        }
    }
}

void gradientOrangeToCyan1_C1()
{
    if (season == 0)
    {
        for (int i = 25; i < 84; i++)
        {
            int red = map(i, 25, 84, 255, 0);
            int green = 0;
            int blue = map(i, 25, 84, 0, 255);
            strip3.setPixelColor(i, red, green, blue); // グラデーション
        }
    }
    else
    {
        for (int i = 25; i < 84; i++)
        {
            int red = map(i, 25, 84, 0, 255);
            int green = 0;
            int blue = map(i, 25, 84, 255, 0);
            strip3.setPixelColor(i, red, green, blue); // グラデーション
        }
    }

    strip3.show();
}

void flowBlueToCyan_C1()
{
    // 各塊を移動
    for (int j = 0; j < numOfBlueBlobs_C1; j++)
    {
        blueStart_C1[j]++;
        blueEnd_C1[j]++;
        if (blueEnd_C1[j] > 102)
        {
            blueStart_C1[j] = 84;
            blueEnd_C1[j] = 84 + blobSize_C1 - 1;
        }
    }
    // LED を設定
    for (int i = 84; i < 103; i++)
    {
        // 現在のLEDがどの塊にも含まれていない場合は水色に設定
        bool isBlob = false;
        for (int j = 0; j < numOfBlueBlobs_C1; j++)
        {
            if (i >= blueStart_C1[j] && i <= blueEnd_C1[j])
            {
                if (season == 0)
                {
                    strip3.setPixelColor(i, 100, 100, 240); // 水色の光の塊
                }
                else
                {
                    strip3.setPixelColor(i, 255, 80, 0); // オレンジの光の塊
                }
                isBlob = true;
                break;
            }
        }
        if (!isBlob)
        {
            if (season == 0)
            {
                strip3.setPixelColor(i, 0, 0, 255); // 青色
            }
            else
            {
                strip3.setPixelColor(i, 255, 0, 0); // 青色
            }
        }
    }

    strip3.show();
}

void gradientOrangeToCyan2_C1()
{
    if (season == 0)
    {
        for (int i = 103; i < 160; i++)
        {
            int red = map(i, 103, 160, 255, 0);
            int green = 0;
            int blue = map(i, 103, 160, 0, 255);
            strip3.setPixelColor(i, red, green, blue); // グラデーション
        }
    }
    else
    {
        for (int i = 103; i < 160; i++)
        {
            int red = map(i, 103, 160, 0, 255);
            int green = 0;
            int blue = map(i, 103, 160, 255, 0);
            strip3.setPixelColor(i, red, green, blue); // グラデーション
        }
    }
    strip3.show();
    // delay(50);
}

void gradientCyanToOrange_C1()
{
    if (season == 0)
    {
        for (int i = 160; i < 215; i++)
        {
            int blue = map(i, 160, 215, 255, 0);
            int green = 0;
            int red = map(i, 160, 215, 0, 255);
            strip3.setPixelColor(i, red, green, blue); // グラデーション
        }
    }
    else
    {
        for (int i = 160; i < 215; i++)
        {
            int blue = map(i, 160, 215, 0, 255);
            int green = 0;
            int red = map(i, 160, 215, 255, 0);
            strip3.setPixelColor(i, red, green, blue); // グラデーション
        }
    }

    strip3.show();
    // delay(50);
}

void partC2()
{
    flowRedToOrange1_C2();
    flowBlueToCyan_C2();
    delay(75);
}

void flowBlueToCyan_C2()
{
    for (int i = 0; i < 42; i++)
    {
        // 現在のLEDがどの塊にも含まれていない場合は青色に設定
        bool isBlob = false;
        for (int j = 0; j < numOfBlueBlobs_C2; j++)
        {
            if (i >= blueStart_C2[j] && i <= blueEnd_C2[j])
            {
                if (season == 0)
                {
                    strip4.setPixelColor(i, 100, 100, 240); // 水色の光の塊
                }
                else
                {
                    strip4.setPixelColor(i, 255, 80, 0); // オレンジの光の塊
                }
                isBlob = true;
                break;
            }
        }
        if (!isBlob)
        {
            if (season == 0)
            {
                strip4.setPixelColor(i, 0, 0, 255); // 青色
            }
            else
            {
                strip4.setPixelColor(i, 255, 0, 0); // 青色
            }
        }
    }
    strip4.show();
    // delay(50);

    // 各塊を移動
    for (int j = 0; j < numOfBlueBlobs_C2; j++)
    {
        blueStart_C2[j]++;
        blueEnd_C2[j]++;
        if (blueEnd_C2[j] > 42)
        {
            blueStart_C2[j] = 0;
            blueEnd_C2[j] = blobSize_C2 - 1;
        }
    }
}

void flowRedToOrange1_C2()
{
    // 各塊を移動
    for (int j = 0; j < numOfRed1Blobs_C2; j++)
    {
        redStart1_C2[j]++;
        redEnd1_C2[j]++;
        if (redEnd1_C2[j] > 76)
        {
            redStart1_C2[j] = 42;
            redEnd1_C2[j] = 42 + blobSize_C2 - 1;
        }
    }

    // LED を設定
    for (int i = 42; i < 77; i++)
    {
        // 現在のLEDがどの塊にも含まれていない場合は青色に設定
        bool isBlob = false;
        for (int j = 0; j < numOfRed1Blobs_C2; j++)
        {
            if (i >= redStart1_C2[j] && i <= redEnd1_C2[j])
            {
                if (season == 0)
                {
                    strip4.setPixelColor(i, 255, 80, 0); // オレンジ色の光の塊
                }
                else
                {
                    strip4.setPixelColor(i, 150, 200, 240); // 水色の光の塊
                }
                isBlob = true;
                break;
            }
        }
        if (!isBlob)
        {
            if (season == 0)
            {
                strip4.setPixelColor(i, 255, 0, 0); // 赤色
            }
            else
            {
                strip4.setPixelColor(i, 0, 0, 255); // 赤色
            }
        }
    }
    strip4.show();
}

void partC()
{
    partC1();
    partC2();
}

void partD()
{
    flowRedToOrange_D();
    flowPurple1_D();
    flowPurple2_D();
    flowBlueToCyan_D();
}

void flowRedToOrange_D()
{

    for (int j = 0; j < numOfRedBlobs_D; j++)
    {
        redStart_D[j]++;
        redEnd_D[j]++;
        if (redEnd_D[j] > 25)
        {
            redStart_D[j] = 0;
            redEnd_D[j] = blobSize_D - 1;
        }
    }
    for (int i = 0; i < 26; i++)
    {
        bool isBlob = false;
        for (int j = 0; j < numOfRedBlobs_D; j++)
        {
            if (i >= redStart_D[j] && i <= redEnd_D[j])
            {
                strip5.setPixelColor(i, 250, 80, 0);
                isBlob = true;
                break;
            }
        }
        if (!isBlob)
        {
            strip5.setPixelColor(i, 250, 0, 0);
        }
    }
    strip5.show();
    delay(50);
}

void flowPurple1_D()
{
    for (int j = 0; j < numOfPurpleBlobs_D; j++)
    {
        purpleStart1_D[j]--;
        purpleEnd1_D[j]--;
        if (purpleStart1_D[j] < 25)
        {
            purpleStart1_D[j] = 30 - blobSize_D + 1;
            purpleEnd1_D[j] = 30;
        }
    }
    for (int i = 30; i >= 26; i--)
    {
        bool isBlob = false;
        for (int j = 0; j < numOfPurpleBlobs_D; j++)
        {
            if (i >= purpleStart1_D[j] && i <= purpleEnd1_D[j])
            {
                if (season == 0)
                {
                    strip5.setPixelColor(i, 255, 80, 0); // オレンジ色の光の塊
                }
                else
                {
                    strip5.setPixelColor(i, 100, 100, 240); // 水色の光の塊
                }
                isBlob = true;
                break;
            }
        }
        if (!isBlob)
        {
            if (season == 0)
            {
                strip5.setPixelColor(i, 255, 0, 0);
            }
            else
            {
                strip5.setPixelColor(i, 0, 0, 255);
            }
        }
    }
    strip5.show();
    delay(50);
}

void flowPurple2_D()
{
    for (int j = 0; j < numOfPurpleBlobs_D; j++)
    {
        purpleStart2_D[j]--;
        purpleEnd2_D[j]--;
        if (purpleStart2_D[j] < 30)
        {
            purpleStart2_D[j] = 35 - blobSize_D + 1;
            purpleEnd2_D[j] = 35;
        }
    }
    for (int i = 35; i >= 31; i--)
    {
        bool isBlob = false;
        for (int j = 0; j < numOfPurpleBlobs_D; j++)
        {
            if (i >= purpleStart2_D[j] && i <= purpleEnd2_D[j])
            {
                if (season == 1)
                {
                    strip5.setPixelColor(i, 255, 80, 0); // オレンジ色の光の塊
                }
                else
                {
                    strip5.setPixelColor(i, 150, 200, 240); // 水色の光の塊
                }
                isBlob = true;
                break;
            }
        }
        if (!isBlob)
        {
            if (season == 1)
            {
                strip5.setPixelColor(i, 255, 0, 0);
            }
            else
            {
                strip5.setPixelColor(i, 0, 0, 255);
            }
        }
    }
    strip5.show();
    delay(50);
}

void flowBlueToCyan_D()
{
    for (int j = 0; j < numOfBlueBlobs_D; j++)
    {
        blueStart_D[j]--;
        blueEnd_D[j]--;
        if (blueStart_D[j] < 35)
        {
            blueStart_D[j] = 70 - blobSize_D + 1;
            blueEnd_D[j] = 70;
        }
    }
    for (int i = 70; i >= 36; i--)
    {
        bool isBlob = false;
        for (int j = 0; j < numOfBlueBlobs_D; j++)
        {
            if (i >= blueStart_D[j] && i <= blueEnd_D[j])
            {
                strip5.setPixelColor(i, 100, 100, 240); // 青の光の塊
                isBlob = true;
                break;
            }
        }
        if (!isBlob)
        {
            strip5.setPixelColor(i, 0, 0, 255);
        }
    }
    strip5.show();
    delay(50);
}

void colorTransition(uint32_t colorStart, uint32_t colorEnd, int transitionTime)
{
    unsigned long startTime = millis();
    int wait = 50; // 各ステップの待機時間

    while (millis() - startTime < transitionTime)
    {
        float progress = (millis() - startTime) / (float)transitionTime; // 進捗度合い（0.0から1.0）
        uint8_t r = (1 - progress) * ((colorStart >> 16) & 0xFF) + progress * ((colorEnd >> 16) & 0xFF);
        uint8_t g = (1 - progress) * ((colorStart >> 8) & 0xFF) + progress * ((colorEnd >> 8) & 0xFF);
        uint8_t b = (1 - progress) * (colorStart & 0xFF) + progress * (colorEnd & 0xFF);

        for (int i = 0; i < strip6.numPixels(); i++)
        {
            strip6.setPixelColor(i, strip6.Color(r, g, b));
        }
        strip6.show();
        delay(wait);
    }
}

void Summer()
{
    unsigned long interruptTime = millis();
    if (interruptTime - lastInterruptTime > debounceTime)
    {
        lastInterruptTime = interruptTime;
        // Summerボタンの処理
        season = 0;
        Serial.println("Summer");
        Serial.println(season);
        digitalWrite(WinterLEDPin, LOW);
        digitalWrite(SummerLEDPin, HIGH);
        strip6.fill(strip6.Color(80, 200, 240)); // blue
        strip6.show();
        Serial.println("partXA");
        partAX(season);
        Serial.println("partXB");
        partBX();
        Serial.println("partXC");
        partCX(season);
        Serial.println("partXD");
        partDX(season);
        buttonS = true;
        Serial.println("keep 3 minutes");
        delay(keep);
        digitalWrite(SummerLEDPin, LOW);
        Serial.println("end");
    }
}

void Winter()
{
    unsigned long interruptTime = millis();
    if (interruptTime - lastInterruptTime > debounceTime)
    {
        lastInterruptTime = interruptTime;
        // Winterボタンの処理
        season = 1;
        digitalWrite(SummerLEDPin, LOW);
        digitalWrite(WinterLEDPin, HIGH);
        strip6.fill(strip6.Color(255, 80, 0)); // orange
        strip6.show();
        Serial.println("partXA");
        partAX(season);
        strip2.fill(strip2.Color(0, 0, 0)); // off
        strip2.show();
        Serial.println("partXC");
        partCX(season);
        strip5.fill(strip5.Color(0, 0, 0)); // off
        strip5.show();
        buttonW = true;
        Serial.println("keep 3 minutes");
        delay(keep);
        digitalWrite(WinterLEDPin, LOW);
        Serial.println("end");
    }
}

void partAX(int season)
{
    flowRedToOrange1_AX(season);     // LED番号1から56までを赤色の光の塊を流す
    gradientOrangeToCyan_AX(season); // LED番号57から76までをオレンジから水色のグラデーションにする
    flowRedToOrange2_AX(season);     // LED番号77から113まで赤色の光の塊を流す
    flowBlueToCyan_AX();
    flowBlueToCyan2_AX(season); // LED番号114から256までを青色の光の塊を流す
}

void flowRedToOrange1_AX(int season)
{
    for (int i = 0; i < 57; i++)
    {
        if (season == 0)
        {
            strip1.setPixelColor(i, 255, 0, 0); // 赤色
        }
        else if (season == 1)
        {
            strip1.setPixelColor(i, 0, 0, 255); // 青色
        }
    }
    strip1.show();
}

void gradientOrangeToCyan_AX(int season)
{
    int green = 0;
    if (season == 0)
    {
        for (int i = 57; i < 77; i++)
        {
            int red = map(i, 57, 77, 255, 0);
            int blue = map(i, 57, 77, 0, 255);
            strip1.setPixelColor(i, red, green, blue); // グラデーション
        }
    }
    else if (season == 1)
    {
        for (int i = 57; i < 77; i++)
        {
            int red = map(i, 57, 77, 0, 255);
            int blue = map(i, 57, 77, 255, 0);
            strip1.setPixelColor(i, red, green, blue); // グラデーション
        }
    }
    strip1.show();
}

void flowRedToOrange2_AX(int season)
{
    for (int i = 77; i < 114; i++)
    {
        if (season == 0)
        {
            strip1.setPixelColor(i, 255, 0, 0); // red
        }
        else if (season == 1)
        {
            strip1.setPixelColor(i, 0, 0, 255); // blue
        }
    }
    strip1.show();
}

void flowBlueToCyan_AX()
{
    strip1.fill(strip1.Color(0, 0, 255), 114, 29);
    strip1.show();
}

void flowBlueToCyan2_AX(int season)
{
    for (int i = 143; i < 262; i++)
    {
        if (season == 0)
        {
            strip1.setPixelColor(i, 0, 0, 255); // blue
        }
        else if (season == 1)
        {
            strip1.setPixelColor(i, 255, 0, 0); // red
        }
    }
    strip1.show();
}

void partBX()
{
    flowRedToOrange1_BX(); // LED番号1から77まで緑色
    flowRedToOrange2_BX(); // LED番号78から84までを赤色
    flowBlueToCyan1_BX();  // LED番号85から91までを青色
    flowBlueToCyan2_BX();  // LED番号92から98までを青色
}

void flowRedToOrange1_BX()
{
    strip2.fill(strip2.Color(0, 255, 0), 0, 77);
    strip2.show();
}

void flowRedToOrange2_BX()
{
    strip2.fill(strip2.Color(255, 0, 0), 77, 7);
    strip2.show();
}

void flowBlueToCyan1_BX()
{
    strip2.fill(strip2.Color(0, 0, 255), 84, 7);
    strip2.show();
}

void flowBlueToCyan2_BX()
{
    strip2.fill(strip2.Color(0, 255, 0), 91, 7);
    strip2.show();
}

void partC1X(int season)
{
    flowRedToOrange1_C1X(season);      // LED番号1から31までを赤色の光の塊を流す
    gradientOrangeToCyan1_C1X(season); // LED番号32から83までをオレンジから水色のグラデーションにする
    flowBlueToCyan_C1X(season);        // LED番号84から104までを青色の光の塊を流す
    gradientOrangeToCyan2_C1X(season); // LED番号105から159までをオレンジから水色のグラデーションにする
    gradientCyanToOrange_C1X(season);  // LED番号160から214までを水色からオレンジのグラデーションにする
}

void flowRedToOrange1_C1X(int season)
{
    for (int i = 0; i < 25; i++)
    {
        if (season == 0)
        {
            strip3.setPixelColor(i, 255, 0, 0); // 赤色
        }
        else
        {
            strip3.setPixelColor(i, 0, 0, 255); // 青色
        }
    }
    strip3.show();
}

void gradientOrangeToCyan1_C1X(int season)
{
    if (season == 0)
    {
        for (int i = 25; i < 84; i++)
        {
            int red = map(i, 25, 84, 255, 0);
            int green = 0;
            int blue = map(i, 25, 84, 0, 255);
            strip3.setPixelColor(i, red, green, blue); // グラデーション
        }
    }
    else
    {
        for (int i = 25; i < 84; i++)
        {
            int red = map(i, 25, 84, 0, 255);
            int green = 0;
            int blue = map(i, 25, 84, 255, 0);
            strip3.setPixelColor(i, red, green, blue); // グラデーション
        }
    }
    strip3.show();
}

void flowBlueToCyan_C1X(int season)
{
    for (int i = 84; i < 103; i++)
    {
        if (season == 0)
        {
            strip3.setPixelColor(i, 0, 0, 255); // 青色
        }
        else
        {
            strip3.setPixelColor(i, 255, 0, 0); // 赤色
        }
    }
    strip3.show();
}

void gradientOrangeToCyan2_C1X(int season)
{
    if (season == 0)
    {
        for (int i = 103; i < 160; i++)
        {
            int red = map(i, 103, 160, 255, 0);
            int green = 0;
            int blue = map(i, 103, 160, 0, 255);
            strip3.setPixelColor(i, red, green, blue); // グラデーション
        }
    }
    else
    {
        for (int i = 103; i < 160; i++)
        {
            int red = map(i, 103, 160, 0, 255);
            int green = 0;
            int blue = map(i, 103, 160, 255, 0);
            strip3.setPixelColor(i, red, green, blue); // グラデーション
        }
    }
    strip3.show();
}

void gradientCyanToOrange_C1X(int season)
{
    if (season == 0)
    {
        for (int i = 160; i < 215; i++)
        {
            int blue = map(i, 160, 215, 255, 0);
            int green = 0;
            int red = map(i, 160, 215, 0, 255);
            strip3.setPixelColor(i, red, green, blue); // グラデーション
        }
    }
    else
    {
        for (int i = 160; i < 215; i++)
        {
            int blue = map(i, 160, 215, 0, 255);
            int green = 0;
            int red = map(i, 160, 215, 255, 0);
            strip3.setPixelColor(i, red, green, blue); // グラデーション
        }
    }
    strip3.show();
}

void partC2X(int season)
{
    flowRedToOrange1_C2X(season);
    flowBlueToCyan_C2X(season);
}

void flowBlueToCyan_C2X(int season)
{
    for (int i = 0; i < 42; i++)
    {
        if (season == 0)
        {
            strip4.setPixelColor(i, 0, 0, 255); // 青色
        }
        else
        {
            strip4.setPixelColor(i, 255, 0, 0); // 赤色
        }
    }
    strip4.show();
}

void flowRedToOrange1_C2X(int season)
{
    for (int i = 42; i < 77; i++)
    {
        if (season == 0)
        {
            strip4.setPixelColor(i, 255, 0, 0); // 赤色
        }
        else
        {
            strip4.setPixelColor(i, 0, 0, 255); // 青色
        }
    }
    strip4.show();
}

void partCX(int season)
{
    partC1X(season);
    partC2X(season);
}

void partDX(int season)
{
    flowRedToOrange_DX();
    flowPurple1_DX(season);
    flowPurple2_DX(season);
    flowBlueToCyan_DX();
}

void flowRedToOrange_DX()
{
    strip5.fill(strip5.Color(255, 0, 0), 0, 26);
    strip5.show();
}

void flowPurple1_DX(int season)
{
    for (int i = 30; i >= 26; i--)
    {
        if (season == 0)
        {
            strip5.setPixelColor(i, 255, 0, 0);
        }
        else
        {
            strip5.setPixelColor(i, 0, 0, 255);
        }
    }
    strip5.show();
}

void flowPurple2_DX(int season)
{
    for (int i = 35; i >= 31; i--)
    {
        if (season == 1)
        {
            strip5.setPixelColor(i, 255, 0, 0);
        }
        else
        {
            strip5.setPixelColor(i, 0, 0, 255);
        }
    }
    strip5.show();
}

void flowBlueToCyan_DX()
{
    strip5.fill(strip5.Color(0, 0, 255), 36, 35);
    strip5.show();
}
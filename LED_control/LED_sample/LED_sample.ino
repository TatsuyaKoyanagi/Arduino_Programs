#include <Adafruit_NeoPixel.h> //ライブラリの読み込み
#define MAX_VAL 65             // 最大輝度の設定をする(暗い←0 ～ 255→明い)
#define DELAY_TIME 50          // 待ち時間1の設定
#define DELAY_TIME2 20         // 待ち時間2の設定

// Adafruit_NeoPixel(LEDの使用個数、Arduinoのピン番号、色の並び+データの転送速度)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, 6, NEO_GRB + NEO_KHZ800);

void setup()
{
  strip.begin(); // インスタンスの使用を開始、この時全てのLEDの状態を「0」とする。
  strip.show();  // Arduinoから全てのLEDへオフ信号「0」を転送し初期化する。
}

void loop()
{
  // LEDへ出力する各色の設定例
  // colorWipe(strip.Color(0, 0, 0), DELAY_TIME); // 黒
  colorWipe(strip.Color(MAX_VAL, 0, 0), DELAY_TIME); // 赤
                                                     /**
                                                     colorWipe(strip.Color(0, MAX_VAL, 0), DELAY_TIME); // 緑
                                                     colorWipe(strip.Color(MAX_VAL, MAX_VAL, 0), DELAY_TIME); // 黄
                                                     colorWipe(strip.Color(0, 0, MAX_VAL), DELAY_TIME); // 青
                                                     colorWipe(strip.Color(MAX_VAL, 0, MAX_VAL), DELAY_TIME); // 紫
                                                     colorWipe(strip.Color(0, MAX_VAL, MAX_VAL), DELAY_TIME); // シアン
                                                     colorWipe(strip.Color(MAX_VAL, MAX_VAL, MAX_VAL), DELAY_TIME); // 白
                                                   
                                                     //LEDを虹色に変化させる
                                                     rainbow(DELAY_TIME2);
                                                     rainbowCycle(DELAY_TIME2);
                                                   
                                                     // 劇場風の照明
                                                     theaterChase(strip.Color(127, 127, 127), 50); // White
                                                     theaterChase(strip.Color(127, 0, 0), 50); // Red
                                                     theaterChase(strip.Color(0, 0, 127), 50); // Blue
                                                     theaterChaseRainbow(50);
                                                     **/
}

// 全てのLEDを指定色で点灯させる関数
void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// LEDを連続的に虹色に変化させる関数
void rainbow(uint8_t wait)
{
  uint16_t i, j;
  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// 虹色を均等に分布させる関数
void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;
  for (j = 0; j < 256 * 5; j++)
  { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// 劇場風のクロールライトにする関数
void theaterChase(uint32_t c, uint8_t wait)
{
  for (int j = 0; j < 10; j++)
  {
    for (int q = 0; q < 3; q++)
    {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, c); // turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, 0); // turn every third pixel off
      }
    }
  }
}

// 虹の様な劇場風クロールライトにする関数
void theaterChaseRainbow(uint8_t wait)
{
  for (int j = 0; j < 256; j++)
  {
    for (int q = 0; q < 3; q++)
    {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, Wheel((i + j) % 255)); // turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, 0); // turn every third pixel off
      }
    }
  }
}

// 色の値を取得するには、0〜255の値を入力します。
// 色の移り変わりはR(赤)→G(緑)→B(青)からR(赤)に戻ります。
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85)
  {
    return strip.Color((WheelPos * 3) * MAX_VAL / 255, (255 - WheelPos * 3) * MAX_VAL / 255, 0);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color((255 - WheelPos * 3) * MAX_VAL / 255, 0, (WheelPos * 3) * MAX_VAL / 255);
  }
  else
  {
    WheelPos -= 170;
    return strip.Color(0, (WheelPos * 3) * MAX_VAL / 255, (255 - WheelPos * 3) * MAX_VAL / 255);
  }
}
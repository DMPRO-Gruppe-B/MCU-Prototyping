#define MOSI 12
#define SCLK 13

int prev_clock = 0;
char cur_char = 0;
int bits_read = 0;

void setup (void) {
    Serial.begin (115200);

    pinMode(MOSI, INPUT);

    delay(20);
    Serial.println("FAM");
    prev_clock = digitalRead(SCLK);
}


void loop (void) {
    int clk = digitalRead(SCLK);
    if (prev_clock == 0 && clk) {
        // Rising clock
        char b = digitalRead(MOSI);
        
        cur_char = cur_char << 1;
        cur_char = cur_char | b;
        bits_read++;
        //Serial.print(bits_read);
        //Serial.print(": ");
        //Serial.println(cur_char);

        if (bits_read == 8) {
            //Serial.print(cur_char, HEX);
            //Serial.print("\n");
            Serial.print(cur_char);
            cur_char = 0;
            bits_read = 0;
        }
    }
    prev_clock = clk;
    delay(1);
}

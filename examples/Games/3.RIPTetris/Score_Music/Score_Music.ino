// this program uses Arduino Nano

#include <Dis7Seg.h>

#define buzzerPin A5

// pins in order (A, B, C, D, E, F, G, DP)
int segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9};

// digit pins from left to right
int digitPins[] = {13, 12, 11, 10};

Dis7Seg dis('-', segmentPins, 4, digitPins);

// Initialize array with -1, and the ones place with 0
int scoreNum[4] = {-1, -1, -1, 0};
int score = 0;
bool gameOver = false;

int serial = 0;

unsigned long previousMillis = 0;
int songIndex = 0;
bool isPlayingNote = true;
unsigned long noteDuration = 0;
bool musicEnabled = true;

const int songNotes[][2] = {
    {659, 400}, {494, 200}, {523, 200}, {587, 400}, {523, 200}, {494, 200}, {440, 400}, {440, 200}, {523, 200}, {659, 400}, {587, 200}, {523, 200}, {494, 400}, {523, 200}, {587, 400}, {659, 400}, {523, 400}, {440, 400}, {440, 400}, {587, 400}, {698, 200}, {880, 400}, {784, 200}, {698, 200}, {659, 600}, {523, 200}, {659, 400}, {587, 200}, {523, 200}, {494, 400}, {494, 200}, {523, 200}, {587, 400}, {659, 400}, {523, 400}, {440, 400}, {440, 400}, {659, 400}, {494, 200}, {523, 200}, {587, 400}, {523, 200}, {494, 200}, {440, 400}, {440, 200}, {523, 200}, {659, 400}, {587, 200}, {523, 200}, {494, 400}, {523, 200}, {587, 400}, {659, 400}, {523, 400}, {440, 400}, {440, 400}, {587, 400}, {698, 200}, {880, 400}, {784, 200}, {698, 200}, {659, 600}, {523, 200}, {659, 400}, {587, 200}, {523, 200}, {494, 400}, {494, 200}, {523, 200}, {587, 400}, {659, 400}, {523, 400}, {440, 400}, {440, 400}};

void playSong()
{
    if (!musicEnabled)
    {
        return;
    }
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= noteDuration)
    {
        previousMillis = currentMillis;

        if (isPlayingNote)
        {
            // Stop the tone and start the pause
            noTone(buzzerPin);
            isPlayingNote = false;
            noteDuration = 50; // Pause duration
        }
        else
        {
            // Start the next note
            tone(buzzerPin, songNotes[songIndex][0]);
            isPlayingNote = true;
            noteDuration = songNotes[songIndex][1]; // Note duration
            songIndex++;

            if (songIndex >= (int)(sizeof(songNotes) / sizeof(songNotes[0])))
            {
                songIndex = 0; // Repeat the song
            }
        }
    }
}

int receiveSerial()
{
    Serial.println("Serial check triggered");
    if (Serial.available() >= 2)
    {
        byte low = Serial.read();
        byte high = Serial.read();
        int input = word(high, low);

        return input;
    }
    return -1;
}

void updateScoreDigits()
{
    for (int i = 0; i < 4; i++)
        scoreNum[i] = -1;
    if (score == 0)
    {
        scoreNum[3] = 0;
    }
    else
    {
        int tempScore = score;
        int index = 3;
        while (tempScore > 0 && index >= 0)
        {
            scoreNum[index] = tempScore % 10;
            tempScore /= 10;
            index--;
        }
    }
}

void setup()
{
    Serial.begin(9600); // Initialize serial communication at 9600 baud
    bool next_step = false;

    while (!next_step)
    {
        serial = receiveSerial();
        if (serial == 10000)
        {
            next_step = true; // Wait for the start signal
            gameOver = false; // Reset the latch
        }
    }
}

void loop()
{
    Serial.print("Score: ");
    Serial.print(score);
    Serial.print(" | Array: [");
    for (int i = 0; i < 4; i++)
    {
        Serial.print(scoreNum[i]);
        if (i < 3)
            Serial.print(", ");
    }
    Serial.println("]");

    // 3. Hardware Refresh (Must run as fast as possible)
    if (!gameOver)
    {
        serial = receiveSerial();

        if (serial != -1)
        {
            if (serial == 10001)
            {
                gameOver = true; // Set the latch
                noTone(buzzerPin);
            }
            // ONLY update the score if the game is actually running
            else
            {
                score = serial;
                updateScoreDigits();
            }
        }

        playSong();
    }
    dis.scan(scoreNum);
}

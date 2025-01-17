// this program uses Arduino Nano

#include <Dis7Seg.h>

#define buzzerPin A5

// pins in order (A, B, C, D, E, F, G, DP)
int segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9};

// digit pins from left to right
int digitPins[] = {10, 11, 12, 13};

Dis7Seg dis('-', segmentPins, 4, digitPins);

// Initialize array with -1, and the ones place with 0
int scoreNum[4] = {-1, -1, -1, -1};
int score = 0;

bool initiate = false;

unsigned long previousMillis = 0;
int songIndex = 0;
bool isPlayingNote = true;
unsigned long noteDuration = 0;

const int songNotes[][2] = {
    {659, 400}, {494, 200}, {523, 200}, {587, 400}, {523, 200}, {494, 200}, {440, 400}, {440, 200}, {523, 200}, {659, 400}, {587, 200}, {523, 200}, {494, 400}, {523, 200}, {587, 400}, {659, 400}, {523, 400}, {440, 400}, {440, 400}, {587, 400}, {698, 200}, {880, 400}, {784, 200}, {698, 200}, {659, 600}, {523, 200}, {659, 400}, {587, 200}, {523, 200}, {494, 400}, {494, 200}, {523, 200}, {587, 400}, {659, 400}, {523, 400}, {440, 400}, {440, 400}, {659, 400}, {494, 200}, {523, 200}, {587, 400}, {523, 200}, {494, 200}, {440, 400}, {440, 200}, {523, 200}, {659, 400}, {587, 200}, {523, 200}, {494, 400}, {523, 200}, {587, 400}, {659, 400}, {523, 400}, {440, 400}, {440, 400}, {587, 400}, {698, 200}, {880, 400}, {784, 200}, {698, 200}, {659, 600}, {523, 200}, {659, 400}, {587, 200}, {523, 200}, {494, 400}, {494, 200}, {523, 200}, {587, 400}, {659, 400}, {523, 400}, {440, 400}, {440, 400}};

void playSong()
{

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

void recieveSerial()
{
    if (Serial.available() >= 2) // Ensure we have at least 2 bytes to read
    {
        // Read the low and high bytes
        byte low = Serial.read();
        byte high = Serial.read();

        // Reconstruct the full integer (16-bit value)
        int input = word(high, low); // Combine the high and low byte to get the original score

        switch (input)
        {
        case 10000:
            initiate = true;
            break;

        default:
            score = input;
            break;
        }
    }
}

void updateScoreDisplay()
{
    // Handle the special case where score is 0
    if (score == 0)
    {
        scoreNum[3] = 0; // Display '0' in the units place
        for (int i = 0; i < 3; i++)
            scoreNum[i] = -1; // Blank all other digits
    }
    else
    {
        // Temporary variable to hold the score
        int tempScore = score;
        bool leadingZero = true;

        // Extract digits and handle leading zeros
        for (int i = 3; i >= 0; i--)
        {
            scoreNum[i] = tempScore % 10;
            tempScore /= 10;

            if (scoreNum[i] == 0 && leadingZero)
                scoreNum[i] = -1; // Blank leading zero
            else
                leadingZero = false;
        }
    }

    // Update the 7-segment display
    dis.scan(scoreNum);
}

void setup()
{
    Serial.begin(9600); // Initialize serial communication at 9600 baud
}

void loop()
{
    recieveSerial();
    if (!initiate)
    {
        dis.scan(scoreNum, nullptr, true); // Refresh the display with updated score
    }
    else
    {
        playSong();
        updateScoreDisplay();
    }
}

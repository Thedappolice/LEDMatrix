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

void receiveSerial()
{
    while (Serial.available() >= 2)
    {
        byte low = Serial.read();
        byte high = Serial.read();

        int input = word(high, low);

        if (input == 10000)
        {
            initiate = true;
        }
        else if (input >= 0 && input <= 9999)
        {
            score = input;
        }
    }
}

void updateScoreDisplay()
{
    // Reset scoreNum before processing
    for (int i = 0; i < 4; i++)
        scoreNum[i] = -1; // Assume all are blank initially

    // Handle the special case where score is 0
    if (score == 0)
    {
        scoreNum[3] = 0; // Display '0' in the units place
    }
    else
    {
        int tempScore = score;
        int index = 3; // Start from the rightmost digit

        // Extract digits, ensuring leading zeros are blank
        while (tempScore > 0)
        {
            scoreNum[index] = tempScore % 10; // Get last digit
            tempScore /= 10;
            index--; // Move left
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
    receiveSerial();
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

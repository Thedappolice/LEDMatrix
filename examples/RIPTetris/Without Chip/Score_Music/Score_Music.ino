#include <Dis7Seg.h>

#define buzzerPin A5

// pins in order (A, B, C, D, E, F, G, DP)
int segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9};

// digit pins from left to right
int digitPins[] = {10, 11, 12, 13};

Dis7Seg dis('-', segmentPins, 4, digitPins);

// Initialize array with -1, and the ones place with 0
int score[4] = {-1, -1, -1, 0};

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

            if (songIndex >= sizeof(songNotes) / sizeof(songNotes[0]))
            {
                songIndex = 0; // Repeat the song
            }
        }
    }
}

void checkScore()
{
    if (Serial.available() > 0)
    {
        int input = Serial.parseInt(); // Read the score sent by the Teensy

        // Fill the array with digits from right to left
        for (int i = 3; i >= 0; i--)
        {
            score[i] = input % 10; // Get the last digit
            input /= 10;           // Remove the last digit from input
        }

        // Check for stray 0s and replace with -1
        for (int i = 0; i < 3; i++)
        {
            if (score[i] == 0 && (i == 0 || score[i - 1] == -1))
            {
                score[i] = -1;
            }
        }
    }
    dis.scan(score); // refresh display with score
}

void setup()
{
    Serial.begin(9600); // Initialize serial communication at 9600 baud
}

void loop()
{
    playSong();
    checkScore();
}

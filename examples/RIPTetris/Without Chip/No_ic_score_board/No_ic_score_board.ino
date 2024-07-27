#include <Dis7Seg.h>

// pins in order (A, B, C, D, E, F, G, DP)
int segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9};

// digit pins from left to right
int digitPins[] = {10, 11, 12, 13};

Dis7Seg dis('-', segmentPins, 4, digitPins);

// Initialize the 7-segment display
int score[4] = {-1, -1, -1, 0}; // Initialize array with -1, and the ones place with 0

void setup()
{
    Serial.begin(9600); // Initialize serial communication at 9600 baud
}

void loop()
{
    if (Serial.available() > 0)
    {
        int input = Serial.parseInt();  // Read the score sent by the Teensy
        
        // Fill the array with digits from right to left
        for (int i = 3; i >= 0; i--)
        {
            score[i] = input % 10; // Get the last digit
            input /= 10;           // Remove the last digit from input
        }

        // Check for stary 0s and replace with -1
        for (int i = 0; i < 3; i++)
        {
            if (score[i] == 0 && (i == 0 || score[i - 1] == -1))
            {
                score[i] = -1;
            }
        }
    }

    dis.scan(score); // refresh display with given number
                                 // and dots' positions (optional)
}

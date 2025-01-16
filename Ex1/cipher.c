#include "cipher.h"
#include <stdlib.h>
#define FRSTCAPTL 65
#define LSTCPTL  90
#define FRSTSML  97
#define LSTSML  122
#define NUM_LTTR  26
/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.

// See full documentation in header file
void encode (char s[], int k){
  for (int i = 0; s[i] != '\0'; i++)
  {
    int key = (int) s[i] + (k + (abs (k) * NUM_LTTR)) % NUM_LTTR;
    //deals with negative
    if (FRSTSML <= s[i] && s[i] <= LSTSML)//
      // checking if the letter is in the range of the small letters
    {
      int new_val = (int) ((key - FRSTSML) % NUM_LTTR + FRSTSML);
      // modulo of 26 (because there are 26 english letters)
      s[i] = (char) new_val;
    }
    else if (FRSTCAPTL <= s[i] && s[i] <= LSTCPTL)
    {
      int new_val = (int) ((key - FRSTCAPTL) % NUM_LTTR + FRSTCAPTL);
      // checking if the letter is in the range of the capitals letters
      s[i] = (char) new_val;
    }
  }
}

// See full documentation in header file
void decode (char s[], int k){
  for (int i = 0; s[i] != '\0'; i++)
  {
    int key = (int) s[i] - (k - (abs (k) * NUM_LTTR)) % NUM_LTTR;
    if (FRSTSML <= s[i] && s[i] <= LSTSML)
    {
    int new_val = (int)((key - FRSTSML) % NUM_LTTR + FRSTSML);
      s[i] = (char) new_val;
    }
    else if (FRSTCAPTL <= s[i] && s[i] <= LSTCPTL)
    {
      int new_val = (int)((key - FRSTCAPTL) % NUM_LTTR + FRSTCAPTL);
      s[i] = (char)new_val;
    }
  }
}

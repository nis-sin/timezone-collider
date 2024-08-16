## Overview
This is a tool used to show different timezones on a scale from 0000 to 2300 so that the user can see where different timezones overlap.

The program uses IANA timezone format: AREA/LOCATION. Eg: for Morocco timezone, you must type in, Africa/Casablanca or Casablanca (note: search is not case sensitive). There are a few exceptions but, in general, this is the convention to follow.

If you want to see the whole list of available timezones you can type in, type 'all' when prompted to enter a timezone.
---
## Running program
To run the program, first compile it using this command:
```
gcc -o main main.c -lcurl -lcjson -lregex
```
This is because libraries such as, curl (to send and receive HTTP requests to communicate with the API), cjson (to parse JSON) and regex (to match specific string patterns) are used in the program.

Then execute command:
```
./main.exe
```
---
## Special thanks
Thank you to the community maintaining [Time API](https://timeapi.io/swagger/index.html).
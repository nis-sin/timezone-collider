## Overview
This is a tool used to show different timezones on a scale from 0000 to 2300 so that the user can see where different timezones overlap.
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
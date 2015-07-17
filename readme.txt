This is a AI for "五子棋". To write an AI, you need to do the following:

1. Create a file ai*.cpp based on your id. You can use ai1.cpp as an example. 
The function name has to be unique but you can name your AI anything you like.
The prototype has to be exactly the same as exampleAI.

2. Edit makefile, add ai*.cpp in FILES.

3. Edit AI_Name.h:
    -- change MAX_AI_NUM
    -- declare your AI function
    -- add the name in AIList

4. type make in the folder

5. run "./game.out * *", in which * represents the ID for the function.

Example: your ID is 3 and you want to compete with exampleAI, then you should
use "./game.out 1 3". The order of arguments will not affect the result when 
-rand_first:1(which is the default) because who moves first is random. 
However, if you set -rand_first:0, the first input ID will move first.

Now the program supports human vs AI. The human control AI has ID 10. You can
start a game with an AI using "./game.out * 10"

There's a abnormal mode, which can be enabled by -normal:0. In this mode, the 
board will be randomly placed two white pieces and one black piece in 9*9 range 
in the middle, and then the game starts by black. This mode randonmize the 
result for deterministic algorithm. You can test your AI using this mode to see
it's actual situation read ability.

Evaluation mode is available now. You can evaluate an arbitrary number of AIs
in this mode. "./game.out 2 3 7 -eval_enable:1 -eval_print_length:10" will 
evaluate AI number 2 3 7 competing with each other for INT_MAX times and see 
the win rate every 10 rounds. -sleep -print -normal -rand_first do not have 
effects in this mode.

Possible Arguments:
args (default)           | Description
-sleep:* (1)             | sleeptime between moves
-print:* (1)             | whether to print board, only accept 0/1
-normal:* (1)            | whether to play in normal mode, only accept 0/1
-rand_first:* (1)        | whether to random the first play AI, only accept 0/1
-eval_enable:* (0)       | whether to use evaluation mode
-eval_round:* (INT_MAX)  | repeat times in evaluation mode
-eval_print_length:* (1) | rounds for each print in evaluation mode 
-keep_seed:* (0)         | print seed at last for debug
-seed:* (0)              | if used, program will use this seed instead of random
-test_enable:* (0)       | enable validation
-test_file:* ('\0')      | specify a file to validate
-test_folder:* ('\0')    | specify a folder to validate

Current ID assigning:
example:1
gaogaotiantian:2
Beokro:3
jimzhu1993:4
dummyindex:5
ShaoyiZhang:6
SeraphRoy:7
MichaelGuoXY:8
ChristinaXu1012:9
ActualPlayer(Human Control):10

Have Fun!

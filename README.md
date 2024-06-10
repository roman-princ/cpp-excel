# FIT Excel

The `CMakeLists.txt` containts a script for a compilation of the source files **HOW THEY ARE**.  
The python script `makeone.py` is used to combine all files and create a version that is assignable to ProgTest and will pass (the output file will be named `combined.cpp`)  
I included the file but you can try on your own.

If you want to compile the `combined.cpp` file and test it then run:
+ `g++ -std=c++20 -Wall -pedantic -g -o excel combined.cpp -L . -l expression_parser`
+ `./excel`
+ The sample test are included
+ It should pass with a score of 7.2250 on PT (hopefully)

**Thanks for your understanding and happy reviewing :)**
# LibN-N64-SDK
The simplest solution to compiling a fully working ROM with as little as possible in C or C++.

After months of trying, finally got my own custom SDK to work, and is a *header-only* implementation. 
What is here is nothing extraordinary, just a couple days of work and it's features are incredibly rudimentary.  Works fine but when clearing screen, it flickers. Have been trying to work around this with software and hardware alike but can't seem to find a solution. Double buffering works but copying the contents makes everything *dog* slow. 

There are some snippets from LibDragon source, such as the font.h and the font drawing code, among some other code Rasky or Chillywilly may have wrote.
### Example
![LIbN-Example](https://user-images.githubusercontent.com/31579132/151407977-710cf39f-4cd8-4797-a189-cba1f2d7c9fe.png)

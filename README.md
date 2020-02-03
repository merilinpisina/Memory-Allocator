# Memory Allocator in C++ Language
### Data Structures in C++ Homework

## Task 

The library you are implementing in this project must work on a pre-allocated memory block. It must respond to the *alloc \<size\>* and *free \<ptr\>* commands. You need to choose an efficient memory allocation algorithm that works with randomly sized blocks. Validate working with directories (ie, protect yourself against incorrect or double memory freezing). Try working with different algorithms. If you can - demonstrate the usage of the library with standard containers in stl.



---
## Note:

```
_CRT_SECURE_NO_WARNINGS is not a warning, it's a setting that controls whether certain warnings are issed or not.

Most standard C functions are perfectly safe. A few are not, have been considered deprecated by most programmers for decades and cause these warnings. A few are "theorticially possible to abuse (as are all the _s variants...), but almost always used in a safe manner", but also cause this warning.

Take snprintf for example. In 99.99% of cases, the count parameter will contain the size of the buffer being passed in, but apparently that's not "secure" so sprintf_s was created where, in those 99.99% of cases, you're just going to pass in the size twice.

Then there's nonsense like strerror_s which is not only less secure than strerror (it's possible to pass in a short buffer and lie about its length), it's also far slower (it copies the string, rather than just returning a pointer to a fixed table; the standard says that the returned string cannot be modified, the fact that it's not declared const is historical cruft) and for bonus redunancy, it's exactly equivelent to the pre-existing standard (not supported by MS) strerror_r except that the arguments are in a different order...
```
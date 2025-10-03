# Translation units, ODR and how code sharing works in C++ codebases!

## Translation Units

-   Every C++ program is made up of various translation units.
-   A translation unit is nothing but a .c/.cpp file!
-   The linker does the job of knitting these files together
-   All the translation units share the same global scope
-   Translation units allow one to separate code into independent files.
-   But there exists no straight-forward mechanism for code-sharing like in modern programming languages!

## Code Sharing Conundrum

-   ### The Beginning:

    -   `main.cpp`
        ```cpp
        int main() {
            square(10);
            return 0;
        }
        ```
    -   `utils.cpp`

        ```cpp
        #include <iostream>

        void square(int a) {
         std::cout << (a*a) << std::endl;
        }
        ```

    -   The linker would knit these files together but the compiler would thrown an error for it does not know
        where the `square` function is defined in the `main.cpp` translation unit
    -   To make the compiler happy we change `main.cpp` to

        ```diff cpp
        + void square(int a);

        int main() {
            square(10);
            return 0;
        }
        ```

    -   Now we can have a basic code sharing mechanism working!

-   ### The Boom:

    -   Let's say our application grows and we have ten more translation units(or `.cpp` files)
    -   If each one of them needs access to the `square` function, we would need to add `void square(int a);` to each of the file!
    -   This could quickly become an issue if in the future we end up putting in more functions in `utils.cpp`
    -   To solve this, we introduce the header files
    -   `utils.h`

        ```cpp
        void square(int a);
        void cube(int a);
        void identity(int a);
        ```

    -   `utils.cpp`

        ```cpp
        #include <iostream>

        void square(int a) {
            std::cout << (a*a) << std::endl;
        };

        void cube(int a) {
            std::cout << (a*a*a) << std::endl;
        };

        void identity(int a) {
            std::cout << a << std::endl;
        };

        ```

    -   `main.cpp`
        ```cpp
        #include "utils.h"
        int main() {
            square(2);
            return 0;
        }
        ```
    -   `file2.cpp`
        ```cpp
        #include "utils.h"
        void print(int a) {
            identity(a);
        }
        ```
    -   The **declarations** of the util functions are written in `utils.h` but the **definition** is present in `utils.cpp` file
    -   This is the basic mechanism for code-sharing in C++ codebases.

-   ### Why keep `utils.cpp` file at all?

    -   The natural next question is to wonder why not move everything from `utils.cpp` into `utils.h`?
    -   Proposed `utils.h` file:

        ```cpp
        #include <iostream>

        void square(int a) {
            std::cout << (a*a) << std::endl;
        };

        void cube(int a) {
            std::cout << (a*a*a) << std::endl;
        };

        void identity(int a) {
            std::cout << a << std::endl;
        };
        ```

    -   Well (drum-roll) cause the linker would give an error!
    -   The compiler pre-processors would pre-process every file and replace `#include "utils.h"` with its contents
    -   `main.cpp` becomes

        ```diff
        - #include "utils.h"

        + #include <iostream>
        + void square(int a) {
        +   std::cout << (a*a) << std::endl;
        + };
        + void cube(int a) {
        +   std::cout << (a*a*a) << std::endl;
        + };
        + void identity(int a) {
        +   std::cout << a << std::endl;
        + };
        void print(int a) {
            identity(a);
        }
        ```

    -   similarly `file2.cpp` becomes:

        ```diff
        - #include "utils.h"

        + #include <iostream>
        + void square(int a) {
        +   std::cout << (a*a) << std::endl;
        + };
        + void cube(int a) {
        +   std::cout << (a*a*a) << std::endl;
        + };
        + void identity(int a) {
        +   std::cout << a << std::endl;
        + };

        int main() {
            square(2);
            return 0;
        }
        ```

    -   The `compiler` does not mind this, but the `linker` is mad for two files now have two independent yet same **definitions** of the _utils function_.

    -   Recall, all translation units share their global scope, thus having multiple definitions is a problem!

-   ### One Definition Rule
    -   One definition rule clearly defines the rules regarding the multiple `definitions` and `declarations`
    -   We shall use`linkage` directives to solve the above problem whilst obeying `One Defnition Rule`!
    -   The summary of `One Definition Rule`:
        -   Within a single translation unit, only one definition is allowed
        -   In the entire program (i.e across all translation units) one cannot have more than one definition for non-inline functions and variables
        -   Templates, types, classes and structs can have more than one definition across the entire program provided they are the same!

## Linkages and Linkage Directives

-   **Internal linkage** means it is only visible in that translation unit.
-   **External linkage** means visibility across all translation units.
-   If something is defined in local scope, it has **No Linkage**
-   Linkage directives:
    -   `default`(nothing specified) and defined in global scope => external linkage
    -   `static` => internal linkage
    -   `inline` => external linkage
    -   `const` or `constexpr`:
        -   When used along with function it is _still_ external linkage
        -   Else internal linkage
    -   `extern` keyword makes stuff external linkage. Pretty redundant on its own (cause stuff is already external linkage). Useful when used along with `const` ig (in c++ only mostly)
-   **Note: By _stuff_ I mean templates, types, functions, structs, classes. (I haven't cared to study for unions and namespace cases)**

## Solving Code-Sharing with Header files and Linkage Directives (some common patterns)

-   `inline` keyword magic:

    -   `inline` keyword creates external linkage whilst making sure that same definitions of _stuff_ across multiple translation units
        is extracted into one.
    -   This allows one to define stuff in `header` files and #include them various .cpp files (translation units).
    -   Doing so without `inline` keyword would make the linker complain for multiple definitions
    -   Using `static` keyword in this case solves the problem too (makes all functions visible only to their translation unit), but increases memory print, cause every translation unit now has its own copy
    -   `inline` keyword solves both this problem
    -   `utils.h`

        ```cpp
        #include <iostream>

        inline void square(int a) {
            std::cout << (a*a) << std::endl;
        };

        inline void cube(int a) {
            std::cout << (a*a*a) << std::endl;
        };

        inline void identity(int a) {
            std::cout << a << std::endl;
        };
        ```

-   `extern` keyword:

    -   `extern` keyword is used to signify there exists a definition of the `declaration` somewhere.
    -   `utils.h`

        ```cpp
        #include <iostream>

        extern void square(int a) {
            std::cout << (a*a) << std::endl;
        };

        extern void cube(int a) {
            std::cout << (a*a*a) << std::endl;
        };

        extern void identity(int a) {
            std::cout << a << std::endl;
        };
        ```

    -   `utils.cpp`

    ```cpp
        #include <iostream>

        void square(int a) {
            std::cout << (a*a) << std::endl;
        };

        void cube(int a) {
            std::cout << (a*a*a) << std::endl;
        };

        void identity(int a) {
            std::cout << a << std::endl;
        };
    ```

    -   This basically means not to ask the question `Why keep `utils.cpp` file at all?`. Here the `extern` keyword can be dropped too!
    -   Might cause some cluterring as the number of header files increase, but hey it works!


fun fib_recursive(int n) int:
    if n <= 1:
        return n
    return fib_recursive(n - 1) + fib_recursive(n - 2)

fun fib_iterative(int n) int:
    a := 0
    b := 1
    c := 0
    while n > 2:
        c = a + b
        a = b
        b = c
        n = n -1

    return c

fun main():
    fib_iterative(5)
    fib_recursive(5)

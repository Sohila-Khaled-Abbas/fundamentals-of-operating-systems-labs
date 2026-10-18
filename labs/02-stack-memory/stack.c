void func1 () {
    int z = 1;
    z = z + 1;
    return;
}

int main ()
{
    int a = 1;
    int b = 3;
    func1();
    int c = a + b;
    return 0;
}
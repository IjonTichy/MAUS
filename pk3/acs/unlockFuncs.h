function int round(int x)
{
    int y = x >> 16;
    if (x - (y << 16) >= 0.5) { return y + 1; }
    return y;
}

function int floatIntg(int x)
{
    return (x >> 16);
}

function int floatFrac(int x)
{
    return (x << 16) >> 16;
}

function int percFrac(int x)
{
    x = floatFrac(x);
    return floatIntg(x * 100);
}

function int abs(int x)
{
    if (x < 0) { return -x; }
    return x;
}

function int mod(int x, int y)
{
    int ret = x - ((x / y) * y);
    if (ret < 0) { ret = y + ret; }
    return ret;
}

function int pow(int x, int y)
{
    int n = 1;
    while (y-- > 0) { n *= x; }
    return n;
}

function int powFloat(int x, int y)
{
    int n = 1.0;
    while (y-- > 0) { n = FixedMul(n, x); }
    return n;
}

function int min(int x, int y)
{
    if (x < y) { return x; }
    return y;
}

function int max (int x, int y)
{
    if (x > y) { return x; }
    return y;
}

function int middle(int x, int y, int z)
{
    if ((x < z) && (y < z)) { return min(max(x, y), z); }
    return max(min(x, y), z);
}

function int keyUp(int key)
{
    int buttons = GetPlayerInput(-1, INPUT_BUTTONS);

    if (~buttons & key) { return 1; }
    return 0;
}

function int keyDown(int key)
{
    int buttons = GetPlayerInput(-1, INPUT_BUTTONS);

    if (buttons & key) { return 1; }
    return 0;
}

function int keyPressed(int key)
{
    int buttons     = GetPlayerInput(-1, INPUT_BUTTONS);
    int oldbuttons  = GetPlayerInput(-1, INPUT_OLDBUTTONS);
    int newbuttons  = (buttons ^ oldbuttons) & buttons;

    if (newbuttons & key) { return 1; }
    return 0;
}

function int adjustBottom(int tmin, int tmax, int i)
{
    if (i < tmin) { tmin = i; }
    if (i > tmax) { tmin += (i - tmax); }

    return tmin;
}

function int adjustTop(int tmin, int tmax, int i)
{
    if (i < tmin) { tmax -= (tmin - i); }
    if (i > tmax) { tmax = i; }

    return tmax;
}

// Taken from http://zdoom.org/wiki/Sqrt
function int sqrt(int number)
{
    if (number == 1.0) { return 1.0;  }
    if (number <= 0) { return 0; }
    int val = 150.0;
    for (int i=0; i<15; i++) { val = (val + FixedDiv(number, val)) >> 1; }

    return val;
}

function int quadPos(int a, int b, int c)
{
    int s1 = sqrt(FixedMul(b, b)-(4*FixedMul(a, c)));
    int s2 = (2 * a);
    int b1 = FixedDiv(-b + s1, s2);

    return b1;
}

function int quadNeg(int a, int b, int c)
{
    int s1 = sqrt(FixedMul(b, b)-(4*FixedMul(a, c)));
    int s2 = (2 * a);
    int b1 = FixedDiv(-b - s1, s2);

    return b1;
}

function int quadHigh(int a, int b, int c, int x)
{
    c -= x;
    return quadPos(a, b, c);
}

function int quadLow(int a, int b, int c, int x)
{
    c -= x;
    return quadNeg(a, b, c);
}

function int quad(int a, int b, int c, int y)
{
    return FixedMul(a, FixedMul(y, y)) + FixedMul(b, y) + y;
}

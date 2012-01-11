
function int abs(int x)
{
    if (x < 0) { return -x; }
    return x;
}

function int mod(int x, int y)
{
    int ret = x - ((x / y) * y);

    if (ret < 0)
    {
        ret = y + ret;
    }

    return ret;
}

function int pow(int x, int y)
{
    int n = 1;
    while (y-- > 0) { n *= x; }
    
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
    if ((x < z) && (y < z))
    {
        return min(max(x, y), z);
    }

    return max(min(x, y), z);
}

function int keyUp(int key)
{
    int buttons = GetPlayerInput(-1, INPUT_BUTTONS);

    if (~buttons & key)
    {
        return 1;
    }

    return 0;
}

function int keyDown(int key)
{
    int buttons = GetPlayerInput(-1, INPUT_BUTTONS);

    if (buttons & key)
    {
        return 1;
    }

    return 0;
}

function int keyPressed(int key)
{
    int buttons     = GetPlayerInput(-1, INPUT_BUTTONS);
    int oldbuttons  = GetPlayerInput(-1, INPUT_OLDBUTTONS);

    int newbuttons  = (buttons ^ oldbuttons) & buttons;

    if (newbuttons & key)
    {
        return 1;
    }

    return 0;
}

function calc_main(expr)
{
    if (!isFinite(expr) || isNaN(expr))
        return expr;

    if (expr === undefined)
        return NaN;

    expr = +(expr.toPrecision(15));

    return expr;
}

function lg(x)
{
    return Math.log(x) / Math.log(10);
}

function ln(x)
{
    return Math.log(x);
}

function factorial(x)
{
    const sign = (x < 0);

    if (isNaN(x))
        return NaN;

    if (Math.abs(x) > 200 || !isFinite(x))
        return (sign ? -Infinity : Infinity);

    if (x % 1 != 0)
    {
        x = gamma(Math.abs(x)+1);
        return sign ? -x : x;
    }

    let result = 1;

    for (let i = Math.abs(Math.floor(x)); i > 1; --i)
        result *= i;

    return (sign ? -result : result);
}

function gamma(n)
{
    const g = 7;
    const p = [0.99999999999980993, 676.5203681218851, -1259.1392167224028, 771.32342877765313, -176.61502916214059, 12.507343278686905, -0.13857109526572012, 9.9843695780195716e-6, 1.5056327351493116e-7];

    if (n < 0.5)
        return Math.PI / Math.sin(n * Math.PI) / gamma(1 - n);

    --n;
    let x = p[0];

    for (let i = 1; i < p.length; ++i)
        x += p[i] / (n + i);

    const t = n + g + 0.5;
    return Math.sqrt(2*Math.PI) * Math.pow(t, n+0.5) * Math.exp(-t) * x;
}


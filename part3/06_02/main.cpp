import std;
import Math;

using MyLib::Rational;

bool equal(double x, double y, double epsilon = 1e-6)
{
    return std::abs(x - y) < epsilon;
}

int main()
{
    Rational x = 1, y(2, 1);

    // ----------------------------------------------------------------

    if (!equal(static_cast<double>(x), 1.0)) 
    {
        std::cerr << "Cast double failed\n";
        return 1;
    }

    auto sum = x + y;
    if (sum != Rational(3, 1)) 
    {
        std::cerr << "Addition failed\n"; 
        return 1;
    }

    x += y;
    if (x != Rational(3, 1)) return 1;

    Rational a(1, 2);
    Rational b(3, 4);
    
    if ((a <=> b) != std::strong_ordering::less) 
    {
        std::cerr << "Spaceship operator failed\n";
        return 1;
    }

    if (a >= b) 
    {
        std::cerr << "Comparison failed\n";
        return 1;
    }

    std::stringstream ss;
    ss << Rational(1, 2);
    if (ss.str() != "1/2") 
    {
        std::cerr << "Output failed\n";
        return 1;
    }

    Rational from_stream;
    ss >> from_stream;
    if (from_stream != Rational(1, 2))
    {
        std::cerr << "Input failed\n";
        return 1;
    }

    std::cout << "All tests passed successfully (Modules version)!\n";
    return 0;
}
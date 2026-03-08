//@compile g++ part4/08_04.cpp -std=c++20 -Wall -Wextra -Wpedantic
#include <iostream>
#include <random>
#include <string>

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    const std::string target        = "methinksitislikeaweasel";
    const int         copies        = 100;
    const double      mutation_rate = 0.05;

    std::random_device              rd;
    std::default_random_engine      engine(rd());
    std::uniform_int_distribution   letter_dist('a', 'z');
    std::uniform_real_distribution  prob_dist(0.0, 1.0);

    //  -------------------------------------------------------------------------------------------
    // generate initial string of 23 random letters

    std::string current(target.size(), ' ');
    for (auto & c : current)
        c = static_cast<char>(letter_dist(engine));

    //  -------------------------------------------------------------------------------------------
    // evolution loop

    for (int generation = 0; ; ++generation)
    {
        std::cout << generation << ": " << current << '\n';

        std::string best;
        int best_metric = static_cast<int>(target.size()) + 1;

        for (int i = 0; i < copies; ++i)
        {
            std::string candidate = current;

            for (auto & c : candidate)
                if (prob_dist(engine) < mutation_rate)
                    c = static_cast<char>(letter_dist(engine));

            int metric = 0;
            for (auto j = 0uz; j < target.size(); ++j)
                if (candidate[j] != target[j]) ++metric;

            if (metric < best_metric)
            {
                best_metric = metric;
                best        = candidate;
            }
        }

        if (best_metric == 0) break;

        current = best;
    }

    std::cout << "done: " << target << '\n';
}

///////////////////////////////////////////////////////////////////////////////////////////////

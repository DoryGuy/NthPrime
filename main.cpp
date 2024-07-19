//
//  main.cpp
//  NthPrime
//
//  Created by Gary Powell on 6/16/19.
//  Copyright © 2019 Guest User. All rights reserved.
//

#include <iostream>
#include <deque>
#include <iterator>
#include <chrono>


// this will grow in size as we discover new primes.
std::deque<int> known_primes = { 1, 2, 3, 5, 7 };

int computeTheNextPrime(int prime_candidate)
{
    using std::advance;
    // the largest known prime that can be a factor is index at 1/2 the known set
    // because anything larger would be 2 x N and that's already not possible.
    // ie 11/7 will never be a factor. nor 13/7
    auto last_possible_index = known_primes.begin();
    advance(last_possible_index,((known_primes.size() + 1)/2));

// we violate the no goto's rule here because we want extreme speed. big primes are expensive to compute.
    try_value:
        prime_candidate += 2; // as all known primes are odd, skip all the even ones.
        auto iter = known_primes.begin();
        advance(iter, 2); // skip 1 and 2
        do {
            if (prime_candidate % *iter == 0) // is it a factor?
                goto try_value;
            ++iter;
        } while (iter != last_possible_index); // use a do while to avoid extra test of the range.

    return prime_candidate;
}

int computeNthPrime(std::deque<int>::size_type  n)
{
    using std::cout;
    using std::endl;
    
    while ( known_primes.size() <= n) {
        auto new_prime = computeTheNextPrime(known_primes.back());
        known_primes.emplace_back(new_prime);
        // debugging message so we know it's doing something!
        // cout << "Computed new prime " << n << " " <<  new_prime << endl;
    }
    return known_primes[n];
}

int main() {
    using std::array;
    using std::cout;
    using std::endl;
    using Clock=std::chrono::high_resolution_clock;
    
    auto t1 = Clock::now();
    
    // make test cases...
    static constexpr array<int,79> answers = { 1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
     67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157,
      163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
      283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397,
    };
    std::deque<int>::size_type i{0};
    for (auto answer_iter = answers.begin(); answer_iter != answers.end(); ++answer_iter) {
        // cout << "i " << i << endl;
        const auto prime = computeNthPrime(i);
        ++i;
        if (prime != *answer_iter) {
            cout << "Prime found \"" << prime << "\" does not match expected answer \"" << *answer_iter << "\"" << endl;
        } else {
            //cout << "Prime found \"" << prime << "\" matched expected answer!" << endl;
        }
    }
    auto t2 = Clock::now();
    cout << "Delta t2-t1: "
    << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()    << " nanoseconds" << std::endl;
    std::cout << "Done!\n";
    return 0;
}

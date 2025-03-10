#include <iostream>
#include <iterator>
#include <list>

int main()
{
    std::list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.push_back(4);

    auto itr = l.begin();

	std::cout << *itr << std::endl;

    itr = std::next(itr);

    std::cout << *itr << std::distance(l.begin(), itr) << std::endl;

	itr = l.end();
    std::cout << *itr << std::distance(l.begin(), itr) << std::endl;

	itr = std::next(itr);
    std::cout << *itr << std::distance(l.begin(), itr) << std::endl;

    return 0;
}

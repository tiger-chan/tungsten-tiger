#include <iostream>
#include "tungsten_tiger.hpp"

#include "specs.hpp"

void tungsten_tiger(){
	if constexpr (tt::spec.debug) {
    	std::cout << "TungstenTiger/0.1.0: Debug\n";
	}
	else {
    	std::cout << "TungstenTiger/0.1.0: Release\n";
	}

	std::cout << "- TungstenTiger/0.1.0: Architecture " << tt::spec.arch << "\n";

	std::cout << "- TungstenTiger/0.1.0: Compiler: " << tt::spec.compiler << "\n";
	std::cout << "- TungstenTiger/0.1.0: Compiler version: " << tt::spec.ver << "\n";

	std::cout << "- TungstenTiger/0.1.0: CXX: " << tt::spec.cxx << "\n";
}

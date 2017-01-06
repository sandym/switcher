//
//  main.cpp
//  switch
//
//  Created by Sandy Martel on 5/1/17.
//  Copyright © 2017 sandy. All rights reserved.
//

#include <iostream>
#include "switch.h"

int main(int argc, const char * argv[])
{
	std::string username = "guest";
	bool passwordIsValid = true;
	
	Switch( username, passwordIsValid )
		.Case( "admin", true,
			[]()
			{
				std::cout << "Welcome chief" << std::endl;
			} )
		.Case( "guest", std::ignore,
			[&passwordIsValid]()
			{
				std::cout << "Welcome guest, your password is " << (passwordIsValid ? "valid" : "invalid" ) << std::endl;
			} )
		.Default( [&username]()
			{
				std::cout << username << ", you're at the wrong place!" << std::endl;
			} );
	
    return 0;
}

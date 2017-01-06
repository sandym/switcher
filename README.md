# C++ multi-switch implementation

Requires C++14, tested with gcc, clang and VS 2015.

```c++
#include "switch.h"

std::string username = ...;
bool passwordIsValid = ...;

Switch( username, passwordIsValid )
	.Case( "admin", true,
		[](){ /* admin user with valid password */ } )
	.Case( "guest", std::ignore,
		[](){ /* guest user, valid or invalid password */ } )
	.Default( [](){ /* default case */ } );
```

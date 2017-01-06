# C++ multi-switch implementation

```c++
#include "switch.h"

Switch( username, passwordIsValid )
	.Case( "admin", true,
		[](){ /* admin user with valid password */ } )
	.Case( "guest", std::ignore,
		[](){ /* guest user, valid or invalid password */ } )
	.Default( [](){ /* default case */ } );
```

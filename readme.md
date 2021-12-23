# dolboeb-executor
Arbitrary code execution inside of vulnerable driver

## How's this works?
Dolboeb-executor will replace a function inside vulnerable driver to a handler: \
![img](https://i.imgur.com/vaQJdIj.png) \
Then it will trigger that function(with ioctl for example) and this handler will execute our UM callback

## How can i customize it?
This solution have a PhyMem example out of the box. \
You can use any driver that you want. \
Dolboeb-executor have an abstract interface to create your own 'exploits' classes. \
![img](https://i.imgur.com/dyYAJFp.png) \
The PhyMem example overrides it like this. \
![img](https://i.imgur.com/o6DwUmr.png) \
A little explanation about how this phymem exploit works: \
Well nothing interesting, it just passes IoCtl with map/unmap codes. \
Then reads or writes to the mapped regions. \
Function that this rva points to at line n70 is the IoCTL handler. \
So we'll patch it to our stub and then call it via IoCTL.\
 \
ezpz :)
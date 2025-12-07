An implementation of jlox & clox in C++ following along with Robert Nystrom's book, [Crafting Interpreters](https://craftinginterpreters.com/)

### Supported Features
The jlox implementation has no additional features from what was mentioned in the book, it's pure lox as specified.  
<br/>
The clox implementation is a work in progress and I'm planning on implementing the challenge features that I had previously implemented in jlox there, as well as whatever is specified in the upcoming challenges.

### Bugs
##### `jlox`
- printing the result of multiplication and division expressions result in a "bad any_cast" exception, likely at the "stringify" function in the interpreter class
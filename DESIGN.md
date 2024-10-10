## Design Questions

Answer each of these with a few sentences or paragraphs; don't write a whole essay, but use good writing practice to communicate the essence of the idea. A good response doesn't need to be long, but it needs to have attention to detail and be clear. Examples help!

- Another encoding of Unicode is UTF-32, which encodes *all* Unicode code points in 4 bytes. For things like ASCII, the leading 3 bytes are all 0's. What are some tradeoffs between UTF-32 and UTF-8?

Answer:
First, you will have more memory and storege use for every charactoer you use. And it has to run on a system that nativly use UTF-32. But you will have simpler indexing for the storege. 
UTF-8 will have more efficient storege use. 

- UTF-8 has a leading `10` on all the bytes past the first for multi-byte code points. This seems wasteful – if the encoding for 3 bytes were instead `1110XXXX XXXXXXXX XXXXXXXX` (where `X` can be any bit), that would fit 20 bits, which is over a million code points worth of space, removing the need for a 4-byte encoding. What are some tradeoffs or reasons the leading `10` might be useful? Can you think of anything that could go wrong with some programs if the encoding didn't include this restriction on multi-byte code points?

Answer:
When we use '10' as a prefix for the code in UTF-8. We can easily maintaining backward compatibility with ASCII and existing systems. If we remove the '10' as a prefix, we can get error messages that runs ascii code machine. In short, it will be a total mess. 
## Design Questions
What code point does it encode in UTF-8, and what character is that?
Answer:
It's null character U+0000. In character it's 0x00. 
What are the three other ways to encode that character?
Answer:
We can use two byte encoding: 11000000 10000000, three byte encoding: 11100000 10000000 10000000, four byte encoding: 11110000 10000000 10000000 10000000
Give an example of a character that has exactly three encodings (but not four, like the one in the previous example does)
Answer:
Letter 'A'. It can be 01000001 in one byte, 11000000 10000001 in two byte, 11100000 10000000 10000001 in three byte. 
What are some problems with having these multiple encodings, especially for ASCII characters? A web search for “overlong UTF-8 encoding” may be useful here.
Answer:
First, it can valiate the UTF-8 encodeing priciple. Overlong encoding can also cause different character storing inconsistencies in data storage and retrieval, leading to errors in text processing and comparison operations. Programs that do not handle overlong encodings correctly may misinterpret data, causing failures in communication protocols or data corruption. 































# PA1 - UTF-8

Representing text is straightforward using ASCII: one byte per character fits well within `char*` and it represents most English text. However, there are many more than 256 characters in the text we use, from non-Latin alphabets (Cyrillic, Arabic, and Chinese character sets, etc.) to emojis and other symbols like €, to accented characters like é and ü.

The [UTF-8 encoding](https://en.wikipedia.org/wiki/UTF-8#Encoding) is the default encoding of text in the majority of software today.
If you've opened a web page, read a text message, or sent an email [in the past 15 years](https://en.wikipedia.org/wiki/UTF-8#/media/File:Unicode_Web_growth.svg) that had any special characters, the text was probably UTF-8 encoded.

Not all software handles UTF-8 correctly! For example, I got a marketing email recently with a header “Take your notes further with Connectâ€‹” I'm guessing that was supposed to be an ellipsis (…), [UTF-8 encoded as the three bytes 0xE2 0x80 0xA6](https://www.compart.com/en/unicode/U+2026), and likely the software used to author the email mishandled the encoding and treated it as [three extended ASCII characters](https://en.wikipedia.org/wiki/Extended_ASCII).

This can cause real problems for real people. For example, people with accented letters in their names can run into issues with sign-in forms (check out Twitter/X account [@yournameisvalid](https://x.com/yournameisvalid?lang=en) for some examples). People with names best written in an alphabet other than Latin can have their names mangled in official documents, and need to have a "Latinized" version of their name for business in the US.

UTF-8 bugs can and do cause security vulnerabities in products we use every day. A simple search for UTF-8 in the CVE database of security vulnerabilities turns up [hundreds of results](https://cve.mitre.org/cgi-bin/cvekey.cgi?keyword=utf-8).

It's useful to get some experience with UTF-8 so you understand how it's supposed to work and can recognize when it doesn't.
To that end, you'll write several functions that work with UTF-8 encoded text, and use them to analyze some example texts.

## Functions - Milestone 1


### `uint8_t is_ascii(char str[])`

Takes a UTF-8 encoded string and returns if it is valid ASCII (e.g. all bytes are 127 or less).

#### Example Usage: 
```
printf("Is 🔥 ASCII? %d\n", is_ascii("🔥"));

=== Output ===
Is 🔥 ASCII? 0

printf("Is abcd ASCII? %d\n", is_ascii("abcd"));

=== Output ===
Is abcd ASCII? 1
```

### `int32_t capitalize_ascii(char str[])`

Takes a UTF-8 encoded string and *changes* it in-place so that any ASCII lowercase characters `a`-`z` are changed to their uppercase versions. Leaves all other characters unchanged. It returns the number of characters updated from lowercase to uppercase.

#### Example Usage: 
```
int32_t ret = 0;
char str[] = "abcd";
capitalize_ascii(str);
printf("Capitalized String: %s\n Characters updated: %d\n", str, ret);`

=== Output ===
Capitalized String: ABCD
Characters updated: 4
```

## Functions - Milestone 2

### `int32_t width_from_start_byte(char start_byte)`

Given the start byte of a UTF-8 sequence, return how many bytes it indicates the sequence will take (start byte + continuation bytes).

Returns 1 for ASCII characters, and -1 if byte is not a valid start byte.

#### Example Usage:
```
char s[] = "Héy"; // same as { 'H', 0xC3, 0xA9, 'y', 0 },   é is start byte + 1 cont. byte
printf("Width: %d bytes\n", width_from_start_byte(s[1])); // start byte 0xC3 indicates 2-byte sequence

=== Output ===
Width: 2 bytes

printf("Width: %d bytes\n", width_from_start_byte(s[2])); // start byte 0xA9 is an invalid start byte

=== Output ===
Width: -1
```

### `int32_t utf8_strlen(char str[])`

Takes a UTF-8 encoded string and returns the number of UTF-8 codepoints it represents.

#### Example Usage:
```
char str[] = "Joséph";
printf("Length of string %s is %d\n", str, utf8_strlen(str));  // 6 codepoints, (even though 7 bytes)

=== Output ===
Length of string Joséph is 6
```

### `int32_t codepoint_index_to_byte_index(char str[], int32_t cpi)`

Given a UTF-8 encoded string, and a codepoint index, return the byte index in the string where the Unicode character at the given codepoint index starts. 

Example: 
```
char str[] = "Joséph";
printf("Cocodepoint_index_to_byte_index("Joséph", 4) == 5;  // 4th codepoint refers character 'p'
```

### `void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[])`

Takes a UTF-8 encoded string and start(inclusive) and end(exclusive) codepoint indices, and writes the substring between those indices to `result`, with a null terminator. Assumes that `result` has sufficient bytes of space available. (Hint: `result` will be created beforehand with a given size and passed as input here. Can any of the above functions be used to determine what the size of `result` should be?)

Example:
```
  utf8_substring("🦀🦮🦮🦀🦀🦮🦮", 3, 7, result) // these emoji are 4 bytes long
  result == 🦀🦀🦮🦮
```

## Functions - Milestone 3



### `uint32_t codepoint_at(char str[], int32_t cpi)`

Takes a UTF-8 encoded string and a codepoint index, and returns a decimal representing the codepoint at that index.

Example:
```
  codepoint_at("Joséph", 4) == 112  // 'p' is the 4th codepoint
```

### `uint8_t is_animal_emoji_at(char str[], int32_t cpi)`

Takes a UTF-8 encoded string and an codepoint index, and returns if the code point at that index is an animal emoji.

For simplicity for this question, we will define that that the “animal emojii” are in two ranges: from 🐀 to 🐿️ and from 🦀 to 🦮. (Yes, this technically includes things like 🐽 which are only related to or part of an animal, and excludes a few things like 🙊, 😸, which are animal faces.). You may find the [wikipedia page on Unicode emoji](https://en.wikipedia.org/wiki/List_of_emojis) helpful here.



## UTF-8 Analyzer

You'll also write a program that reads UTF-8 input and prints out some information about it.

Here's what a sample run of your program should look like:

```
$ ./utf8analyzer
Enter a UTF-8 encoded string: My 🐩’s name is Erdős.
Valid ASCII: false
Uppercased ASCII: "MY 🐩’S NAME IS ERDőS."
Length in bytes: 27
Number of code points: 21
Bytes per code point: 1 1 1 4 3 1 1 1 1 1 1 1 1 1 1 1 1 1 2 1 1
Substring of the first 6 code points: "My 🐩’s"
Code points as decimal numbers: 77 121 32 128041 8217 115 32 110 97 109 101 32 105 115 32 69 114 100 337 115 46
Animal emojis: 🐩
```

You can also test the contents of _files_ by using the `<` operator:

```
$ cat utf8test.txt
My 🐩’s name is Erdős.
$ ./utf8analyzer < utf8test.txt
Enter a UTF-8 encoded string: 
Valid ASCII: false
Uppercased ASCII: MY 🐩’S NAME IS ERDőS.
Length in bytes: 27
Number of code points: 21
Bytes per code point: 1 1 1 4 3 1 1 1 1 1 1 1 1 1 1 1 1 1 2 1 1
Substring of the first 6 code points: "My 🐩’s"
Code points as decimal numbers: 77 121 32 128041 8217 115 32 110 97 109 101 32 105 115 32 69 114 100 337 115 46
Animal emojis: 🐩
```

## Testing
We provide 2 basic tests in the `tests` folder - which contain simple tests for detecting if there are errors in your code while identifying valid ASCII and converting ASCII lowercase to uppercase characters. We have provided a test bash file that checks if your program output contains each line in the .expect file. You can use the following commands to run the tests (You may need to change the permission of the `test_script` file to be executable with the command `chmod u+x test_script`.):
```
gcc *.c -o utfanalyzer // compiles your C code into an executable called utfanalyzer
./test_script utfanalyzer
```
Then it will print out result in your terminal. 

Here are some other ideas for tests you should write. They aren't necessarily comprehensive (you should design your own!) but they should get you started. For each of these kinds of strings, you should check how UTF-8 analyzer handles them:

- Strings with a single UTF-8 character that is 1, 2, 3, 4 bytes
- Strings with two UTF-8 characters in all combinations of 1/2/3/4 bytes. (e.g. `"aa"`, `"aá"`, `"áa"`, `"áá"`, and so on)
- Strings with and without animal emojii, including at the beginning, middle, and end of the string, and at the beginning, middle, and end of the range
- Strings of exactly 5 characters

## Design Questions

Answer each of these with a few sentences or paragraphs; don't write a whole essay, but use good writing practice to communicate the essence of the idea. A good response doesn't need to be long, but it needs to have attention to detail and be clear. Examples help!

- Another encoding of Unicode is UTF-32, which encodes *all* Unicode code points in 4 bytes. For things like ASCII, the leading 3 bytes are all 0's. What are some tradeoffs between UTF-32 and UTF-8?

- UTF-8 has a leading `10` on all the bytes past the first for multi-byte code points. This seems wasteful – if the encoding for 3 bytes were instead `1110XXXX XXXXXXXX XXXXXXXX` (where `X` can be any bit), that would fit 20 bits, which is over a million code points worth of space, removing the need for a 4-byte encoding. What are some reasons this might be useful? Can you think of anything that could go wrong with some programs if the encoding didn't include this restriction on multi-byte code points?


## What to Hand In

- Any `.c` files you wrote (can be one file or many). We will run `gcc *.c -o utfanalyzer` to compile your code, so you should make sure it works when we do that.
- A file `DESIGN.md` (with exactly that name) containing the answers to the design questions
- Your tests with expected output in files `tests/*.txt`, `tests/*.txt.expect`

Hand in to the `pa1` assignment on Gradescope. The submission system will show you the output of compiling and running your program on the test input described above to make sure the baseline format of your submission works. You will not get feedback about your overall grade before the deadline.

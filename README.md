# bbcpp

## Introduction

bbcpp is a C++ library for parsing BBCode, or Bulletin Board Code, a markup language used to format posts in many message boards.

This library primarily focuses on parsing the BBCode into a tree data structure that can be used for formatted output. This library does not include any output classes, though a basic HTML output class will likely be included. 

## Usage

    auto doc = BBDocument::create();
    doc->load("This is [b]an example[/b] of some text.");

## Special Tags

### `STYLE`

### `COLOR`

### `LIST`/`[*]`

### `IMG`

### `URL`

### `QUOTE`

The **bbcpp** parser will accept three different formats for the `QUOTE` tag:

1. `[QUOTE user=Username postid=1234]`: A key-value pair of values. In theory they are space delimited unless quoted. (Used with phpBB)
1. `[QUOTE="username, post: 1799684, member: 11733"]`: Another key-value pair format except the first argument is assumed to be the username. (Used with XenForo)   
1. `[QUOTE=Username;1234]`: `Username` is the name of the user being quoted and `1234` is the postid. (Used with vBulletin)

## BBNode Tree

The following are examples of the node tree built during parsing.

#### Example 1

> `This is [b]an example[/b] of some text`

```
#document
│-- @"This is"
│-- [b]  
│   │-- @"an example"
│   │-- [/b]
│-- @"of some text"
```

#### Example 2

> `[QUOTE]This is [b]important[/b] news![/QUOTE]` <br/><br/>
> `Indeed it is!`

```
#document
│-- [QUOTE]  
│   │-- @"This is " 
│   │-- [b]
|   |   |-- @"important"
|   |   |-- [/b]
│   │-- @"news!"
│   │-- [/QUOTE]
│-- @"\n\nIndeed it is!"
```

#### Example 3
> `[QUOTE user=Joe userid=1 postid=1234]This is another quote![/QUOTE]`<br/><br/>
> `I'm quoting you!`

```
#document
│-- [QUOTE]
|   |-- {user=Joe}
|   |-- {userid=1}
|   |-- {postid=1234} 
│   │-- @"This is another quote!"
│   │-- [/QUOTE]
│-- @"\n\nI'm quoting you!"
```

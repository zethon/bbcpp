# bbcpp

## Introduction

bbcpp is a C++ library for parsing BBCode, or Bulletin Board Code, a markup language used to format posts in many message boards.

## Usage
    auto doc = BBDocument::create();
    doc->load("This is [b]an example[/b] of some text.");

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

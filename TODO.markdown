# Directions to go

## Moving between documents

## QNames

## Root node
Barely exists!

## Namespaces

## XPath

### Node-type functions
- comment()
- processing-instruction(arg)
### Axes (easy)
- ancestor
- ancestor-or-self
- following
- following-sibling
- preceding
- preceding-sibling
### Axes (hard)
- namespace

conversion of xpath values [string(), number()]

nodeset equality and relational

more functions

nodeset should perform set, union (and ordering?)

grouping parens (resets axis order!)

tokenizer should not include space as part of a string/qname

# Bugs
Evaluating steps when there is no context node (both xpath-step && expression-path?)
Comments are not output at all

# Ideas

Add a visitor to the expression hierarchy.
- Could make the parser tests more focused
- Assert that a specific expression is found
- Could be used for printing the expression back to XPath
- Could be eventually used for optimization?

Generate random valid expression trees for performance testing?
- Somehow need to leverage the structure in the parser...

No need for 'has_function' - just return null?

objectize the axes - have them know axis name and just do a hashtable

Unify node-test-{element,attribute}
They both test for the "name" of something, but have axis type...

# Big ideas

Command-line streaming / filtering tool
fully reads in a "top-level" element, runs xpath on it. true means output it

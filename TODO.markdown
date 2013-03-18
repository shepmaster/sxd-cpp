# Directions to go

## Moving between documents

## QNames

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

filter expr

grouping parens (resets axis order!)

# Bugs
Evaluating steps when there is no context node (both xpath-step && expression-path?)
Comments are not output at all

# Ideas

No need for 'has_function' - just return null?

objectize the axes - have them know axis name and just do a hashtable

Unify node-test-{element,attribute}
They both test for the "name" of something, but have axis type...

# Big ideas

Command-line streaming / filtering tool
fully reads in a "top-level" element, runs xpath on it. true means output it

Exuberant Ctags (extended)
==============
This a branch of Exuberant Ctags, the original code comes from:
<http://ctags.sourceforge.net>, see the `README` in the project for more
details.

This project, inherited from the original, follows the GNU General Public License

> It supports the following languages: Assembler, AWK, ASP, BETA,
> Bourne/Korn/Z Shell, C, C++, C#, COBOL, Eiffel, Erlang, Fortran, Java, Lisp,
> Lua, Makefile, Pascal, Perl, PHP, PL/SQL, Python, REXX, Ruby, Scheme,
> S-Lang, SML (Standard ML), Tcl, Vera, Verilog, VHDL, Vim, and YACC.

Besides above, this branch implemented a <font color=#ff0000>**markdown**</font>
module, which extracts the headings of the markdown doc. Cooperated with vim and
taglist, a hiarchie of the document can be displayed as below:

![markdown-taglist.png](markdown-taglist.png)

A file called `markdown.c` is added to the original project is what I have done.

## How to define a new language in Ctags

new language defined with a new .c file say new_language.c

file to modify: 
1. source.mak

	source

		...
		yacc.c \
		new_language.c \
		vstring.c
		...

	object

		yacc.$(OBJEXT) \
		new_language.$(OBJEXT) \
		vstring.$(OBJEXT)

2. parser.h

	add new_language parser

		...
		VhdlParser, \
		VimParser, \
		YaccParser, \
		NewLanguageParser

2. taglist.vim

	taglist parse catags' output

		" vim language
		let s:tlist_def_vim_settings =
								\ 'vim;v:variable;a:autocmds;c:commands;m:map;f:function'

		" yacc language
		let s:tlist_def_yacc_settings = 'yacc;l:label'

		" markdown language
		let s:tlist_def_markdown_settings = 'markdown;h:heading'

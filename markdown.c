
/*
*  $Id: markdown.c 485 2015-08-22-Sat 00:01:10 dfishburn $
*
*  Copyright (c) 2000-2015, Gavin Chou
*
*  This source code is released for free distribution under the terms of the
*  GNU General Public License.
*
*  Thanks are due to Jay Glanville for significant improvements.
*
*  This module contains functions for generating tags for user-defined
*  functions for the Markdown syntax analyzing.
*/

/*
*  INCLUDE FILES
*/
#include "general.h"  /* must always come first */

#include <string.h>
#include <setjmp.h>
#ifdef DEBUG
#include <stdio.h>
#endif


#include "parse.h"
#include "read.h"
#include "vstring.h"

#if 0
typedef struct sLineInfo {
  tokenType  type;
  keywordId  keyword;
  vString *  string;
  vString *  scope;
  unsigned long lineNumber;
  fpos_t filePosition;
} lineInfo;
#endif

/*
*  DATA DEFINITIONS
*/
typedef enum {
  K_HEADING,
} markdownKind;

static kindOption MarkdownKinds [] = {
  { TRUE,  'h', "heading",  "markdwon heading" },
};

const int MAX_HEADING_LEVEL = 6;
// max display heading length
const int MAX_HEADING_LENGTH = 200;

/*
 *   DATA DECLARATIONS
 */

#if 0
typedef enum eException {
  ExceptionNone, ExceptionEOF 
} exception_t;
#endif

/*
 *  DATA DEFINITIONS
 */

#if 0
static jmp_buf Exception;
#endif

/*
 *  FUNCTION DEFINITIONS
 */

static boolean isHeading(const unsigned char* line) {
  if (strncmp((const char*) line, "#", (size_t) 1) == 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

// heading line check should be done first
// this function cannot be called to parse non heading line
static void parseHeading(const unsigned char* line) {
  vString* name = vStringNew();
  size_t lineLen = strlen(line);
  // the first char is already '#'
  const unsigned char* cp = line + 1;
  // at least level 1 due to
  int level = 1;
  while (level <= MAX_HEADING_LEVEL) {
    if (*cp == '#') {
      ++cp;
      ++level;
      // current level
//       if (level == 2) {
//         vStringPut(name, (int) ' ');
//       } else if (level > 2) {
//         vStringPut(name, (int) ' ');
//         vStringPut(name, (int) ' ');
//       }

      if (level > 2) {
        vStringPut(name, (int) ' ');
        vStringPut(name, (int) ' ');
      }
    } else if (*cp == ' ' || *cp == '\t') {
      ++cp;
    } else {
      break;
    }
  }

  size_t offset = cp - line;
  boolean isHtmlTag = FALSE;
  // here is a trick: when *cp != '<', cp + 1 wont be accessed, hence there wont be 
  // any illegal address
  if (*cp == '<' && *(cp + 1) == 'a' && lineLen - offset > 7) {
    int i = 7;
    for (; *(cp + i) != '\0'; ++i) {
      if (*(cp + i - 3) == '<' && *(cp + i - 2) == '/' && *(cp + i - 1) == 'a' && *(cp + i) == '>') {
        isHtmlTag = TRUE;
        ++i;
        break;
      }
    }
    if (isHtmlTag) {
      cp += i;
    }
  }
  int i;
  for (i = 0; i < MAX_HEADING_LENGTH && *cp != '\0'; ++i, ++cp) {
    vStringPut(name, (int) *cp);
  }

  vStringTerminate(name);
  makeSimpleTag(name, MarkdownKinds, K_HEADING);
  vStringClear(name);
}

static const unsigned char * readMarkdownLine (void)
{
  const unsigned char *line;

  // fileReadLine is predefined by ctags
  line = fileReadLine();

// deal with comment
//   while ((line = fileReadLine ()) != NULL)
//   {
//     while (isspace ((int) *line))
//       ++line;
// 
//     if ((int) *line == '"')
//       continue;  /* skip comment */
// 
//     break;
//   }

  return line;
}

static boolean parseMarkdownLine (const unsigned char *line)
{
  boolean readNextLine = TRUE;

  if (isHeading(line))
  {
    parseHeading(line);
  }

  return readNextLine;
}

static void parseMarkdownFile (const unsigned char *line)
{
  boolean readNextLine = TRUE;
  line = readMarkdownLine();

  while (line != NULL)
  {
    readNextLine = parseMarkdownLine(line);

    if ( readNextLine )
      line = readMarkdownLine();

  }
}

static void findMarkdownTags (void)
{
  const unsigned char *line;
    /* TODO - change this into a structure */

  line = "";

  parseMarkdownFile (line);
}

// ctags structure
extern parserDefinition* MarkdownParser (void)
{
  static const char *const extensions [] = { "md", "markdown", NULL };
  parserDefinition* def = parserNew ("Markdown");
  def->kinds    = MarkdownKinds;
  def->kindCount  = KIND_COUNT (MarkdownKinds);
  def->extensions = extensions;
  def->parser    = findMarkdownTags;
  return def;
}

/* vim:set tabstop=2 shiftwidth=2 expandtab: */

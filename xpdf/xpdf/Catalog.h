//========================================================================
//
// Catalog.h
//
// Copyright 1996-2003 Glyph & Cog, LLC
//
//========================================================================

#ifndef CATALOG_H
#define CATALOG_H

#include <aconf.h>

#include <set>

#ifdef USE_GCC_PRAGMAS
#pragma interface
#endif

class XRef;
class Object;
class Page;
class PageAttrs;
struct Ref;
class LinkDest;
class UGString;

//------------------------------------------------------------------------
// NameTree
//------------------------------------------------------------------------

class NameTree {
public:
  NameTree();
  void init(XRef *xref, Object *tree);
  void parse(Object *tree);
  GBool lookup(UGString *name, Object *obj);
  void free();

private:
  struct Entry {
    Entry(Array *array, int index);
    ~Entry();
    UGString *name;
    Object value;
    void free();
    static int cmp(const void *key, const void *entry);
  };

  void addEntry(Entry *entry);

  XRef *xref;
  Object *root;
  Entry **entries;
  int size, length;
};

//------------------------------------------------------------------------
// Catalog
//------------------------------------------------------------------------

class Catalog {
public:

  enum PageMode {
    UseNone,
    UseOutlines,
    UseThumbs,
    FullScreen,
    UseOC
  };

  // Constructor.
  Catalog(XRef *xrefA);

  // Destructor.
  ~Catalog();

  // Is catalog valid?
  GBool isOk() { return ok; }

  // Get number of pages.
  int getNumPages() { return numPages; }

  // Get a page.
  Page *getPage(int i) { return pages[i-1]; }

  // Get the reference for a page object.
  Ref *getPageRef(int i) { return &pageRefs[i-1]; }

  // Return base URI, or NULL if none.
  GString *getBaseURI() { return baseURI; }

  // Returns the page mode.
  PageMode getPageMode() { return pageMode; }

  // Return the contents of the metadata stream, or NULL if there is
  // no metadata.
  GString *readMetadata();

  // Return the structure tree root object.
  Object *getStructTreeRoot() { return &structTreeRoot; }

  // Find a page, given its object ID.  Returns page number, or 0 if
  // not found.
  int findPage(int num, int gen);

  // Find a named destination.  Returns the link destination, or
  // NULL if <name> is not a destination.
  LinkDest *findDest(UGString *name);

  Object *getOutline() { return &outline; }

  Object *getAcroForm() { return &acroForm; }

private:

  XRef *xref;			// the xref table for this PDF file
  Page **pages;			// array of pages
  Ref *pageRefs;		// object ID for each page
  int numPages;			// number of pages
  int pagesSize;		// size of pages array
  Object dests;			// named destination dictionary
  NameTree destNameTree;	// name tree
  GString *baseURI;		// base URI for URI-type links
  PageMode pageMode;  // page mode
  Object metadata;		// metadata stream
  Object structTreeRoot;	// structure tree root dictionary
  Object outline;		// outline dictionary
  Object acroForm;		// AcroForm dictionary
  GBool ok;			// true if catalog is valid

  int readPageTree(Dict *pages, PageAttrs *attrs, int start, std::set< std::pair<int, int> > &readNodes);
  Object *findDestInTree(Object *tree, GString *name, Object *obj);
};

#endif

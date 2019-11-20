#ifndef _ugit_GitDiffCtrl_h_
#define _ugit_GitDiffCtrl_h_

#include <TextDiffCtrl/TextDiffCtrl.h>
#include <ugit/Git.h>

#define DIFF_FILE_START "diff --"
#define PANEL_ONE   0
#define PANEL_LEFT  1
#define PANEL_RIGHT 2

struct GitDiffCtrl : public Upp::ParentCtrl {
  private:
	Upp::Vector<Upp::String> llines, rlines, slines;

	Upp::Option hidelinenumber;
	Upp::Option onepanel;
	Upp::Splitter diffone;
	Upp::Splitter difftwo;
	Upp::TextCompareCtrl single;
	Upp::TextCompareCtrl left;
	Upp::TextCompareCtrl right;

	int  GetNumber(const Upp::String& text, const Upp::String& sign);
	void GetFilePos(int number, int line, int panel);
	void SetTwo(Upp::Vector<Upp::String>& ll, Upp::Vector<Upp::String>& rl);
	void SetOne(Upp::Vector<Upp::String>& sl);
	void TogglePanel();
	void ToggleLineNumber();

  public:
	typedef GitDiffCtrl CLASSNAME;

	Upp::Event<Upp::String, int> WhenLeftDouble;

	virtual void DiffHighlight(Upp::Vector<Upp::LineEdit::Highlight>& hl, const Upp::WString& ln);

	void Clear();
	void Set(Upp::Vector<Upp::String>& lines);
	void SetLinePos(const Upp::String& text);
	int  GetCount() { return single.GetCount(); }

	GitDiffCtrl();
	virtual ~GitDiffCtrl(){};
};

#endif

// vim: ts=4
